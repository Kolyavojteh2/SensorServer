#ifndef SENSORSINDICATOR_H
#define SENSORSINDICATOR_H

#include <QString>
#include <QMap>
#include <QVariant>

class SensorsIndicator
{
public:
    SensorsIndicator(const QString &name, const QMap<time_t, QVariant>& data = QMap<time_t, QVariant>());

    QString getName() const;
    void setName(const QString& name);

    void addData(const time_t t, const QVariant data);
    QVariant getData(const time_t t) const;

    QMap<time_t, QVariant>& getAllData();

private:
    QString m_name;
    QMap<time_t, QVariant> m_data;
};

#endif // SENSORSINDICATOR_H
