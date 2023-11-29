#include "sensorsindicator.h"

SensorsIndicator::SensorsIndicator(const QString &name, const QMap<time_t, QVariant>& data) : m_name(name), m_data(data) {}

QString SensorsIndicator::getName() const
{
    return m_name;
}

void SensorsIndicator::setName(const QString& name)
{
    m_name = name;
}

void SensorsIndicator::addData(const time_t t, const QVariant data)
{
    m_data[t] = data;
}

QVariant SensorsIndicator::getData(const time_t t) const
{
    if (m_data.find(t) == m_data.end())
        return QVariant();

    return m_data[t];
}

QMap<time_t, QVariant>& SensorsIndicator::getAllData()
{
    return m_data;
}
