#ifndef DISTRIBUTEDDEVICE_H
#define DISTRIBUTEDDEVICE_H

#include <QString>
#include <QVector>

#include "sensormodule.h"

class DistributedDevice
{
public:
    DistributedDevice(const QString& mac, const QString& name = QString());

    QString getMAC() const;
    void setMAC(const QString& mac);

    QString getName() const;
    void setName(const QString& name);

    void addSensor(const QString& sensorName);
    SensorModule& getSensor(const QString& sensorName);
    QVector<SensorModule>& getSensors();

    void addIndicator(const QString& sensorName, const QString &indicatorName,
                      const QMap<time_t, QVariant>& data = QMap<time_t, QVariant>());
    SensorsIndicator& getIndicator(const QString& sensorName, const QString &indicatorName);

    void addData(const QString& sensorName, const QString &indicatorName, const time_t t, const QVariant data);
    QVariant getData(const QString& sensorName, const QString &indicatorName, const time_t t) const;
    QMap<time_t, QVariant>& getAllData(const QString& sensorName, const QString &indicatorName);

    void setAlive(bool isAlive = true);
    bool getAlive() const;

private:
    QString m_mac;
    QString m_name;
    QVector<SensorModule> m_sensors;

    bool m_alive;
};

#endif // DISTRIBUTEDDEVICE_H
