/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2013 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include "prologue.h"

#include <string.h>

#include "parse.h"
#include "log.h"

char **
splitString (const char *string, char delimiter, int *count) {
  char **array = NULL;

  if (string) {
    while (1) {
      const char *start = string;
      int index = 0;

      if (*start) {
        while (1) {
          const char *end = strchr(start, delimiter);
          size_t length = end? (size_t)(end-start): strlen(start);

          if (array) {
            char *element = malloc(length+1);

            if (!(array[index] = element)) {
              logMallocError();
              deallocateStrings(array);
              array = NULL;
              goto done;
            }

            memcpy(element, start, length);
            element[length] = 0;
          }
          index += 1;

          if (!end) break;
          start = end + 1;
        }
      }

      if (array) {
        array[index] = NULL;
        if (count) *count = index;
        break;
      }

      if (!(array = malloc((index + 1) * sizeof(*array)))) {
        logMallocError();
        break;
      }
    }
  }

done:
  if (!array && count) *count = 0;
  return array;
}

void
deallocateStrings (char **array) {
  char **element = array;
  while (*element) free(*element++);
  free(array);
}

char *
joinStrings (const char *const *strings, int count) {
  char *string;
  size_t length = 0;
  size_t lengths[count];
  int index;

  for (index=0; index<count; index+=1) {
    length += lengths[index] = strlen(strings[index]);
  }

  if ((string = malloc(length+1))) {
    char *target = string;

    for (index=0; index<count; index+=1) {
      length = lengths[index];
      memcpy(target, strings[index], length);
      target += length;
    }

    *target = 0;
  }

  return string;
}

int
rescaleInteger (int value, int from, int to) {
  return (to * (value + (from / (to * 2)))) / from;
}

int
isInteger (int *value, const char *string) {
  if (*string) {
    char *end;
    long l = strtol(string, &end, 0);
    if (!*end) {
      *value = l;
      return 1;
    }
  }
  return 0;
}

int
isUnsignedInteger (unsigned int *value, const char *string) {
  if (*string) {
    char *end;
    unsigned long l = strtoul(string, &end, 0);
    if (!*end) {
      *value = l;
      return 1;
    }
  }
  return 0;
}

int
isLogLevel (int *level, const char *string) {
  if (!*string) return 1;

  {
    int length = strlen(string);
    int index;

    for (index=0; index<logLevelCount; index+=1) {
      if (strncasecmp(string, logLevelNames[index], length) == 0) {
        *level = index;
        return 1;
      }
    }
  }

  {
    int value;

    if (isInteger(&value, string) && (value >= 0) && (value < logLevelCount)) {
      *level = value;
      return 1;
    }
  }

  return 0;
}

int
validateInteger (int *value, const char *string, const int *minimum, const int *maximum) {
  if (*string) {
    int i;
    if (!isInteger(&i, string)) return 0;
    if (minimum && (i < *minimum)) return 0;
    if (maximum && (i > *maximum)) return 0;
    *value = i;
  }
  return 1;
}

int
validateChoice (unsigned int *value, const char *string, const char *const *choices) {
  int length = strlen(string);
  *value = 0;
  if (!length) return 1;

  {
    int index = 0;
    while (choices[index]) {
      if (strncasecmp(string, choices[index], length) == 0) {
        *value = index;
        return 1;
      }
      ++index;
    }
  }

  return 0;
}

int
validateFlag (unsigned int *value, const char *string, const char *on, const char *off) {
  const char *choices[] = {off, on, NULL};
  return validateChoice(value, string, choices);
}

int
validateOnOff (unsigned int *value, const char *string) {
  return validateFlag(value, string, "on", "off");
}

int
validateYesNo (unsigned int *value, const char *string) {
  return validateFlag(value, string, "yes", "no");
}

#ifndef NO_FLOAT
int
isFloat (float *value, const char *string) {
  if (*string) {
    char *end;
    double d = strtod(string, &end);
    if (!*end) {
      *value = d;
      return 1;
    }
  }
  return 0;
}

int
validateFloat (float *value, const char *string, const float *minimum, const float *maximum) {
  if (*string) {
    float f;
    if (!isFloat(&f, string)) return 0;
    if (minimum && (f < *minimum)) return 0;
    if (maximum && (f > *maximum)) return 0;
    *value = f;
  }
  return 1;
}
#endif /* NO_FLOAT */

static int
parseParameters (
  char **values,
  const char *const *names,
  const char *qualifier,
  const char *parameters
) {
  int ok = 0;

  if (parameters && *parameters) {
    char *copiedParameters = strdup(parameters);

    if (copiedParameters) {
      char *name = copiedParameters;

      ok = 1;
      while (1) {
        char *end = strchr(name, ',');
        int done = !end;
        if (!done) *end = 0;

        if (*name) {
          char *value = strchr(name, '=');

          if (!value) {
            logMessage(LOG_ERR, "%s: %s", gettext("missing parameter value"), name);
          } else if (value == name) {
          noName:
            logMessage(LOG_ERR, "%s: %s", gettext("missing parameter name"), name);
          } else {
            size_t nameLength = value++ - name;
            int eligible = 1;

            if (qualifier) {
              char *colon = memchr(name, ':', nameLength);

              if (colon) {
                size_t qualifierLength = (size_t)(colon - name);
                size_t nameAdjustment = qualifierLength + 1;

                eligible = 0;
                if (!qualifierLength) {
                  logMessage(LOG_ERR, "%s: %s", gettext("missing parameter qualifier"), name);
                } else if (!(nameLength -= nameAdjustment)) {
                  goto noName;
                } else if ((qualifierLength == strlen(qualifier)) &&
                           (memcmp(name, qualifier, qualifierLength) == 0)) {
                  name += nameAdjustment;
                  eligible = 1;
                }
              }
            }

            if (eligible) {
              unsigned int index = 0;

              while (names[index]) {
                if (strncasecmp(name, names[index], nameLength) == 0) {
                  char *copiedValue = strdup(value);

                  if (copiedValue) {
                    free(values[index]);
                    values[index] = copiedValue;
                  } else {
                    logMallocError();
                    ok = 0;
                  }

                  break;
                }

                index += 1;
              }

              if (!ok) break;
              if (!names[index]) logMessage(LOG_ERR, "%s: %s", gettext("unsupported parameter"), name);
            }
          }
        }

        if (done) break;
        name = end + 1;
      }

      free(copiedParameters);
    } else {
      logMallocError();
    }
  } else {
    ok = 1;
  }

  return ok;
}

char **
getParameters (const char *const *names, const char *qualifier, const char *parameters) {
  if (!names) {
    static const char *const noNames[] = {NULL};
    names = noNames;
  }

  {
    char **values;
    unsigned int count = 0;
    while (names[count]) count += 1;

    if ((values = malloc((count + 1) * sizeof(*values)))) {
      unsigned int index = 0;

      while (index < count) {
        if (!(values[index] = strdup(""))) {
          logMallocError();
          break;
        }

        index += 1;
      }

      if (index == count) {
        values[index] = NULL;
        if (parseParameters(values, names, qualifier, parameters)) return values;
      }

      deallocateStrings(values);
    } else {
      logMallocError();
    }
  }

  return NULL;
}

void
logParameters (const char *const *names, char **values, const char *description) {
  if (names && values) {
    while (*names) {
      logMessage(LOG_INFO, "%s: %s=%s", description, *names, *values);
      ++names;
      ++values;
    }
  }
}
