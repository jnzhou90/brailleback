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

#warning MIDI support not available on this platform

#include "log.h"

MidiDevice *
openMidiDevice (int errorLevel, const char *device) {
  logMessage(errorLevel, "MIDI device not supported.");
  return NULL;
}

void
closeMidiDevice (MidiDevice *midi) {
}

int
flushMidiDevice (MidiDevice *midi) {
  return 1;
}

int
setMidiInstrument (MidiDevice *midi, unsigned char channel, unsigned char instrument) {
  return 1;
}

int
beginMidiBlock (MidiDevice *midi) {
  return 1;
}

int
endMidiBlock (MidiDevice *midi) {
  return 1;
}

int
startMidiNote (MidiDevice *midi, unsigned char channel, unsigned char note, unsigned char volume) {
  return 1;
}

int
stopMidiNote (MidiDevice *midi, unsigned char channel) {
  return 1;
}

int
insertMidiWait (MidiDevice *midi, int duration) {
  return 1;
}
