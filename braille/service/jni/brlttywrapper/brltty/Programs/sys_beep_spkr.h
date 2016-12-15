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

#ifdef HAVE_MACHINE_SPEAKER_H
#include <machine/speaker.h>

#include "log.h"

static int speaker = -1;

static int
getSpeaker (void) {
  if (speaker == -1) {
    if ((speaker = open("/dev/speaker", O_WRONLY)) != -1) {
      logMessage(LOG_DEBUG, "Speaker opened: fd=%d", speaker);
    } else {
      logSystemError("speaker open");
    }
  }
  return speaker;
}
#else /* HAVE_MACHINE_SPEAKER_H */
#warning beeps not available on this installation: no <machine/speaker.h>
#endif /* HAVE_MACHINE_SPEAKER_H */

int
canBeep (void) {
#ifdef HAVE_MACHINE_SPEAKER_H
  return 1;
#else /* HAVE_MACHINE_SPEAKER_H */
  return 0;
#endif /* HAVE_MACHINE_SPEAKER_H */
}

int
synchronousBeep (unsigned short frequency, unsigned short milliseconds) {
#ifdef HAVE_MACHINE_SPEAKER_H
  int speaker = getSpeaker();
  if (speaker != -1) {
    tone_t tone;
    memset(&tone, 0, sizeof(tone));
    tone.frequency = frequency;
    tone.duration = (milliseconds + 9) / 10;
    if (ioctl(speaker, SPKRTONE, &tone) != -1) return 1;
    logSystemError("speaker tone");
  }
#endif /* HAVE_MACHINE_SPEAKER_H */
  return 0;
}

int
asynchronousBeep (unsigned short frequency, unsigned short milliseconds) {
  return 0;
}

int
startBeep (unsigned short frequency) {
  return 0;
}

int
stopBeep (void) {
  return 0;
}

void
endBeep (void) {
#ifdef HAVE_MACHINE_SPEAKER_H
  if (speaker != -1) {
    close(speaker);
    speaker = -1;
  }
#endif /* HAVE_MACHINE_SPEAKER_H */
}
