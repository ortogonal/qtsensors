#include "iiobase.h"

#include <QtCore/QDebug>

#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>

constexpr const auto sysFsIioDevices = "/sys/bus/iio/devices";
constexpr const auto deviceName = "iio:device";

/*constexpr const char *const iio_direction[] = {
    [0] = "in",
    [1] = "out",
};

constexpr const char *const iio_chan_type_name_spec[] = {
    [IIO_VOLTAGE] = "voltage",
    [IIO_CURRENT] = "current",
    [IIO_POWER] = "power",
    [IIO_ACCEL] = "accel",
    [IIO_ANGL_VEL] = "anglvel",
    [IIO_MAGN] = "magn",
    [IIO_LIGHT] = "illuminance",
    [IIO_INTENSITY] = "intensity",
    [IIO_PROXIMITY] = "proximity",
    [IIO_TEMP] = "temp",
    [IIO_INCLI] = "incli",
    [IIO_ROT] = "rot",
    [IIO_ANGL] = "angl",
    [IIO_TIMESTAMP] = "timestamp",
    [IIO_CAPACITANCE] = "capacitance",
    [IIO_ALTVOLTAGE] = "altvoltage",
    [IIO_CCT] = "cct",
    [IIO_PRESSURE] = "pressure",
    [IIO_HUMIDITYRELATIVE] = "humidityrelative",
    [IIO_ACTIVITY] = "activity",
    [IIO_STEPS] = "steps",
    [IIO_ENERGY] = "energy",
    [IIO_DISTANCE] = "distance",
    [IIO_VELOCITY] = "velocity",
    [IIO_CONCENTRATION] = "concentration",
    [IIO_RESISTANCE] = "resistance",
    [IIO_PH] = "ph",
    [IIO_UVINDEX] = "uvindex",
    [IIO_ELECTRICALCONDUCTIVITY] = "electricalconductivity",
    [IIO_COUNT] = "count",
    [IIO_INDEX] = "index",
    [IIO_GRAVITY] = "gravity",
};
*/

constexpr const std::pair<iio_modifier, const char *> iioModifierNames[] = {
    {IIO_MOD_X, "x"},
    {IIO_MOD_Y, "y"},
    {IIO_MOD_Z, "z"},
    {IIO_MOD_X_AND_Y, "x&y"},
    {IIO_MOD_X_AND_Z, "x&z"},
    {IIO_MOD_Y_AND_Z, "y&z"},
    {IIO_MOD_X_AND_Y_AND_Z, "x&y&z"},
    {IIO_MOD_X_OR_Y, "x|y"},
    {IIO_MOD_X_OR_Z, "x|z"},
    {IIO_MOD_Y_OR_Z, "y|z"},
    {IIO_MOD_X_OR_Y_OR_Z, "x|y|z"},
    {IIO_MOD_ROOT_SUM_SQUARED_X_Y, "sqrt(x^2+y^2)"},
    {IIO_MOD_SUM_SQUARED_X_Y_Z, "x^2+y^2+z^2"},
    {IIO_MOD_LIGHT_BOTH, "both"},
    {IIO_MOD_LIGHT_IR, "ir"},
    {IIO_MOD_LIGHT_CLEAR, "clear"},
    {IIO_MOD_LIGHT_RED, "red"},
    {IIO_MOD_LIGHT_GREEN, "green"},
    {IIO_MOD_LIGHT_BLUE, "blue"},
    {IIO_MOD_LIGHT_UV, "uv"},
    {IIO_MOD_QUATERNION, "quaternion"},
    {IIO_MOD_TEMP_AMBIENT, "ambient"},
    {IIO_MOD_TEMP_OBJECT, "object"},
    {IIO_MOD_NORTH_MAGN, "from_north_magnetic"},
    {IIO_MOD_NORTH_TRUE, "from_north_true"},
    {IIO_MOD_NORTH_MAGN_TILT_COMP, "from_north_magnetic_tilt_comp"},
    {IIO_MOD_NORTH_TRUE_TILT_COMP, "from_north_true_tilt_comp"},
    {IIO_MOD_RUNNING, "running"},
    {IIO_MOD_JOGGING, "jogging"},
    {IIO_MOD_WALKING, "walking"},
    {IIO_MOD_STILL, "still"},
    {IIO_MOD_ROOT_SUM_SQUARED_X_Y_Z, "sqrt(x^2+y^2+z^2)"},
    {IIO_MOD_I, "i"},
    {IIO_MOD_Q, "q"},
    {IIO_MOD_CO2, "co2"},
    {IIO_MOD_VOC, "voc"},
};

constexpr const std::pair<IIOBase::iio_chan_info_enum, const char *> iioChannelInfoPostfix[] = {
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW, "raw"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_PROCESSED, "input"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_SCALE, "scale"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_OFFSET, "offset"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_CALIBSCALE, "calibscale"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_CALIBBIAS, "calibbias"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_PEAK, "peak_raw"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_PEAK_SCALE, "peak_scale"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_QUADRATURE_CORRECTION_RAW, "quadrature_correction_raw"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_AVERAGE_RAW, "mean_raw"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_LOW_PASS_FILTER_3DB_FREQUENCY, "filter_low_pass_3db_frequency"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_HIGH_PASS_FILTER_3DB_FREQUENCY, "filter_high_pass_3db_frequency"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_SAMP_FREQ, "sampling_frequency"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_FREQUENCY, "frequency"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_PHASE, "phase"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_HARDWAREGAIN, "hardwaregain"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_HYSTERESIS, "hysteresis"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_INT_TIME, "integration_time"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_ENABLE, "en"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_CALIBHEIGHT, "calibheight"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_CALIBWEIGHT, "calibweight"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_DEBOUNCE_COUNT, "debounce_count"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_DEBOUNCE_TIME, "debounce_time"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_CALIBEMISSIVITY, "calibemissivity"},
    {IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_OVERSAMPLING_RATIO, "oversampling_ratio"},
};

constexpr const std::pair<iio_chan_type, const char *> iioTypeNameSpec[] = {
    {IIO_VOLTAGE, "voltage"},
    {IIO_CURRENT, "current"},
    {IIO_POWER, "power"},
    {IIO_ACCEL, "accel"},
    {IIO_ANGL_VEL, "anglvel"},
    {IIO_MAGN, "magn"},
    {IIO_LIGHT, "illuminance"},
    {IIO_INTENSITY, "intensity"},
    {IIO_PROXIMITY, "proximity"},
    {IIO_TEMP, "temp"},
    {IIO_INCLI, "incli"},
    {IIO_ROT, "rot"},
    {IIO_ANGL, "angl"},
    {IIO_TIMESTAMP, "timestamp"},
    {IIO_CAPACITANCE, "capacitance"},
    {IIO_ALTVOLTAGE, "altvoltage"},
    {IIO_CCT, "cct"},
    {IIO_PRESSURE, "pressure"},
    {IIO_HUMIDITYRELATIVE, "humidityrelative"},
    {IIO_ACTIVITY, "activity"},
    {IIO_STEPS, "steps"},
    {IIO_ENERGY, "energy"},
    {IIO_DISTANCE, "distance"},
    {IIO_VELOCITY, "velocity"},
    {IIO_CONCENTRATION, "concentration"},
    {IIO_RESISTANCE, "resistance"},
    {IIO_PH, "ph"},
    {IIO_UVINDEX, "uvindex"},
    {IIO_ELECTRICALCONDUCTIVITY, "electricalconductivity"},
    {IIO_COUNT, "count"},
    {IIO_INDEX, "index"},
    {IIO_GRAVITY, "gravity"},
};

struct sensorAttributes
{
    enum iio_chan_type sensor_type;
    struct channel_t
    {
        enum iio_modifier modifier;
        enum IIOBase::iio_chan_info_enum channelInfo;
    };
    std::initializer_list<channel_t> channels;
};

constexpr const struct sensorAttributes iioSensorReqAttributes[] = {
    {IIO_ACCEL,
     {{IIO_MOD_X, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW},
      /*{IIO_MOD_Y, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW},
      {IIO_MOD_Z, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW}*/}},
    {IIO_ANGL_VEL,
     {{IIO_MOD_X, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW},
      {IIO_MOD_Y, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW},
      {IIO_MOD_Z, IIOBase::iio_chan_info_enum::IIO_CHAN_INFO_RAW}}},
};

IIOBase::IIOBase(QSensor *sensor)
    : QSensorBackend(sensor)
{
    sensor->setDataRate(3);
    addDataRate(1, 10);
    qDebug() << sensor->availableDataRates();
}

void IIOBase::start() {}

void IIOBase::stop() {}

/* Use sysfs to check if we "have" a sensor with
 * specific name
 */
QDir IIOBase::sensorWithName(const QByteArray &name)
{
    QDir devices(sysFsIioDevices);
    if (devices.exists()) {
        QDirIterator it(devices.absolutePath());
        while (it.hasNext()) {
            auto filePath = it.next();

            if (it.fileInfo().fileName().startsWith(deviceName)) {
                QFile nameFile(filePath + "/name");
                if (nameFile.open(QFile::ReadOnly)) {
                    auto data = nameFile.readAll().trimmed();

                    if (data == name) {
                        return QDir(filePath);
                    }
                }
            }
        }
    }
    return QDir();
}

QString IIOBase::sensorDiveForPath(const QDir &deviceDirectory)
{
    if (deviceDirectory.exists()) {
        QFile devFile(deviceDirectory.absoluteFilePath("uevent"));
        if (devFile.open(QFile::ReadOnly)) {
            QTextStream in(&devFile);

            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith("DEVNAME")) {
                    auto data = line.split('=');
                    if (data.size() == 2) {
                        return "/dev/" + data.at(1);
                    }
                }
            }
        }
    }
    return QString();
}

QVector<iio_chan_type> IIOBase::sensorTypesForPath(const QDir &deviceDirectroy)
{
    QVector<iio_chan_type> types;

    if (deviceDirectroy.exists()) {
        for (const auto &sensor : iioSensorReqAttributes) {
            bool hasRequiredChannels = true;
            for (const auto channel : sensor.channels) {
                qDebug() << Q_FUNC_INFO << channelFilename(sensor.sensor_type, channel.modifier, channel.channelInfo);
                auto chFilename = channelFilename(sensor.sensor_type, channel.modifier, channel.channelInfo);
                hasRequiredChannels &= QFile::exists(deviceDirectroy.absoluteFilePath(chFilename));
            }

            if (hasRequiredChannels) {
                types.append(sensor.sensor_type);
            }
        }
    }

    return types;
}

QString IIOBase::channelFilename(const iio_chan_type type, const iio_modifier modifer, const IIOBase::iio_chan_info_enum postfix)
{
    for (const auto& typePair : iioTypeNameSpec) {
        if (typePair.first == type) {
            for (const auto& modifierPair : iioModifierNames) {
                if (modifierPair.first == modifer) {
                    for (const auto& postFixPair : iioChannelInfoPostfix) {
                        if (postFixPair.first == postfix) {
                            return QString("in_%1_%2_%3").arg(typePair.second, modifierPair.second, postFixPair.second);
                        }
                    }
                }
            }
        }
    }
    return QString();
}
