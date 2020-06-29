#include "iioaccelerometer.h"

char const * const IIOAccelerometer::id("iio.accelerometer");

IIOAccelerometer::IIOAccelerometer(QSensor *sensor)
    : IIOBase(sensor)
{

}
