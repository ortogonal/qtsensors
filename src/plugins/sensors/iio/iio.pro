TARGET = qtsensors_iio
QT = core sensors

OTHER_FILES = plugin.json

!android:LIBS += -lrt
SOURCES += main.cpp \
    iioaccelerometer.cpp \
    iiobase.cpp

PLUGIN_TYPE = sensors
PLUGIN_CLASS_NAME = IIOSensorPlugin
load(qt_plugin)

HEADERS += \
    iioaccelerometer.h \
    iiobase.h
