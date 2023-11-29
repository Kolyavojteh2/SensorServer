#include "sensormodule.h"

SensorModule::SensorModule(const QString& name) : m_name(name) {}

QString SensorModule::getName() const
{
    return m_name;
}

void SensorModule::setName(const QString& name)
{
    m_name = name;
}

void SensorModule::addIndicator(const QString &indicatorName,
                  const QMap<time_t, QVariant>& data)
{
    m_indicators.push_back(SensorsIndicator(indicatorName, data));
}

SensorsIndicator& SensorModule::getIndicator(const QString &indicatorName)
{
    static SensorsIndicator dummy("");

    for (int i = 0; i < m_indicators.size(); ++i)
    {
        if (m_indicators[i].getName() == indicatorName)
            return m_indicators[i];
    }

    return dummy;
}

QVector<SensorsIndicator>& SensorModule::getIndicators()
{
    return m_indicators;
}

void SensorModule::addData(const QString &indicatorName, const time_t t, const QVariant data)
{
    for (int i = 0; i < m_indicators.size(); ++i)
    {
        if (m_indicators[i].getName() == indicatorName)
        {
            m_indicators[i].addData(t, data);
            return;
        }
    }
}

QVariant SensorModule::getData(const QString &indicatorName, const time_t t) const
{
    for (int i = 0; i < m_indicators.size(); ++i)
    {
        if (m_indicators[i].getName() == indicatorName)
            return m_indicators[i].getData(t);
    }

    return QVariant();
}

QMap<time_t, QVariant>& SensorModule::getAllData(const QString &indicatorName)
{
    static QMap<time_t, QVariant> dummy;

    for (int i = 0; i < m_indicators.size(); ++i)
    {
        if (m_indicators[i].getName() == indicatorName)
            return m_indicators[i].getAllData();
    }

    return dummy;
}

