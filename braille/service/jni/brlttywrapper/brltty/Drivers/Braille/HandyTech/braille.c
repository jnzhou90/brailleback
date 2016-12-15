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
#include <time.h>
#include <errno.h>

#include "bitfield.h"
#include "log.h"
#include "parse.h"
#include "timing.h"
#include "ascii.h"

typedef enum {
  PARM_SETTIME
} DriverParameter;
#define BRLPARMS "settime"

#define BRLSTAT ST_AlvaStyle
#define BRL_HAVE_STATUS_CELLS
#define BRL_HAVE_PACKET_IO
#include "brl_driver.h"
#include "touch.h"
#include "brldefs-ht.h"

BEGIN_KEY_NAME_TABLE(routing)
  KEY_SET_ENTRY(HT_SET_RoutingKeys, "RoutingKey"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLE(dots)
  KEY_NAME_ENTRY(HT_KEY_B1, "B1"),
  KEY_NAME_ENTRY(HT_KEY_B2, "B2"),
  KEY_NAME_ENTRY(HT_KEY_B3, "B3"),
  KEY_NAME_ENTRY(HT_KEY_B4, "B4"),

  KEY_NAME_ENTRY(HT_KEY_B5, "B5"),
  KEY_NAME_ENTRY(HT_KEY_B6, "B6"),
  KEY_NAME_ENTRY(HT_KEY_B7, "B7"),
  KEY_NAME_ENTRY(HT_KEY_B8, "B8"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLE(keypad)
  KEY_NAME_ENTRY(HT_KEY_B12, "B12"),
  KEY_NAME_ENTRY(HT_KEY_Zero, "Zero"),
  KEY_NAME_ENTRY(HT_KEY_B13, "B13"),
  KEY_NAME_ENTRY(HT_KEY_B14, "B14"),

  KEY_NAME_ENTRY(HT_KEY_B11, "B11"),
  KEY_NAME_ENTRY(HT_KEY_One, "One"),
  KEY_NAME_ENTRY(HT_KEY_Two, "Two"),
  KEY_NAME_ENTRY(HT_KEY_Three, "Three"),

  KEY_NAME_ENTRY(HT_KEY_B10, "B10"),
  KEY_NAME_ENTRY(HT_KEY_Four, "Four"),
  KEY_NAME_ENTRY(HT_KEY_Five, "Five"),
  KEY_NAME_ENTRY(HT_KEY_Six, "Six"),

  KEY_NAME_ENTRY(HT_KEY_B9, "B9"),
  KEY_NAME_ENTRY(HT_KEY_Seven, "Seven"),
  KEY_NAME_ENTRY(HT_KEY_Eight, "Eight"),
  KEY_NAME_ENTRY(HT_KEY_Nine, "Nine"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLE(rockers)
  KEY_NAME_ENTRY(HT_KEY_Escape, "LeftRockerTop"),
  KEY_NAME_ENTRY(HT_KEY_Return, "LeftRockerBottom"),

  KEY_NAME_ENTRY(HT_KEY_Up, "RightRockerTop"),
  KEY_NAME_ENTRY(HT_KEY_Down, "RightRockerBottom"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLE(modular)
  KEY_NAME_ENTRY(HT_KEY_Up, "Left"),
  KEY_NAME_ENTRY(HT_KEY_Down, "Right"),

  KEY_NAME_ENTRY(HT_KEY_STATUS+0, "Status1"),
  KEY_NAME_ENTRY(HT_KEY_STATUS+1, "Status2"),
  KEY_NAME_ENTRY(HT_KEY_STATUS+2, "Status3"),
  KEY_NAME_ENTRY(HT_KEY_STATUS+3, "Status4"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(mdlr)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(keypad),
  KEY_NAME_TABLE(modular),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLE(modularEvolution)
  KEY_NAME_ENTRY(HT_KEY_Space, "Left"),
  KEY_NAME_ENTRY(HT_KEY_SpaceRight, "Right"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(me64)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(rockers),
  KEY_NAME_TABLE(modularEvolution),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLES(me88)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(rockers),
  KEY_NAME_TABLE(keypad),
  KEY_NAME_TABLE(modularEvolution),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLE(brailleStar)
  KEY_NAME_ENTRY(HT_KEY_Space, "SpaceLeft"),
  KEY_NAME_ENTRY(HT_KEY_SpaceRight, "SpaceRight"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(bs40)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(rockers),
  KEY_NAME_TABLE(brailleStar),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLES(bs80)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(rockers),
  KEY_NAME_TABLE(keypad),
  KEY_NAME_TABLE(brailleStar),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLES(ab40)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(rockers),
  KEY_NAME_TABLE(brailleStar),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLE(brailleWave)
  KEY_NAME_ENTRY(HT_KEY_Up, "Left"),
  KEY_NAME_ENTRY(HT_KEY_Down, "Right"),

  KEY_NAME_ENTRY(HT_KEY_Escape, "Escape"),
  KEY_NAME_ENTRY(HT_KEY_Space, "Space"),
  KEY_NAME_ENTRY(HT_KEY_Return, "Return"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(wave)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(brailleWave),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLE(easyBraille)
  KEY_NAME_ENTRY(HT_KEY_Up, "Left"),
  KEY_NAME_ENTRY(HT_KEY_Down, "Right"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(easy)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(dots),
  KEY_NAME_TABLE(easyBraille),
END_KEY_NAME_TABLES

BEGIN_KEY_NAME_TABLE(basicBraille)
  KEY_NAME_ENTRY(HT_KEY_B2, "Display3"),
  KEY_NAME_ENTRY(HT_KEY_B3, "Display2"),
  KEY_NAME_ENTRY(HT_KEY_B4, "Display1"),
  KEY_NAME_ENTRY(HT_KEY_B5, "Display4"),
  KEY_NAME_ENTRY(HT_KEY_B6, "Display5"),
  KEY_NAME_ENTRY(HT_KEY_B7, "Display6"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(bb)
  KEY_NAME_TABLE(routing),
  KEY_NAME_TABLE(basicBraille),
END_KEY_NAME_TABLES

typedef enum {
  HT_BWK_Backward = 0X01,
  HT_BWK_Forward = 0X08,

  HT_BWK_Escape = 0X02,
  HT_BWK_Enter = 0X04
} HT_BookwormKey;

BEGIN_KEY_NAME_TABLE(bookworm)
  KEY_NAME_ENTRY(HT_BWK_Backward, "Backward"),
  KEY_NAME_ENTRY(HT_BWK_Forward, "Forward"),

  KEY_NAME_ENTRY(HT_BWK_Escape, "Escape"),
  KEY_NAME_ENTRY(HT_BWK_Enter, "Enter"),
END_KEY_NAME_TABLE

BEGIN_KEY_NAME_TABLES(bkwm)
  KEY_NAME_TABLE(bookworm),
END_KEY_NAME_TABLES

DEFINE_KEY_TABLE(mdlr)
DEFINE_KEY_TABLE(me64)
DEFINE_KEY_TABLE(me88)
DEFINE_KEY_TABLE(bs40)
DEFINE_KEY_TABLE(bs80)
DEFINE_KEY_TABLE(ab40)
DEFINE_KEY_TABLE(wave)
DEFINE_KEY_TABLE(easy)
DEFINE_KEY_TABLE(bb)
DEFINE_KEY_TABLE(bkwm)

BEGIN_KEY_TABLE_LIST
  &KEY_TABLE_DEFINITION(mdlr),
  &KEY_TABLE_DEFINITION(me64),
  &KEY_TABLE_DEFINITION(me88),
  &KEY_TABLE_DEFINITION(bs40),
  &KEY_TABLE_DEFINITION(bs80),
  &KEY_TABLE_DEFINITION(ab40),
  &KEY_TABLE_DEFINITION(wave),
  &KEY_TABLE_DEFINITION(easy),
  &KEY_TABLE_DEFINITION(bb),
  &KEY_TABLE_DEFINITION(bkwm),
END_KEY_TABLE_LIST

static const unsigned char BookwormSessionEnd[] = {0X05, 0X07};	/* bookworm trailer to display braille */

typedef int (ByteInterpreter) (unsigned char byte);
static ByteInterpreter interpretByte_key;
static ByteInterpreter interpretByte_Bookworm;

typedef int (CellWriter) (BrailleDisplay *brl);
static CellWriter writeCells_statusAndText;
static CellWriter writeCells_Bookworm;
static CellWriter writeCells_Evolution;

static BrailleFirmnessSetter setFirmness;

static BrailleSensitivitySetter setSensitivity_Evolution;
static BrailleSensitivitySetter setSensitivity_ActiveBraille;

typedef struct {
  const char *name;
  const KeyTableDefinition *keyTableDefinition;

  ByteInterpreter *interpretByte;
  CellWriter *writeCells;
  BrailleFirmnessSetter *setFirmness;
  BrailleSensitivitySetter *setSensitivity;

  const unsigned char *sessionEndAddress;

  HT_ModelIdentifier identifier:8;
  unsigned char textCells;
  unsigned char statusCells;

  unsigned char sessionEndLength;

  unsigned hasATC:1; /* Active Tactile Control */
} ModelEntry;

#define HT_BYTE_SEQUENCE(name,bytes) .name##Address = bytes, .name##Length = sizeof(bytes)
static const ModelEntry modelTable[] = {
  { .identifier = HT_MODEL_Modular20,
    .name = "Modular 20+4",
    .textCells = 20,
    .statusCells = 4,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(mdlr),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_Modular40,
    .name = "Modular 40+4",
    .textCells = 40,
    .statusCells = 4,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(mdlr),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_Modular80,
    .name = "Modular 80+4",
    .textCells = 80,
    .statusCells = 4,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(mdlr),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_ModularEvolution64,
    .name = "Modular Evolution 64",
    .textCells = 64,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(me64),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_Evolution,
    .setSensitivity = setSensitivity_Evolution,
    .hasATC = 1
  }
  ,
  { .identifier = HT_MODEL_ModularEvolution88,
    .name = "Modular Evolution 88",
    .textCells = 88,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(me88),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_Evolution,
    .setSensitivity = setSensitivity_Evolution,
    .hasATC = 1
  }
  ,
  { .identifier = HT_MODEL_BrailleWave,
    .name = "Braille Wave",
    .textCells = 40,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(wave),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_Bookworm,
    .name = "Bookworm",
    .textCells = 8,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(bkwm),
    .interpretByte = interpretByte_Bookworm,
    .writeCells = writeCells_Bookworm,
    HT_BYTE_SEQUENCE(sessionEnd, BookwormSessionEnd)
  }
  ,
  { .identifier = HT_MODEL_Braillino,
    .name = "Braillino",
    .textCells = 20,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(bs40),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_BrailleStar40,
    .name = "Braille Star 40",
    .textCells = 40,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(bs40),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_BrailleStar80,
    .name = "Braille Star 80",
    .textCells = 80,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(bs80),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_EasyBraille,
    .name = "Easy Braille",
    .textCells = 40,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(easy),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_statusAndText
  }
  ,
  { .identifier = HT_MODEL_ActiveBraille,
    .name = "Active Braille",
    .textCells = 40,
    .statusCells = 0,
    .keyTableDefinition = &KEY_TABLE_DEFINITION(ab40),
    .interpretByte = interpretByte_key,
    .writeCells = writeCells_Evolution,
    .setFirmness = setFirmness,
    .setSensitivity = setSensitivity_ActiveBraille,
    .hasATC = 0
  }
  ,
#define HT_BASIC_BRAILLE(cells)                     \
  { .identifier = HT_MODEL_BasicBraille##cells,     \
    .name = "Basic Braille " STRINGIFY(cells),      \
    .textCells = cells,                             \
    .statusCells = 0,                               \
    .keyTableDefinition = &KEY_TABLE_DEFINITION(bb),\
    .interpretByte = interpretByte_key,             \
    .writeCells = writeCells_Evolution              \
  }
  HT_BASIC_BRAILLE(16),
  HT_BASIC_BRAILLE(20),
  HT_BASIC_BRAILLE(32),
  HT_BASIC_BRAILLE(40),
  HT_BASIC_BRAILLE(48),
  HT_BASIC_BRAILLE(64),
  HT_BASIC_BRAILLE(80),
  HT_BASIC_BRAILLE(160)
#undef HT_BASIC_BRAILLE
  ,
  { /* end of table */
    .name = NULL
  }
};
#undef HT_BYTE_SEQUENCE

#define BRLROWS		1
#define MAX_STCELLS	4	/* highest number of status cells */

/* Global variables */
static unsigned char *rawData = NULL;		/* translated data to send to Braille */
static unsigned char *prevData = NULL;	/* previously sent raw data */
static unsigned char rawStatus[MAX_STCELLS];		/* to hold status info */
static unsigned char prevStatus[MAX_STCELLS];	/* to hold previous status */
static const ModelEntry *model;		/* points to terminal model config struct */

typedef struct {
  int (*openPort) (char **parameters, const char *device);
  void (*closePort) ();
  int (*awaitInput) (int milliseconds);
  int (*readBytes) (unsigned char *buffer, int length, int wait);
  int (*writeBytes) (const unsigned char *buffer, int length, unsigned int *delay);
} InputOutputOperations;

static const InputOutputOperations *io;
static const unsigned int baud = 19200;
static unsigned int charactersPerSecond;

/* Serial IO */
#include "io_serial.h"

static SerialDevice *serialDevice = NULL;			/* file descriptor for Braille display */

static int
openSerialPort (char **parameters, const char *device) {
  if ((serialDevice = serialOpenDevice(device))) {
    serialSetParity(serialDevice, SERIAL_PARITY_ODD);

    if (serialRestartDevice(serialDevice, baud)) {
      return 1;
    }

    serialCloseDevice(serialDevice);
    serialDevice = NULL;
  }
  return 0;
}

static void
closeSerialPort (void) {
  if (serialDevice) {
    serialCloseDevice(serialDevice);
    serialDevice = NULL;
  }
}

static int
awaitSerialInput (int milliseconds) {
  return serialAwaitInput(serialDevice, milliseconds);
}

static int
readSerialBytes (unsigned char *buffer, int count, int wait) {
  const int timeout = 100;
  return serialReadData(serialDevice, buffer, count,
                        (wait? timeout: 0), timeout);
}

static int
writeSerialBytes (const unsigned char *buffer, int length, unsigned int *delay) {
  int count = serialWriteData(serialDevice, buffer, length);
  if (delay && (count != -1)) *delay += (length * 1000 / charactersPerSecond) + 1;
  return count;
}

static const InputOutputOperations serialOperations = {
  openSerialPort, closeSerialPort,
  awaitSerialInput, readSerialBytes, writeSerialBytes
};

/* USB IO */
#include "io_usb.h"

static UsbChannel *usb = NULL;

#define HT_HID_REPORT_TIMEOUT 100

typedef enum {
  HT_HID_RPT_OutData    = 0X01, /* receive data from device */
  HT_HID_RPT_InData     = 0X02, /* send data to device */
  HT_HID_RPT_InCommand  = 0XFB, /* run USB-HID firmware command */
  HT_HID_RPT_OutVersion = 0XFC, /* get version of USB-HID firmware */
  HT_HID_RPT_OutBaud    = 0XFD, /* get baud rate of serial connection */
  HT_HID_RPT_InBaud     = 0XFE, /* set baud rate of serial connection */
} HT_HidReportNumber;

typedef enum {
  HT_HID_CMD_FlushBuffers = 0X01, /* flush input and output buffers */
} HtHidCommand;

static size_t hidReportSize_OutData;
static size_t hidReportSize_InData;
static size_t hidReportSize_InCommand;
static size_t hidReportSize_OutVersion;
static size_t hidReportSize_OutBaud;
static size_t hidReportSize_InBaud;

static uint16_t hidFirmwareVersion;
static unsigned char *hidInputReport = NULL;
#define hidInputLength (hidInputReport[1])
#define hidInputBuffer (&hidInputReport[2])
static unsigned char hidInputOffset;

static int
getHidReport (unsigned char number, unsigned char *buffer, int size) {
  int result = usbHidGetReport(usb->device, usb->definition.interface,
                               number, buffer, size, HT_HID_REPORT_TIMEOUT);
  if (result > 0 && buffer[0] != number) {
    logMessage(LOG_WARNING, "unexpected HID report number: expected %02X, received %02X",
               number, buffer[0]);
    errno = EIO;
    result = -1;
  }

  return result;
}

static int
setHidReport (const unsigned char *report, int size) {
  return usbHidSetReport(usb->device, usb->definition.interface,
                         report[0], report, size, HT_HID_REPORT_TIMEOUT);
}

typedef struct {
  HT_HidReportNumber number;
  size_t *size;
} ReportEntry;

static void
getHidReportSizes (const ReportEntry *table) {
  unsigned char *items;
  ssize_t length = usbHidGetItems(usb->device, usb->definition.interface, 0,
                                  &items, HT_HID_REPORT_TIMEOUT);

  if (items) {
    const ReportEntry *report = table;

    while (report->number) {
      usbHidGetReportSize(items, length, report->number, report->size);
      report += 1;
    }

    free(items);
  }
}

static void
allocateHidInputBuffer (void) {
  if (hidReportSize_OutData) {
    if ((hidInputReport = malloc(hidReportSize_OutData))) {
      hidInputLength = 0;
      hidInputOffset = 0;
    } else {
      logMessage(LOG_ERR, "HID input buffer not allocated: %s", strerror(errno));
    }
  }
}

static void
getHidFirmwareVersion (void) {
  hidFirmwareVersion = 0;

  if (hidReportSize_OutVersion) {
    unsigned char report[hidReportSize_OutVersion];
    int result = getHidReport(HT_HID_RPT_OutVersion, report, sizeof(report));

    if (result > 0) {
      hidFirmwareVersion = (report[1] << 8) | report[2];
      logMessage(LOG_INFO, "Firmware Version: %u.%u", report[1], report[2]);
    }
  }
}

static void
executeHidFirmwareCommand (HtHidCommand command) {
  if (hidReportSize_InCommand) {
    unsigned char report[hidReportSize_InCommand];

    report[0] = HT_HID_RPT_InCommand;
    report[1] = command;

    setHidReport(report, sizeof(report));
  }
}

typedef struct {
  void (*initialize) (void);
  int (*awaitInput) (int milliseconds);
  int (*readBytes) (unsigned char *buffer, int length, int wait);
  int (*writeBytes) (const unsigned char *buffer, int length);
} UsbOperations;

static const UsbOperations *usbOps;

static void
initializeUsb1 (void) {
}

static int
awaitUsbInput1 (int milliseconds) {
  return usbAwaitInput(usb->device, usb->definition.inputEndpoint, milliseconds);
}

static int
readUsbBytes1 (unsigned char *buffer, int length, int wait) {
  int timeout = 100;

  return usbReapInput(usb->device, usb->definition.inputEndpoint, buffer, length,
                      (wait? timeout: 0), timeout);
}

static int
writeUsbBytes1 (const unsigned char *buffer, int length) {
  return usbWriteEndpoint(usb->device, usb->definition.outputEndpoint,
                          buffer, length, 1000);
}

static const UsbOperations usbOperations1 = {
  .initialize = initializeUsb1,
  .awaitInput = awaitUsbInput1,
  .readBytes = readUsbBytes1,
  .writeBytes = writeUsbBytes1
};

static void
initializeUsb2 (void) {
  static const ReportEntry reportTable[] = {
    {.number=HT_HID_RPT_OutData, .size=&hidReportSize_OutData},
    {.number=HT_HID_RPT_InData, .size=&hidReportSize_InData},
    {.number=HT_HID_RPT_InCommand, .size=&hidReportSize_InCommand},
    {.number=HT_HID_RPT_OutVersion, .size=&hidReportSize_OutVersion},
    {.number=HT_HID_RPT_OutBaud, .size=&hidReportSize_OutBaud},
    {.number=HT_HID_RPT_InBaud, .size=&hidReportSize_InBaud},
    {.number=0}
  };
  getHidReportSizes(reportTable);
  allocateHidInputBuffer();
  getHidFirmwareVersion();
  executeHidFirmwareCommand(HT_HID_CMD_FlushBuffers);
}

static int
awaitUsbInput2 (int milliseconds) {
  if (hidReportSize_OutData) {
    TimePeriod period;

    if (hidInputOffset < hidInputLength) return 1;
    startTimePeriod(&period, milliseconds);

    while (1) {
      int result = getHidReport(HT_HID_RPT_OutData, hidInputReport,
                                hidReportSize_OutData);

      if (result == -1) return 0;
      hidInputOffset = 0;
      if (hidInputLength > 0) return 1;

      if (afterTimePeriod(&period, NULL)) break;
      approximateDelay(10);
    }
  }

  errno = EAGAIN;
  return 0;
}

static int
readUsbBytes2 (unsigned char *buffer, int length, int wait) {
  int count = 0;

  while (count < length) {
    if (!io->awaitInput(wait? 100: 0)) {
      count = -1;
      break;
    }

    {
      int amount = MIN(length-count, hidInputLength-hidInputOffset);

      memcpy(&buffer[count], &hidInputBuffer[hidInputOffset], amount);
      hidInputOffset += amount;
      count += amount;
    }
  }

  return count;
}

static int
writeUsbBytes2 (const unsigned char *buffer, int length) {
  int index = 0;

  if (hidReportSize_InData) {
    while (length) {
      unsigned char report[hidReportSize_InData];
      unsigned char count = MIN(length, (sizeof(report) - 2));
      int result;

      report[0] = HT_HID_RPT_InData;
      report[1] = count;
      memcpy(report+2, &buffer[index], count);
      memset(&report[count+2], 0, sizeof(report)-count-2);

      result = setHidReport(report, sizeof(report));
      if (result == -1) return -1;

      index += count;
      length -= count;
    }
  }

  return index;
}

static const UsbOperations usbOperations2 = {
  .initialize = initializeUsb2,
  .awaitInput = awaitUsbInput2,
  .readBytes = readUsbBytes2,
  .writeBytes = writeUsbBytes2
};

static void
initializeUsb3 (void) {
  static const ReportEntry reportTable[] = {
    {.number=HT_HID_RPT_OutData, .size=&hidReportSize_OutData},
    {.number=HT_HID_RPT_InData, .size=&hidReportSize_InData},
    {.number=0}
  };
  getHidReportSizes(reportTable);
  allocateHidInputBuffer();
}

static int
awaitUsbInput3 (int milliseconds) {
  if (hidReportSize_OutData) {
    TimePeriod period;

    if (hidInputOffset < hidInputLength) return 1;
    startTimePeriod(&period, milliseconds);

    while (1) {
      int result = usbReapInput(usb->device, usb->definition.inputEndpoint,
                                hidInputReport, hidReportSize_OutData,
                                0, 100);

      if (result == -1) return 0;
      if (result > 0 && hidInputLength > 0) {
        hidInputOffset = 0;
        return 1;
      }

      if (afterTimePeriod(&period, NULL)) break;
      approximateDelay(10);
    }
  }

  errno = EAGAIN;
  return 0;
}

static int
writeUsbBytes3 (const unsigned char *buffer, int length) {
  int index = 0;

  if (hidReportSize_InData) {
    while (length) {
      unsigned char report[hidReportSize_InData];
      unsigned char count = MIN(length, (sizeof(report) - 2));
      int result;

      report[0] = HT_HID_RPT_InData;
      report[1] = count;
      memcpy(report+2, &buffer[index], count);
      memset(&report[count+2], 0, sizeof(report)-count-2);

      result = usbWriteEndpoint(usb->device, usb->definition.outputEndpoint,
                                report, sizeof(report), 1000);
      if (result == -1) return -1;

      index += count;
      length -= count;
    }
  }

  return index;
}

static const UsbOperations usbOperations3 = {
  .initialize = initializeUsb3,
  .awaitInput = awaitUsbInput3,
  .readBytes = readUsbBytes2,
  .writeBytes = writeUsbBytes3
};

static int
openUsbPort (char **parameters, const char *device) {
  const SerialParameters serial = {
    SERIAL_DEFAULT_PARAMETERS,
    .baud = baud,
    .parity = SERIAL_PARITY_ODD
  };

  const UsbChannelDefinition definitions[] = {
    { /* GoHubs chip */
      .vendor=0X0921, .product=0X1200,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .serial = &serial,
      .data=&usbOperations1
    }
    ,
    { /* FTDI chip */
      .vendor=0X0403, .product=0X6001,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=2,
      .serial = &serial,
      .data=&usbOperations1
    }
    ,
    { /* Easy Braille (HID) */
      .vendor=0X1FE4, .product=0X0044,
      .configuration=1, .interface=0, .alternative=0,
      .data=&usbOperations2
    }
    ,
    { /* Braille Star 40 (HID) */
      .vendor=0X1FE4, .product=0X0074,
      .configuration=1, .interface=0, .alternative=0,
      .data=&usbOperations2
    }
    ,
    { /* USB-HID adapter */
      .vendor=0X1FE4, .product=0X0003,
      .configuration=1, .interface=0, .alternative=0,
      .data=&usbOperations2
    }
    ,
    { /* Active Braille */
      .vendor=0X1FE4, .product=0X0054,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 16 */
      .vendor=0X1FE4, .product=0X0081,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 20 */
      .vendor=0X1FE4, .product=0X0082,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 32 */
      .vendor=0X1FE4, .product=0X0083,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 40 */
      .vendor=0X1FE4, .product=0X0084,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 48 */
      .vendor=0X1FE4, .product=0X008A,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 64 */
      .vendor=0X1FE4, .product=0X0086,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 80 */
      .vendor=0X1FE4, .product=0X0087,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { /* Basic Braille 160 */
      .vendor=0X1FE4, .product=0X008B,
      .configuration=1, .interface=0, .alternative=0,
      .inputEndpoint=1, .outputEndpoint=1,
      .data=&usbOperations3
    }
    ,
    { .vendor=0 }
  };

  if ((usb = usbFindChannel(definitions, (void *)device))) {
    usbOps = usb->definition.data;
    usbOps->initialize();

    return 1;
  }
  return 0;
}

static void
closeUsbPort (void) {
  if (hidInputReport) {
    free(hidInputReport);
    hidInputReport = NULL;
  }

  if (usb) {
    usbCloseChannel(usb);
    usb = NULL;
  }
}

static int
awaitUsbInput (int milliseconds) {
  return usbOps->awaitInput(milliseconds);
}

static int
readUsbBytes (unsigned char *buffer, int length, int wait) {
  int count = usbOps->readBytes(buffer, length, wait);

  if (count != -1) return count;
  if (errno == EAGAIN) return 0;
  return -1;
}

static int
writeUsbBytes (const unsigned char *buffer, int length, unsigned int *delay) {
  if (delay) *delay += (length * 1000 / charactersPerSecond) + 1;

  return usbOps->writeBytes(buffer, length);
}

static const InputOutputOperations usbOperations = {
  openUsbPort, closeUsbPort,
  awaitUsbInput, readUsbBytes, writeUsbBytes
};

/* Bluetooth IO */
#include "io_bluetooth.h"

static BluetoothConnection *bluetoothConnection = NULL;

static int
openBluetoothPort (char **parameters, const char *device) {
  return (bluetoothConnection = bthOpenConnection(device, 1, 0)) != NULL;
}

static void
closeBluetoothPort (void) {
  if (bluetoothConnection) {
    bthCloseConnection(bluetoothConnection);
    bluetoothConnection = NULL;
  }
}

static int
awaitBluetoothInput (int milliseconds) {
  return bthAwaitInput(bluetoothConnection, milliseconds);
}

static int
readBluetoothBytes (unsigned char *buffer, int length, int wait) {
  const int timeout = 100;
  return bthReadData(bluetoothConnection, buffer, length,
                     (wait? timeout: 0), timeout);
}

static int
writeBluetoothBytes (const unsigned char *buffer, int length, unsigned int *delay) {
  int count = bthWriteData(bluetoothConnection, buffer, length);
  if (delay) *delay += (length * 1000 / charactersPerSecond) + 1;
  if (count != length) {
    if (count == -1) {
      logSystemError("HandyTech Bluetooth write");
    } else {
      logMessage(LOG_WARNING, "Trunccated bluetooth write: %d < %d", count, length);
    }
  }
  return count;
}

static const InputOutputOperations bluetoothOperations = {
  openBluetoothPort, closeBluetoothPort,
  awaitBluetoothInput, readBluetoothBytes, writeBluetoothBytes
};

typedef enum {
  BDS_OFF,
  BDS_READY,
  BDS_WRITING
} BrailleDisplayState;
static BrailleDisplayState currentState = BDS_OFF;
static TimePeriod statePeriod;
static unsigned int retryCount = 0;
static unsigned char updateRequired = 0;

static ssize_t
brl_readPacket (BrailleDisplay *brl, void *buffer, size_t size) {
  unsigned char *packet = buffer;
  size_t offset = 0;
  size_t length = 0;

  while (1) {
    unsigned char byte;

    {
      int started = offset > 0;
      int count = io->readBytes(&byte, 1, started);

      if (count != 1) {
        if (!count && started) logPartialPacket(packet, offset);
        return count;
      }
    }

    if (offset == 0) {
      switch (byte) {
        default:
          length = 1;
          break;

        case HT_PKT_OK:
          length = 2;
          break;

        case HT_PKT_Extended:
          length = 4;
          break;
      }
    } else {
      switch (packet[0]) {
        case HT_PKT_Extended:
          if (offset == 2) {
            length += byte;
          } else if (offset == 4) {
            if ((packet[1] == HT_MODEL_ActiveBraille) && 
                (packet[2] == 2) &&
                (packet[3] == HT_EXTPKT_Confirmation) &&
                (byte == 0X15))
              length += 1;
          }
          break;
      }
    }

    if (offset < size) {
      packet[offset] = byte;
    } else {
      if (offset == size) logTruncatedPacket(packet, offset);
      logDiscardedByte(byte);
    }

    if (++offset == length) {
      if (offset <= size) {
        int ok = 0;

        switch (packet[0]) {
          case HT_PKT_Extended:
            if (packet[length-1] == SYN) ok = 1;
            break;

          default:
            ok = 1;
            break;
        }

        if (ok) {
          logInputPacket(packet, offset);
          return length;
        }

        logCorruptPacket(packet, offset);
      }

      offset = 0;
      length = 0;
    }
  }
}

static ssize_t
brl_writePacket (BrailleDisplay *brl, const void *packet, size_t length) {
  logOutputPacket(packet, length);
  return io->writeBytes(packet, length, &brl->writeDelay);
}

static void
setState (BrailleDisplayState state) {
  if (state == currentState) {
    ++retryCount;
  } else {
    retryCount = 0;
    currentState = state;
  }

  startTimePeriod(&statePeriod, 1000);
  // logMessage(LOG_DEBUG, "State: %d+%d", currentState, retryCount);
}

static int
brl_reset (BrailleDisplay *brl) {
  static const unsigned char packet[] = {HT_PKT_Reset};
  return brl_writePacket(brl, packet, sizeof(packet)) != -1;
}

static void
deallocateBuffers (void) {
  if (rawData) {
    free(rawData);
    rawData = NULL;
  }

  if (prevData) {
    free(prevData);
    prevData = NULL;
  }
}

static int
reallocateBuffer (unsigned char **buffer, size_t size) {
  void *address = realloc(*buffer, size);
  int allocated = address != NULL;
  if (allocated) {
    *buffer = address;
  } else {
    logSystemError("buffer allocation");
  }
  return allocated;
}

static int
identifyModel (BrailleDisplay *brl, unsigned char identifier) {
  for (
    model = modelTable;
    model->name && (model->identifier != identifier);
    model++
  );

  if (!model->name) {
    logMessage(LOG_ERR, "Detected unknown HandyTech model with ID %02X.",
               identifier);
    return 0;
  }

  logMessage(LOG_INFO, "Detected %s: %d data %s, %d status %s.",
             model->name,
             model->textCells, (model->textCells == 1)? "cell": "cells",
             model->statusCells, (model->statusCells == 1)? "cell": "cells");

  brl->textColumns = model->textCells;			/* initialise size of display */
  brl->textRows = BRLROWS;
  brl->statusColumns = model->statusCells;
  brl->statusRows = 1;

  brl->keyBindings = model->keyTableDefinition->bindings;
  brl->keyNameTables = model->keyTableDefinition->names;

  brl->setFirmness = model->setFirmness;
  brl->setSensitivity = model->setSensitivity;

  if (!reallocateBuffer(&rawData, brl->textColumns*brl->textRows)) return 0;
  if (!reallocateBuffer(&prevData, brl->textColumns*brl->textRows)) return 0;

  memset(rawStatus, 0, model->statusCells);
  memset(rawData, 0, model->textCells);

  retryCount = 0;
  updateRequired = 0;
  currentState = BDS_OFF;
  setState(BDS_READY);

  return 1;
}

static int
writeExtendedPacket (
  BrailleDisplay *brl, HT_ExtendedPacketType type,
  const unsigned char *data, unsigned char size
) {
  HT_Packet packet;
  packet.fields.type = HT_PKT_Extended;
  packet.fields.data.extended.model = model->identifier;
  packet.fields.data.extended.length = size + 1; /* type byte is included */
  packet.fields.data.extended.type = type;
  memcpy(packet.fields.data.extended.data.bytes, data, size);
  packet.fields.data.extended.data.bytes[size] = SYN;
  size += 5; /* EXT, ID, LEN, TYPE, ..., SYN */
  return brl_writePacket(brl, (unsigned char *)&packet, size) == size;
}

static int
setAtcMode (BrailleDisplay *brl, unsigned char value) {
  const unsigned char data[] = {value};
  return writeExtendedPacket(brl, HT_EXTPKT_SetAtcMode, data, sizeof(data));
}

static int
setFirmness (BrailleDisplay *brl, BrailleFirmness setting) {
  const unsigned char data[] = {setting * 2 / BRL_FIRMNESS_MAXIMUM};
  return writeExtendedPacket(brl, HT_EXTPKT_SetFirmness, data, sizeof(data));
}

static int
setSensitivity_Evolution (BrailleDisplay *brl, BrailleSensitivity setting) {
  const unsigned char data[] = {0XFF - (setting * 0XF0 / BRL_SENSITIVITY_MAXIMUM)};
  return writeExtendedPacket(brl, HT_EXTPKT_SetAtcSensitivity, data, sizeof(data));
}

static int
setSensitivity_ActiveBraille (BrailleDisplay *brl, BrailleSensitivity setting) {
  const unsigned char data[] = {setting * 6 / BRL_SENSITIVITY_MAXIMUM};
  return writeExtendedPacket(brl, HT_EXTPKT_SetAtcSensitivity2, data, sizeof(data));
}

typedef int (DateTimeProcessor) (BrailleDisplay *brl, const HT_DateTime *dateTime);
static DateTimeProcessor *dateTimeProcessor = NULL;

static int
requestDateTime (BrailleDisplay *brl, DateTimeProcessor *processor) {
  int result = writeExtendedPacket(brl, HT_EXTPKT_GetRTC, NULL, 0);

  if (result) {
    dateTimeProcessor = processor;
  }

  return result;
}

static int
logDateTime (BrailleDisplay *brl, const HT_DateTime *dateTime) {
  logMessage(LOG_INFO,
             "date and time of %s:"
             " %04" PRIu16 "-%02" PRIu8 "-%02" PRIu8
             " %02" PRIu8 ":%02" PRIu8 ":%02" PRIu8,
             model->name,
             getBigEndian16(dateTime->year), dateTime->month, dateTime->day,
             dateTime->hour, dateTime->minute, dateTime->second);

  return 1;
}

static int
synchronizeDateTime (BrailleDisplay *brl, const HT_DateTime *dateTime) {
  long int delta;
  TimeValue hostTime;
  getCurrentTime(&hostTime);

  {
    TimeValue deviceTime;

    {
      TimeComponents components = {
        .year = getBigEndian16(dateTime->year),
        .month = dateTime->month - 1,
        .day = dateTime->day - 1,
        .hour = dateTime->hour,
        .minute = dateTime->minute,
        .second = dateTime->second
      };

      makeTimeValue(&deviceTime, &components);
    }

    delta = millisecondsBetween(&hostTime, &deviceTime);
    if (delta < 0) delta = -delta;
  }

  if (delta > 1000) {
    TimeComponents components;
    HT_DateTime payload;

    expandTimeValue(&hostTime, &components);
    putLittleEndian16(&payload.year, components.year);
    payload.month = components.month + 1;
    payload.day = components.day + 1;
    payload.hour = components.hour;
    payload.minute = components.minute;
    payload.second = components.second;

    logMessage(LOG_DEBUG, "Time difference between host and device: %ld.%03ld",
               (delta / MSECS_PER_SEC), (delta % MSECS_PER_SEC));

    if (writeExtendedPacket(brl, HT_EXTPKT_SetRTC,
                            (unsigned char *)&payload, sizeof(payload))) {
      return requestDateTime(brl, logDateTime);
    }
  }

  return 1;
}

static int
brl_construct (BrailleDisplay *brl, char **parameters, const char *device) {
  unsigned int setTime = 0;

  if (isSerialDevice(&device)) {
    io = &serialOperations;
  } else if (isUsbDevice(&device)) {
    io = &usbOperations;
  } else if (isBluetoothDevice(&device)) {
    io = &bluetoothOperations;
  } else {
    unsupportedDevice(device);
    return 0;
  }

  rawData = prevData = NULL;		/* clear pointers */
  charactersPerSecond = baud / 11;

  if (*parameters[PARM_SETTIME])
    if (!validateYesNo(&setTime, parameters[PARM_SETTIME]))
      logMessage(LOG_WARNING, "%s: %s", "invalid set time setting", parameters[PARM_SETTIME]);
  setTime = !!setTime;

  if (io->openPort(parameters, device)) {
    int tries = 0;

    while (brl_reset(brl)) {
      while (io->awaitInput(100)) {
        HT_Packet response;
        int length = brl_readPacket(brl, &response, sizeof(response));

        if (length > 0) {
          if (response.fields.type == HT_PKT_OK) {
            if (identifyModel(brl, response.fields.data.ok.model)) {
              makeOutputTable(dotsTable_ISO11548_1);

              if (model->hasATC) {
                setAtcMode(brl, 1);

                touchAnalyzeCells(brl, NULL);
                brl->touchEnabled = 1;
              }

              if (setTime) {
                if (model->identifier == HT_MODEL_ActiveBraille) {
                  requestDateTime(brl, synchronizeDateTime);
                } else {
                  logMessage(LOG_INFO, "%s does not support setting the clock", model->name);
                }
              }

              return 1;
            }

            deallocateBuffers();
          }
        }
      }
      if (errno != EAGAIN) break;

      if (++tries == 3) break;
    }

    io->closePort();
  }

  return 0;
}

static void
brl_destruct (BrailleDisplay *brl) {
  if (model->sessionEndLength) {
    brl_writePacket(brl, model->sessionEndAddress, model->sessionEndLength);
  }
  io->closePort();

  deallocateBuffers();
}

static int
writeCells (BrailleDisplay *brl) {
  if (!model->writeCells(brl)) return 0;
  setState(BDS_WRITING);
  return 1;
}

static int
writeCells_statusAndText (BrailleDisplay *brl) {
  unsigned char buffer[1 + model->statusCells + model->textCells];
  unsigned char *byte = buffer;

  *byte++ = 0X01;
  byte = mempcpy(byte, rawStatus, model->statusCells);
  byte = mempcpy(byte, rawData, model->textCells);

  return brl_writePacket(brl, buffer, byte-buffer) != -1;
}

static int
writeCells_Bookworm (BrailleDisplay *brl) {
  unsigned char buffer[1 + model->statusCells + model->textCells + 1];

  buffer[0] = 0X01;
  memcpy(buffer+1, rawData, model->textCells);
  buffer[sizeof(buffer)-1] = SYN;
  return brl_writePacket(brl, buffer, sizeof(buffer)) != -1;
}

static int
writeCells_Evolution (BrailleDisplay *brl) {
  return writeExtendedPacket(brl, HT_EXTPKT_Braille, rawData, model->textCells);
}

static int
updateCells (BrailleDisplay *brl) {
  if (!updateRequired) return 1;
  if (currentState != BDS_READY) return 1;

  if (!writeCells(brl)) {
    setState(BDS_OFF);
    return 0;
  }

  updateRequired = 0;
  return 1;
}

static int
brl_writeWindow (BrailleDisplay *brl, const wchar_t *text) {
  const size_t cellCount = model->textCells;
  if (cellsHaveChanged(prevData, brl->buffer, cellCount, NULL, NULL, NULL)) {
    translateOutputCells(rawData, prevData, cellCount);
    updateRequired = 1;
  }
  updateCells(brl);
  return 1;
}

static int
brl_writeStatus (BrailleDisplay *brl, const unsigned char *st) {
  const size_t cellCount = model->statusCells;
  if (cellsHaveChanged(prevStatus, st, cellCount, NULL, NULL, NULL)) {
    translateOutputCells(rawStatus, prevStatus, cellCount);
    updateRequired = 1;
  }
  return 1;
}

static int
interpretByte_key (unsigned char byte) {
  int release = (byte & HT_KEY_RELEASE) != 0;
  if (release) byte ^= HT_KEY_RELEASE;

  if ((byte >= HT_KEY_ROUTING) &&
      (byte < (HT_KEY_ROUTING + model->textCells))) {
    return enqueueKeyEvent(HT_SET_RoutingKeys, byte - HT_KEY_ROUTING, !release);
  }

  if ((byte >= HT_KEY_STATUS) &&
      (byte < (HT_KEY_STATUS + model->statusCells))) {
    return enqueueKeyEvent(HT_SET_NavigationKeys, byte, !release);
  }

  if ((byte > 0) && (byte < 0X20)) {
    return enqueueKeyEvent(HT_SET_NavigationKeys, byte, !release);
  }

  return 0;
}

static int
interpretByte_Bookworm (unsigned char byte) {
  static const unsigned char keys[] = {
    HT_BWK_Backward,
    HT_BWK_Forward,
    HT_BWK_Escape,
    HT_BWK_Enter,
    0
  };

  const unsigned char *key = keys;
  const HT_KeySet set = HT_SET_NavigationKeys;

  if (!byte) return 0;
  {
    unsigned char bits = byte;
    while (*key) bits &= ~*key++;
    if (bits) return 0;
    key = keys;
  }

  while (*key) {
    if ((byte & *key) && !enqueueKeyEvent(set, *key, 1)) return 0;
    key += 1;
  }

  do {
    key -= 1;
    if ((byte & *key) && !enqueueKeyEvent(set, *key, 0)) return 0;
  } while (key != keys);

  return 1;
}

static int
brl_readCommand (BrailleDisplay *brl, KeyTableCommandContext context) {
  int noInput = 1;

  while (1) {
    HT_Packet packet;
    int size = brl_readPacket(brl, &packet, sizeof(packet));

    if (size == -1) return BRL_CMD_RESTARTBRL;
    if (size == 0) break;
    noInput = 0;

    /* a kludge to handle the Bookworm going offline */
    if (model->identifier == HT_MODEL_Bookworm) {
      if (packet.fields.type == 0X06) {
        if (currentState != BDS_OFF) {
          /* if we get another byte right away then the device
           * has gone offline and is echoing its display
           */
          if (io->awaitInput(10)) {
            setState(BDS_OFF);
            continue;
          }

          /* if an input error occurred then restart the driver */
          if (errno != EAGAIN) return BRL_CMD_RESTARTBRL;

          /* no additional input so fall through and interpret the packet as keys */
        }
      }
    }

    switch (packet.fields.type) {
      case HT_PKT_OK:
        if (packet.fields.data.ok.model == model->identifier) {
          setState(BDS_READY);
          updateRequired = 1;
          continue;
        }
        break;

      default:
        switch (currentState) {
          case BDS_OFF:
            continue;

          case BDS_WRITING:
            switch (packet.fields.type) {
              case HT_PKT_NAK:
                updateRequired = 1;
              case HT_PKT_ACK:
                if (model->hasATC) touchAnalyzeCells(brl, prevData);
                setState(BDS_READY);
                continue;

              case HT_PKT_Extended: {
                unsigned char length UNUSED = packet.fields.data.extended.length - 1;
                const unsigned char *bytes = &packet.fields.data.extended.data.bytes[0];

                switch (packet.fields.data.extended.type) {
                  case HT_EXTPKT_Confirmation:
                    switch (bytes[0]) {
                      case HT_PKT_NAK:
                        updateRequired = 1;
                      case HT_PKT_ACK:
                        if (model->hasATC) touchAnalyzeCells(brl, prevData);
                        setState(BDS_READY);
                        continue;

                      default:
                        break;
                    }
                    break;

                  default:
                    break;
                }
                break;
              }

              default:
                break;
            }

          case BDS_READY:
            switch (packet.fields.type) {
              case HT_PKT_Extended: {
                unsigned char length = packet.fields.data.extended.length - 1;
                const unsigned char *bytes = &packet.fields.data.extended.data.bytes[0];

                switch (packet.fields.data.extended.type) {
                  case HT_EXTPKT_Key:
                    if (model->interpretByte(bytes[0])) {
                      updateCells(brl);
                      return EOF;
                    }
                    break;

                  case HT_EXTPKT_Scancode: {
                    while (length--)
                      enqueueCommand(BRL_BLK_PASSAT | BRL_ARG(*bytes++));
                    break;
                  }

                  case HT_EXTPKT_GetRTC: {
                    const HT_DateTime *const payload = (HT_DateTime *)bytes;
                    DateTimeProcessor *processor = dateTimeProcessor;
                    dateTimeProcessor = NULL;

                    if (processor) {
                      if (!processor(brl, payload)) {
                        break;
                      }
                    }

                    continue;
                  }

                  case HT_EXTPKT_AtcInfo: {
                    unsigned int cellCount = model->textCells + model->statusCells;
                    unsigned char pressureValues[cellCount];
                    const unsigned char *pressure;

                    if (bytes[0]) {
                      int cellIndex = bytes[0] - 1;
                      int dataIndex;

                      memset(pressureValues, 0, cellCount);
                      for (dataIndex=1; dataIndex<length; dataIndex++) {
                        unsigned char byte = bytes[dataIndex];
                        unsigned char nibble;

                        nibble = HIGH_NIBBLE(byte);
                        pressureValues[cellIndex++] = nibble | (nibble >> 4);

                        nibble = LOW_NIBBLE(byte);
                        pressureValues[cellIndex++] = nibble | (nibble << 4);
                      }

                      pressure = &pressureValues[0];
                    } else {
                      pressure = NULL;
                    }

                    {
                      int command = touchAnalyzePressure(brl, pressure);
                      if (command != EOF) return command;
                    }

                    continue;
                  }

                  case HT_EXTPKT_ReadingPosition: {
                    const size_t cellCount = model->textCells + model->statusCells;
                    unsigned char pressureValues[cellCount];
                    const unsigned char *pressure;

                    if (bytes[0] != 0XFF) {
                      const int cellIndex = bytes[0];

                      memset(pressureValues, 0, cellCount);
                      pressureValues[cellIndex] = 0XFF;

                      pressure = &pressureValues[0];
                    } else {
                      pressure = NULL;
                    }

                    {
                      int command = touchAnalyzePressure(brl, pressure);
                      if (command != EOF) return command;
                    }

                    continue;
                  }

                  default:
                    break;
                }
                break;
              }

              default:
                if (model->interpretByte(packet.fields.type)) {
                  updateCells(brl);
                  return EOF;
                }
                break;
            }
            break;
        }
        break;
    }

    logUnexpectedPacket(packet.bytes, size);
    logMessage(LOG_WARNING, "state %d", currentState);
  }

  if (noInput) {
    switch (currentState) {
      case BDS_OFF:
        break;

      case BDS_READY:
        break;

      case BDS_WRITING:
        if (afterTimePeriod(&statePeriod, NULL)) {
          if (retryCount > 3) return BRL_CMD_RESTARTBRL;
          if (!writeCells(brl)) return BRL_CMD_RESTARTBRL;
        }
        break;
    }
  }
  updateCells(brl);

  return EOF;
}
