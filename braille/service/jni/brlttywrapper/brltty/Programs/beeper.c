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

#include "log.h"
#include "timing.h"
#include "system.h"
#include "notes.h"

struct NoteDeviceStruct {
  char mustHaveAtLeastOneField;
};

static NoteDevice *
beeperConstruct (int errorLevel) {
  NoteDevice *device;

  if ((device = malloc(sizeof(*device)))) {
    if (canBeep()) {
      logMessage(LOG_DEBUG, "beeper enabled");
      return device;
    }

    free(device);
  } else {
    logMallocError();
  }

  logMessage(LOG_DEBUG, "beeper not available");
  return NULL;
}

static int
beeperPlay (NoteDevice *device, unsigned char note, unsigned int duration) {
  logMessage(LOG_DEBUG, "tone: msec=%d note=%d", duration, note);

  if (!note) {
    accurateDelay(duration);
    return 1;
  }

  if (asynchronousBeep(getIntegerNoteFrequency(note), duration*4)) {
    accurateDelay(duration);
    stopBeep();
    return 1;
  }

  if (startBeep(getIntegerNoteFrequency(note))) {
    accurateDelay(duration);
    stopBeep();
    return 1;
  }

  return synchronousBeep(getIntegerNoteFrequency(note), duration);
}

static int
beeperFlush (NoteDevice *device) {
  return 1;
}

static void
beeperDestruct (NoteDevice *device) {
  endBeep();
  free(device);
  logMessage(LOG_DEBUG, "beeper disabled");
}

const NoteMethods beeperMethods = {
  beeperConstruct,
  beeperPlay,
  beeperFlush,
  beeperDestruct
};
