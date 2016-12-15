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

#ifndef BRLTTY_INCLUDED_MESSAGE
#define BRLTTY_INCLUDED_MESSAGE

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* message.h - send a message to Braille and speech */

/* Prototype: */
extern int message (const char *mode, const char *text, short flags);


/* Flags for the second argument: */
#define MSG_SILENT 1		/* Prevent output to speech */
#define MSG_WAITKEY 2		/* Wait for a key after the message is displayed */
#define MSG_NODELAY 4 /* message now automatically delays for DISPDEL ms,
			 unless this flag is set. */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BRLTTY_INCLUDED_MESSAGE */
