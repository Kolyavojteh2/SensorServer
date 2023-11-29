#ifndef SENSORMODULE_H
#define SENSORMODULE_H

#include <QString>
#include <QVector>
#include <QVariant>

#include "sensorsindicator.h"

class SensorModule
{
public:
    SensorModule(const QString& name);

    QString getName() const;
    void setName(const QString& name);

    void addIndicator(const QString &indicatorName,
                      const QMap<time_t, QVariant>& data = QMap<time_t, QVariant>());
    SensorsIndicator& getIndicator(const QString &indicatorName);
    QVector<SensorsIndicator>& getIndicators();

    void addData(const QString &indicatorName, const time_t t, const QVariant data);
    QVariant getData(const QString &indicatorName, const time_t t) const;
    QMap<time_t, QVariant>& getAllData(const QString &indicatorName);

private:
    QString m_name;
    QVector<SensorsIndicator> m_indicators;
};

#endif // SENSORMODULE_H
