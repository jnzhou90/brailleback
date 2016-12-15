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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/kd.h>

#ifdef HAVE_LINUX_INPUT_H
#include <linux/input.h>
#endif /* HAVE_LINUX_INPUT_H */

#include "log.h"
#include "file.h"
#include "device.h"
#include "timing.h"
#include "system.h"
#include "sys_linux.h"
#include "bitmask.h"

#ifdef HAVE_LINUX_UINPUT_H
#include <linux/uinput.h>

static int
enableUinputKeyEvents (int device) {
  int key;

  if (ioctl(device, UI_SET_EVBIT, EV_KEY) == -1) {
    logSystemError("ioctl[UI_SET_EVBIT,EV_KEY]");
    return 0;
  }

  for (key=0; key<=KEY_MAX; key+=1) {
    if (ioctl(device, UI_SET_KEYBIT, key) == -1) {
      logMessage(LOG_WARNING, "ioctl[UI_SET_KEYBIT] failed for key 0X%X: %s",
                 key, strerror(errno));
    }
  }

  return 1;
}

static int
enableUinputAutorepeat (int device) {
  if (ioctl(device, UI_SET_EVBIT, EV_REP) == -1) {
    logSystemError("ioctl[UI_SET_EVBIT,EV_REP]");
    return 0;
  }

  return 1;
}
#endif /* HAVE_LINUX_UINPUT_H */

char *
getProgramPath (void) {
  char *path = NULL;
  size_t size = 0X80;
  char *buffer = NULL;

  while (1) {
    {
      char *newBuffer = realloc(buffer, size<<=1);

      if (!newBuffer) {
        logMallocError();
        break;
      }

      buffer = newBuffer;
    }

    {
      int length = readlink("/proc/self/exe", buffer, size);

      if (length == -1) {
        if (errno != ENOENT) logSystemError("readlink");
        break;
      }

      if (length < size) {
        buffer[length] = 0;
        if (!(path = strdup(buffer))) logMallocError();
        break;
      }
    }
  }

  if (buffer) free(buffer);
  return path;
}

char *
getBootParameters (const char *name) {
  size_t nameLength = strlen(name);
  char *parameters;

  if ((parameters = strdup(""))) {
    const char *path = "/proc/cmdline";
    FILE *file;

    if ((file = fopen(path, "r"))) {
      char buffer[0X1000];
      char *line = fgets(buffer, sizeof(buffer), file);

      if (line) {
        char *token;

        while ((token = strtok(line, " \n"))) {
          line = NULL;

          if ((strncmp(token, name, nameLength) == 0) &&
              (token[nameLength] == '=')) {
            char *newParameters = strdup(token + nameLength + 1);

            if (newParameters) {
              free(parameters);
              parameters = newParameters;
            } else {
              logMallocError();
            }
          }
        }
      }

      fclose(file);
    }
  } else {
    logMallocError();
  }

  return parameters;
}

#include "sys_exec_unix.h"

#include "sys_mount_linux.h"

#ifdef ENABLE_SHARED_OBJECTS
#define SHARED_OBJECT_LOAD_FLAGS (RTLD_NOW | RTLD_GLOBAL)
#include "sys_shlib_dlfcn.h"
#endif /* ENABLE_SHARED_OBJECTS */

#ifdef ENABLE_BEEPER_SUPPORT
#include "sys_beep_linux.h"
#else /* ENABLE_BEEPER_SUPPORT */
#include "sys_beep_none.h"
#endif /* ENABLE_BEEPER_SUPPORT */

#ifdef ENABLE_PCM_SUPPORT
#if defined(USE_PCM_SUPPORT_ALSA)
#include "sys_pcm_alsa.h"
#elif defined(USE_PCM_SUPPORT_OSS)
#define PCM_OSS_DEVICE_PATH "/dev/dsp"
#include "sys_pcm_oss.h"
#else /* USE_PCM_SUPPORT_ */
#warning PCM interface either unspecified or unsupported
#include "sys_pcm_none.h"
#endif /* USE_PCM_SUPPORT_ */
#endif /* ENABLE_PCM_SUPPORT */

#ifdef ENABLE_MIDI_SUPPORT
#if defined(USE_MIDI_SUPPORT_OSS)
#define MIDI_OSS_DEVICE_PATH "/dev/sequencer";
#include "sys_midi_oss.h"
#elif defined(USE_MIDI_SUPPORT_ALSA)
#include "sys_midi_alsa.h"
#else /* USE_MIDI_SUPPORT_ */
#warning MIDI interface either unspecified or unsupported
#include "sys_midi_none.h"
#endif /* USE_MIDI_SUPPORT_ */
#endif /* ENABLE_MIDI_SUPPORT */

#include "sys_ports_glibc.h"

int
installKernelModule (const char *name, int *status) {
  if (status && *status) return *status == 2;

  {
    const char *command = "modprobe";
    char buffer[0X100];
    if (status) ++*status;

    {
      const char *path = "/proc/sys/kernel/modprobe";
      FILE *stream = fopen(path, "r");

      if (stream) {
        char *line = fgets(buffer, sizeof(buffer), stream);

        if (line) {
          size_t length = strlen(line);
          if (length && (line[length-1] == '\n')) line[--length] = 0;
          if (length) command = line;
        }

        fclose(stream);
      } else {
        logMessage(LOG_WARNING, "cannot open %s: %s", path, strerror(errno));
      }
    }

    {
      const char *const arguments[] = {command, "-q", name, NULL};
      int ok = executeHostCommand(arguments) == 0;

      if (!ok) {
        logMessage(LOG_WARNING, "kernel module not installed: %s", name);
        return 0;
      }

      if (status) ++*status;
    }
  }

  return 1;
}

int
openCharacterDevice (const char *name, int flags, int major, int minor) {
  char *path = getDevicePath(name);
  int descriptor;

  if (!path) {
    descriptor = -1;
  } else if ((descriptor = open(path, flags)) != -1) {
    logMessage(LOG_DEBUG, "device opened: %s: fd=%d", path, descriptor);
  } else {
    logMessage(LOG_DEBUG, "cannot open device: %s: %s", path, strerror(errno));

    if (errno == ENOENT) {
      free(path);
      if ((path = makeWritablePath(locatePathName(name)))) {
        if ((descriptor = open(path, flags)) != -1) {
          logMessage(LOG_DEBUG, "device opened: %s: fd=%d", path, descriptor);
        } else {
          logMessage(LOG_DEBUG, "cannot open device: %s: %s", path, strerror(errno));

          if (errno == ENOENT) {
            mode_t mode = S_IFCHR | S_IRUSR | S_IWUSR;

            if (mknod(path, mode, makedev(major, minor)) == -1) {
              logMessage(LOG_DEBUG, "cannot create device: %s: %s", path, strerror(errno));
            } else {
              logMessage(LOG_DEBUG, "device created: %s mode=%06o major=%d minor=%d",
                         path, mode, major, minor);

              if ((descriptor = open(path, flags)) != -1) {
                logMessage(LOG_DEBUG, "device opened: %s: fd=%d", path, descriptor);
              } else {
                logMessage(LOG_DEBUG, "cannot open device: %s: %s", path, strerror(errno));
              }
            }
          }
        }
      }
    }
  }

  if (descriptor != -1) {
    int ok = 0;
    struct stat status;

    if (fstat(descriptor, &status) == -1) {
      logMessage(LOG_DEBUG, "cannot fstat device: %d [%s]: %s",
                 descriptor, path, strerror(errno));
    } else if (!S_ISCHR(status.st_mode)) {
      logMessage(LOG_DEBUG, "not a character device: %s: fd=%d", path, descriptor);
    } else {
      ok = 1;
    }

    if (!ok) {
      close(descriptor);
      logMessage(LOG_DEBUG, "device closed: %s: fd=%d", path, descriptor);
      descriptor = -1;
    }
  }

  if (path) free(path);
  return descriptor;
}

int
getUinputDevice (void) {
  static int uinputDevice = -1;

#ifdef HAVE_LINUX_UINPUT_H
  if (uinputDevice == -1) {
    const char *name;

    {
      static int status = 0;
      int wait = !status;
      if (!installKernelModule("uinput", &status)) wait = 0;
      if (wait) approximateDelay(500);
    }

    {
      static const char *const names[] = {"uinput", "input/uinput", NULL};
      name = resolveDeviceName(names, "uinput");
    }

    if (name) {
      int device = openCharacterDevice(name, O_WRONLY, 10, 223);

      if (device != -1) {
        struct uinput_user_dev description;
        logMessage(LOG_DEBUG, "uinput opened: %s fd=%d", name, device);
        
        memset(&description, 0, sizeof(description));
        strcpy(description.name, "brltty");

        if (write(device, &description, sizeof(description)) != -1) {
          if (enableUinputKeyEvents(device)) {
            if (enableUinputAutorepeat(device)) {
              if (ioctl(device, UI_DEV_CREATE) != -1) {
                uinputDevice = device;
              } else {
                logSystemError("ioctl[UI_DEV_CREATE]");
              }
            }
          }
        } else {
          logSystemError("write(struct uinput_user_dev)");
        }

        if (device != uinputDevice) {
          close(device);
          logMessage(LOG_DEBUG, "uinput closed");
        }
      } else {
        logMessage(LOG_DEBUG, "cannot open uinput");
      }
    }
  }
#endif /* HAVE_LINUX_UINPUT_H */

  return uinputDevice;
}

int
hasInputEvent (int device, uint16_t type, uint16_t code, uint16_t max) {
#ifdef HAVE_LINUX_INPUT_H
  BITMASK(mask, max+1, long);

  if (ioctl(device, EVIOCGBIT(type, sizeof(mask)), mask) != -1)
    if (BITMASK_TEST(mask, code))
      return 1;
#endif /* HAVE_LINUX_INPUT_H */

  return 0;
}

int
writeInputEvent (uint16_t type, uint16_t code, int32_t value) {
#ifdef HAVE_LINUX_INPUT_H
  int device = getUinputDevice();

  if (device != -1) {
    struct input_event event;

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = type;
    event.code = code;
    event.value = value;

    if (write(device, &event, sizeof(event)) != -1) {
      return 1;
    } else {
      logSystemError("write(struct input_event)");
    }
  }
#endif /* HAVE_LINUX_INPUT_H */

  return 0;
}

#ifdef HAVE_LINUX_INPUT_H
static BITMASK(pressedKeys, KEY_MAX+1, char);
#endif /* HAVE_LINUX_INPUT_H */

int
writeKeyEvent (int key, int press) {
#ifdef HAVE_LINUX_INPUT_H
  if (writeInputEvent(EV_KEY, key, press)) {
    if (press) {
      BITMASK_SET(pressedKeys, key);
    } else {
      BITMASK_CLEAR(pressedKeys, key);
    }

    writeInputEvent(EV_SYN, SYN_REPORT, 0);

    return 1;
  }
#endif /* HAVE_LINUX_INPUT_H */

  return 0;
}

void
releaseAllKeys (void) {
#ifdef HAVE_LINUX_INPUT_H
  int key;
  for (key=0; key<=KEY_MAX; ++key) {
    if (BITMASK_TEST(pressedKeys, key)) {
      if (!writeKeyEvent(key, 0)) break;
    }
  }
#endif /* HAVE_LINUX_INPUT_H */
}
