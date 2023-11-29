#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QObject>
#include <array>
#include <set>

#include "Config.h"
#include "DataProcessingManager/DataProcessingManager.h"
#include "DSCSHandlers/dscshandlers.h"
#include "SensorDevices/sensorsmanager.h"

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

    void send(const char* data, const int size);
    void setSensorManager(SensorsManager *manager);

    ~TCPServer();

public slots:
    void processNewConnection();
    void comunicateWithRoot();

private:
    QTcpServer m_tcpServer;
    QMap<int, QTcpSocket*> m_clientSockets;

    std::set<std::array<uint8_t, MAC_ADDRESS_LENGTH>> m_clientAddress;

    DataProcessingManager<std::string, PacketHandlerFunc> m_handlers;
    SensorsManager *m_sensorManager = nullptr;
};

#endif // TCPSERVER_H
