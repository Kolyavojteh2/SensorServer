#ifndef SENSORSMANAGER_H
#define SENSORSMANAGER_H

#include <QList>

#include "distributeddevice.h"
#include "SensorsTreeModel/sensorstreemodel.h"
#include "TCPServer/tcpserver.h"

class SensorsManager
{
public:
    SensorsManager(SensorsTreeModel *model, TCPServer *server);

    // Device
    void addAliveDevice(const QString &mac);
    void addDevice(const QString &mac);
    DistributedDevice& getDevice(const QString &mac);
    QList<DistributedDevice>& getDevices();

    // Sensor
    void addSensor(const QString &mac, const QString& sensor);
    SensorModule& getSensor(const QString &mac, const QString &sensor);

    // Indicator
    void addIndicator(const QString &mac, const QString& sensor, const QString &indicator);
    SensorsIndicator& getIndicator(const QString &mac, const QString &sensor, const QString &indicator);

    // Data
    void addData(const QString &mac, const QString& sensor, const QString &indicator, const time_t t, const QVariant &value);
    QVariant getData(const QString &mac, const QString& sensor, const QString &indicator, const time_t t);
    QMap<time_t, QVariant> getAllData(const QString &mac, const QString& sensor, const QString &indicator);


    // Get device info
    void updateSensorsInfo(const QString &mac);
    void updateIndicatorsInfo(const QString &mac, const QString &sensor);

    // AliveNode
    void sendAliveRequest(const QString &mac);

    // ChronoUpdate
    void sendChronoUpdate(const QString &mac);

    // DataExchange
    void sendGetIndicatorsDataRequest(const QString &mac, const QString &sensor, const QString &indicator);
    void clearDataOnDevice(const QString &mac, const QString &sensor, const QString &indicator, const time_t from, const time_t to);

    // Send Reading period
    void setReadingPeriod(const QString &mac, bool isCyclic, const time_t period);

private:
    QList<DistributedDevice> m_devices;

    SensorsTreeModel *m_model;
    TCPServer *m_server;

};

#endif // SENSORSMANAGER_H
