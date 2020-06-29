/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qaccelerometer.h>
#include <qsensorbackend.h>
#include <qsensormanager.h>
#include <qsensorplugin.h>

#include <QtCore/QDebug>
#include <QtCore/QFile>

#include "iioaccelerometer.h"
#include "iiobase.h"

class IIOSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)
public:
    void registerSensors() override
    {
        /*QString path = QString::fromLatin1(qgetenv("QT_ACCEL_FILEPATH"));
        if (!path.isEmpty() && !QSensorManager::isBackendRegistered(QAccelerometer::type, LinuxSysAccelerometer::id))
            QSensorManager::registerBackend(QAccelerometer::type, LinuxSysAccelerometer::id, this);*/

        // TODO: We need to handle ALL sensors also if the user hasn't set QT_IIO_SENSORS

        QByteArray sensorListStr = qgetenv("QT_IIO_SENSORS");
        auto sensorList = sensorListStr.split(':');

        for (const auto &sensorName : sensorList) {
            QDir sensorPath = IIOBase::sensorWithName(sensorName);
            auto devicePath = IIOBase::sensorDiveForPath(sensorPath);

            auto types = IIOBase::sensorTypesForPath(sensorPath);

            // TODO: Register multiple sensor for one device
            for (const auto type : types) {
                switch (type) {
                case IIO_ACCEL:
                    QSensorManager::registerBackend(QAccelerometer::type,
                                                    IIOAccelerometer::id,
                                                    this);
                    break;

                case IIO_VOLTAGE:
                case IIO_CURRENT:
                case IIO_POWER:
                case IIO_ANGL_VEL:
                case IIO_MAGN:
                case IIO_LIGHT:
                case IIO_INTENSITY:
                case IIO_PROXIMITY:
                case IIO_TEMP:
                case IIO_INCLI:
                case IIO_ROT:
                case IIO_ANGL:
                case IIO_TIMESTAMP:
                case IIO_CAPACITANCE:
                case IIO_ALTVOLTAGE:
                case IIO_CCT:
                case IIO_PRESSURE:
                case IIO_HUMIDITYRELATIVE:
                case IIO_ACTIVITY:
                case IIO_STEPS:
                case IIO_ENERGY:
                case IIO_DISTANCE:
                case IIO_VELOCITY:
                case IIO_CONCENTRATION:
                case IIO_RESISTANCE:
                case IIO_PH:
                case IIO_UVINDEX:
                case IIO_ELECTRICALCONDUCTIVITY:
                case IIO_COUNT:
                case IIO_INDEX:
                case IIO_GRAVITY:
                    qWarning().noquote() << "Sensor type" << type << "not supported";
                    break;
                }
            }
        }
    }

    QSensorBackend *createBackend(QSensor *sensor) override
    {
        qDebug() << Q_FUNC_INFO << sensor;
        if (sensor->identifier() == IIOAccelerometer::id) {
            return new IIOAccelerometer(sensor);
        }
        return nullptr;
    }
};

#include "main.moc"
