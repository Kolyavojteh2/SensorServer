#include "tcpserver.h"
#include "DSCSHandlers/dscshandlers.h"

#include "DSS_Protocol/packets/BootstrapPacket.h"
#include "DSS_Protocol/packets/GetSensorsPacketResponse.h"
#include "DSS_Protocol/packets/GetSensorHeaderPacketResponse.h"
#include "DSS_Protocol/packets/ChronoUpdateRequestPacket.h"
#include "DSS_Protocol/packets/AliveNodeResponsePacket.h"
#include "DSS_Protocol/packets/GetSensorDataPacketResponse.h"

#include <iomanip>

TCPServer::TCPServer(QObject *parent)
    : QObject{parent}
{
    connect(&m_tcpServer, SIGNAL(newConnection()), this, SLOT(processNewConnection()));

    // Listen new connections
    int ret = m_tcpServer.listen(QHostAddress::AnyIPv4, TCP_SERVER_DEFAULT_PORT);
    if (ret == false)
    {
        // TODO: add error handling
        return;
    }

    m_handlers.add(typeid(BootstrapPacket_t).name(), DSCSHandlers::bootstrapHandler);
    m_handlers.add(typeid(GetSensorsPacketResponse_t).name(), DSCSHandlers::getSensorsResponseHandler);
    m_handlers.add(typeid(GetSensorHeaderPacketResponse_t).name(), DSCSHandlers::getIndicatorsResponseHandler);
    m_handlers.add(typeid(ChronoUpdateRequestPacket_t).name(), DSCSHandlers::chronoUpdateRequestHandler);
    m_handlers.add(typeid(AliveNodeResponsePacket_t).name(), DSCSHandlers::aliveResponseHandler);
    m_handlers.add(typeid(GetSensorDataPacketResponse_t).name(), DSCSHandlers::getIndicatorDataResponseHandler);
}

void TCPServer::setSensorManager(SensorsManager *manager)
{
    m_sensorManager = manager;
}

TCPServer::~TCPServer()
{
    for (auto &it : m_clientSockets)
        it->close();
}

void TCPServer::processNewConnection()
{
    QTcpSocket* clientSocket = m_tcpServer.nextPendingConnection();
    int id = clientSocket->socketDescriptor();
    m_clientSockets[id] = clientSocket;

    QObject::connect(m_clientSockets[id], SIGNAL(readyRead()), this, SLOT(comunicateWithRoot()));

    QHostAddress clientAddress = clientSocket->peerAddress();
    quint16 clientPort = clientSocket->peerPort();

    std::cout << "The connection with " << clientAddress.toString().toStdString() << ":" << clientPort
              << " has been established." << std::endl;
}

void TCPServer::comunicateWithRoot()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    QByteArray byteArray = clientSocket->readAll();
    std::vector<uint8_t> bin;
    std::copy(byteArray.begin(), byteArray.end(), std::back_insert_iterator(bin));

    HandlerArguments_t handlerArgs;
    handlerArgs.server = this;
    handlerArgs.binInput = &bin;
    handlerArgs.manager = m_sensorManager;

    m_handlers.runAll(&handlerArgs);
}

void TCPServer::send(const char* data, const int size)
{
    for (auto &it : m_clientSockets)
    {
        if (!it->isOpen())
            continue;

        int sentBytes = it->write(data, size);
        if (sentBytes != size)
        {
            if (sentBytes < 0)
            {
                std::cerr << "[TCPServer] sent error." << std::endl;
                it->close();
                continue;
            }

            std::cerr << "[TCPServer] sent " << sentBytes << " from " << size << "." << std::endl;
        }
    }
}
