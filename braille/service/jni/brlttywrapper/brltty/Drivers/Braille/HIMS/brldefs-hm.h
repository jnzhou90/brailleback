/*
 * libbrlapi - A library providing access to braille terminals for applications.
 *
 * Copyright (C) 2002-2013 Sébastien Hinderer <Sebastien.Hinderer@ens-lyon.org>
 *
 * libbrlapi comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU Lesser General Public License, as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at your option) any
 * later version. Please see the file LICENSE-LGPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#ifndef BRLTTY_INCLUDED_HM_BRLDEFS
#define BRLTTY_INCLUDED_HM_BRLDEFS

typedef enum {
  /* dot keys */
  HM_KEY_Dot1  = 1,
  HM_KEY_Dot2  = 2,
  HM_KEY_Dot3  = 3,
  HM_KEY_Dot4  = 4,
  HM_KEY_Dot5  = 5,
  HM_KEY_Dot6  = 6,
  HM_KEY_Dot7  = 7,
  HM_KEY_Dot8  = 8,
  HM_KEY_Space = 9,

  /* Braille Sense/Edge keys */
  HM_KEY_BS_F1 = 10,
  HM_KEY_BS_F2 = 11,
  HM_KEY_BS_F3 = 12,
  HM_KEY_BS_F4 = 13,

  /* Braille Sense keys */
  HM_KEY_BS_Backward = 14,
  HM_KEY_BS_Forward  = 15,

  /* SyncBraille keys */
  HM_KEY_SB_LeftUp    = 13,
  HM_KEY_SB_RightUp   = 14,
  HM_KEY_SB_RightDown = 15,
  HM_KEY_SB_LeftDown  = 16,

  /* Braille Edge keys */
  HM_KEY_BE_LeftScrollUp    = 17,
  HM_KEY_BE_RightScrollUp   = 18,
  HM_KEY_BE_LeftScrollDown  = 19,
  HM_KEY_BE_RightScrollDown = 20,

  HM_KEY_BE_F5 = 21,
  HM_KEY_BE_F6 = 22,
  HM_KEY_BE_F7 = 23,
  HM_KEY_BE_F8 = 24,

  HM_KEY_BE_LeftArrowUp     = 25,
  HM_KEY_BE_LeftArrowDown   = 26,
  HM_KEY_BE_LeftArrowLeft   = 27,
  HM_KEY_BE_LeftArrowRight  = 28,

  HM_KEY_BE_RightArrowUp    = 29,
  HM_KEY_BE_RightArrowDown  = 30,
  HM_KEY_BE_RightArrowLeft  = 31,
  HM_KEY_BE_RightArrowRight = 32
} HM_NavigationKey;

typedef enum {
  HM_SET_NavigationKeys = 0,
  HM_SET_RoutingKeys
} HM_KeySet;

#endif /* BRLTTY_INCLUDED_HM_BRLDEFS */ 
