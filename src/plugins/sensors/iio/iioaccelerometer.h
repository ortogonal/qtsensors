#ifndef IIOACCELEROMETER_H
#define IIOACCELEROMETER_H

#include "iiobase.h"

class IIOAccelerometer : public IIOBase
{
    // TODO: Needed?
    Q_OBJECT
public:
    static char const * const id;

    IIOAccelerometer(QSensor *sensor);
};

#endif // IIOACCELEROMETER_H
