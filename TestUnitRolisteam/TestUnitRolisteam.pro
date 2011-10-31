TEMPLATE = subdirs

#include(TestUnitRolisteamV1.pri)
DEFINES =DSTDIR=$$SRCDIR
SUBDIRS = data #chat MainWindow  picture   map Dice

OBJECTS_DIR = obj
OTHER_FILES = TestUnitRolisteamV2.pri

