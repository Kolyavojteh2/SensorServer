#include "distributeddevice.h"

DistributedDevice::DistributedDevice(const QString& mac, const QString& name) : m_mac(mac), m_name(name), m_alive(false) {}

QString DistributedDevice::getMAC() const
{
    return m_mac;
}

void DistributedDevice::setMAC(const QString& mac)
{
    m_mac = mac;
}

QString DistributedDevice::getName() const
{
    return m_name;
}

void DistributedDevice::setName(const QString& name)
{
    m_name = name;
}

void DistributedDevice::addSensor(const QString& sensorName)
{
    m_sensors.push_back(SensorModule(sensorName));
}

SensorModule& DistributedDevice::getSensor(const QString& sensorName)
{
    static SensorModule dummy("");

    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
            return it;
    }

    return dummy;
}

QVector<SensorModule>& DistributedDevice::getSensors()
{
    return m_sensors;
}

void DistributedDevice::addIndicator(const QString& sensorName, const QString &indicatorName, const QMap<time_t, QVariant>& data)
{
    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
        {
            it.addIndicator(indicatorName, data);
            return;
        }
    }
}

SensorsIndicator& DistributedDevice::getIndicator(const QString& sensorName, const QString &indicatorName)
{
    static SensorsIndicator dummy("");

    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
        {
            return it.getIndicator(indicatorName);
        }
    }

    return dummy;
}

void DistributedDevice::addData(const QString& sensorName, const QString &indicatorName, const time_t t, const QVariant data)
{
    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
            it.addData(indicatorName, t, data);
    }
}

QVariant DistributedDevice::getData(const QString& sensorName, const QString &indicatorName, const time_t t) const
{
    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
            return it.getData(indicatorName, t);
    }

    return QVariant();
}

QMap<time_t, QVariant>& DistributedDevice::getAllData(const QString& sensorName, const QString &indicatorName)
{
    static QMap<time_t, QVariant> dummy;

    for (auto &it : m_sensors)
    {
        if (it.getName() == sensorName)
            return it.getAllData(indicatorName);
    }

    return dummy;
}

void DistributedDevice::setAlive(bool isAlive)
{
    m_alive = isAlive;
}

bool DistributedDevice::getAlive() const
{
    return m_alive;
}
