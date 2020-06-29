#ifndef IIOBASE_H
#define IIOBASE_H

#include <QDir>
#include <QSensor>
#include <QSensorBackend>

#include <linux/iio/types.h>

class IIOBase : public QSensorBackend
{
    Q_OBJECT
public:
    enum class iio_chan_info_enum {
        IIO_CHAN_INFO_RAW = 0,
        IIO_CHAN_INFO_PROCESSED,
        IIO_CHAN_INFO_SCALE,
        IIO_CHAN_INFO_OFFSET,
        IIO_CHAN_INFO_CALIBSCALE,
        IIO_CHAN_INFO_CALIBBIAS,
        IIO_CHAN_INFO_PEAK,
        IIO_CHAN_INFO_PEAK_SCALE,
        IIO_CHAN_INFO_QUADRATURE_CORRECTION_RAW,
        IIO_CHAN_INFO_AVERAGE_RAW,
        IIO_CHAN_INFO_LOW_PASS_FILTER_3DB_FREQUENCY,
        IIO_CHAN_INFO_HIGH_PASS_FILTER_3DB_FREQUENCY,
        IIO_CHAN_INFO_SAMP_FREQ,
        IIO_CHAN_INFO_FREQUENCY,
        IIO_CHAN_INFO_PHASE,
        IIO_CHAN_INFO_HARDWAREGAIN,
        IIO_CHAN_INFO_HYSTERESIS,
        IIO_CHAN_INFO_INT_TIME,
        IIO_CHAN_INFO_ENABLE,
        IIO_CHAN_INFO_CALIBHEIGHT,
        IIO_CHAN_INFO_CALIBWEIGHT,
        IIO_CHAN_INFO_DEBOUNCE_COUNT,
        IIO_CHAN_INFO_DEBOUNCE_TIME,
        IIO_CHAN_INFO_CALIBEMISSIVITY,
        IIO_CHAN_INFO_OVERSAMPLING_RATIO,
        };

    IIOBase(QSensor *sensor);

    // QSensorBackend interface
    void start() override;
    void stop() override;

    static QDir sensorWithName(const QByteArray &name);
    static QString sensorDiveForPath(const QDir &deviceDirectory);
    static QVector<iio_chan_type> sensorTypesForPath(const QDir &deviceDirectroy);
    static QString channelFilename(const iio_chan_type type, const iio_modifier modifer, const iio_chan_info_enum postfix);

protected:
    QDir m_sysFsDevicePath;

private:
};

#endif // IIOBASE_H
