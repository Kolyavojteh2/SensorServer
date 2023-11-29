#include "sensorsmanager.h"
#include "Utils/utils.h"
#include "Config.h"

#include "DSS_Protocol/DSS_Protocol.h"
#include "DSS_Protocol/packets/GetSensorHeaderPacketRequest.h"
#include "DSS_Protocol/packets/ChronoUpdateResponsePacket.h"
#include "DSS_Protocol/packets/GetSensorDataPacketRequest.h"
#include "DSS_Protocol/packets/ClearSensorDataRequest.h"
#include "DSS_Protocol/packets/SetSensorReadingModeRequest.h"

#include <climits>

#include <iostream>

SensorsManager::SensorsManager(SensorsTreeModel *model, TCPServer *server) : m_model(model), m_server(server)
{
    m_server->setSensorManager(this);
}

void SensorsManager::addAliveDevice(const QString &mac)
{
    DistributedDevice& dev = getDevice(mac);
    if (dev.getMAC().isEmpty())
    {
        std::cerr << "[SensorManager] wrong device" << std::endl;
        return;
    }
    dev.setAlive(true);
}

void SensorsManager::addDevice(const QString &device)
{
    DistributedDevice dev(device);

    m_devices.append(dev);
    m_model->addDevice(device);
    addAliveDevice(device);

    std::cout << "[SensorsManager] added device: " << device.toStdString() << std::endl;
}

DistributedDevice& SensorsManager::getDevice(const QString &mac)
{
    static DistributedDevice dummy("");

    for (auto &it : m_devices)
    {
        if (it.getMAC() == mac)
            return it;
    }

    std::cerr << "Dummy error" << std::endl;
    return dummy;
}

// Sensor
void SensorsManager::addSensor(const QString &mac, const QString& sensor)
{
    DistributedDevice &dev = getDevice(mac);
    dev.addSensor(sensor);

    m_model->addSensor(mac, sensor);
}

SensorModule& SensorsManager::getSensor(const QString &mac, const QString &sensor)
{
    return getDevice(mac).getSensor(sensor);
}

// Indicator
void SensorsManager::addIndicator(const QString &mac, const QString& sensor, const QString &indicator)
{
    DistributedDevice &dev = getDevice(mac);
    dev.getSensor(sensor).addIndicator(indicator);

    m_model->addIndicator(mac, sensor, indicator);
}

SensorsIndicator& SensorsManager::getIndicator(const QString &mac, const QString &sensor, const QString &indicator)
{
    return getDevice(mac).getIndicator(sensor, indicator);
}

void SensorsManager::addData(const QString &mac, const QString& sensor, const QString &indicator,
                             const time_t t, const QVariant &value)
{
    SensorsIndicator& indic = getIndicator(mac, sensor, indicator);
    indic.addData(t, value);

    m_model->setIndicatorValue(mac, sensor, indicator, value);
}

QVariant SensorsManager::getData(const QString &mac, const QString& sensor, const QString &indicator, const time_t t)
{
    SensorsIndicator& indic = getIndicator(mac, sensor, indicator);
    return indic.getData(t);
}

QMap<time_t, QVariant> SensorsManager::getAllData(const QString &mac, const QString& sensor, const QString &indicator)
{
    SensorsIndicator& indic = getIndicator(mac, sensor, indicator);
    return indic.getAllData();
}

QList<DistributedDevice>& SensorsManager::getDevices()
{
    return m_devices;
}

void SensorsManager::updateSensorsInfo(const QString &mac)
{
    DSS_Protocol_t request(PacketType_t::GetSensorsPacketRequest);
    request.destinationMAC = utils::MAC::toVector(mac.toStdString());
    request.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    std::vector<uint8_t> bin;
    request.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());

    std::cout << "[SensorsManager] sent sensors request to " << mac.toStdString() << std::endl;
}

void SensorsManager::updateIndicatorsInfo(const QString &mac, const QString &sensor)
{
    DSS_Protocol_t request(PacketType_t::GetSensorHeaderPacketRequest);
    request.destinationMAC = utils::MAC::toVector(mac.toStdString());
    request.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    GetSensorHeaderPacketRequest_t *packet = dynamic_cast<GetSensorHeaderPacketRequest_t *>(request.packet);
    if (packet == nullptr)
    {
        std::cerr << "[SensorsManager] dynamic_cast<GetSensorHeaderPacketRequest_t *> fail." << std::endl;
        return;
    }
    packet->sensorName = sensor.toStdString();

    std::vector<uint8_t> bin;
    request.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());
    std::cout << "[SensorsManager] sent indicators request to " << mac.toStdString() << " for sensor \"" <<
            sensor.toStdString() << "\""<< std::endl;
}

void SensorsManager::sendAliveRequest(const QString &mac)
{
    DSS_Protocol_t request(PacketType_t::AliveNodeRequestPacket);
    request.destinationMAC = utils::MAC::toVector(mac.toStdString());
    request.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    std::vector<uint8_t> bin;
    request.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());

    std::cout << "[SensorsManager] sent aliveNode request to " << mac.toStdString() << std::endl;
}

void SensorsManager::sendChronoUpdate(const QString &mac)
{
    DSS_Protocol_t chronoResponse(PacketType_t::ChronoUpdateResponsePacket);
    chronoResponse.destinationMAC = utils::MAC::toVector(mac.toStdString());
    chronoResponse.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    ChronoUpdateResponsePacket_t *packet = dynamic_cast<ChronoUpdateResponsePacket_t *>(chronoResponse.packet);
    packet->newTime = time(NULL);

    std::vector<uint8_t> bin;
    chronoResponse.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());
    std::cout << "[SensorsManager] sent chronoUpdate response to " << mac.toStdString() << std::endl;
}

void SensorsManager::sendGetIndicatorsDataRequest(const QString &mac, const QString &sensor, const QString &indicator)
{
    DSS_Protocol_t getDataRequest(PacketType_t::GetSensorDataPacketRequest);
    getDataRequest.destinationMAC = utils::MAC::toVector(mac.toStdString());
    getDataRequest.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    GetSensorDataPacketRequest_t *packet = dynamic_cast<GetSensorDataPacketRequest_t *>(getDataRequest.packet);
    packet->sensorName = sensor.toStdString();
    packet->dataName = indicator.toStdString();
    packet->timeFrom = 0;
    packet->timeTo = INT_LEAST32_MAX;

    std::vector<uint8_t> bin;
    getDataRequest.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());
    std::cout << "[SensorsManager] sent getIndicatorsData request to " <<
        mac.toStdString() << ":" << sensor.toStdString() << ":" << indicator.toStdString() << std::endl;
}

void SensorsManager::clearDataOnDevice(const QString &mac, const QString &sensor, const QString &indicator, const time_t from, const time_t to)
{
    DSS_Protocol_t clearDataRequest(PacketType_t::ClearSensorDataRequest);
    clearDataRequest.destinationMAC = utils::MAC::toVector(mac.toStdString());
    clearDataRequest.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    ClearSensorDataRequest_t *packet = dynamic_cast<ClearSensorDataRequest_t *>(clearDataRequest.packet);
    packet->sensorName = sensor.toStdString();
    packet->dataName = indicator.toStdString();
    packet->timeFrom = from;
    packet->timeTo = to;

    std::vector<uint8_t> bin;
    clearDataRequest.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());
    std::cout << "[SensorsManager] sent clearData request to " <<
        mac.toStdString() << ":" << sensor.toStdString() << ":" << indicator.toStdString() <<
        ". From " << from << " to " << to << "." << std::endl;
}

void SensorsManager::setReadingPeriod(const QString &mac, bool isCyclic, const time_t period)
{
    DSS_Protocol_t setReadingPeriodRequest(PacketType_t::SetSensorReadingModeRequest);
    setReadingPeriodRequest.destinationMAC = utils::MAC::toVector(mac.toStdString());
    setReadingPeriodRequest.sourceMAC = std::vector<uint8_t>(MAC_ADDRESS_LENGTH, 0);

    SetSensorReadingModeRequest_t *packet = dynamic_cast<SetSensorReadingModeRequest_t *>(setReadingPeriodRequest.packet);
    packet->isCyclic = isCyclic;
    packet->period = period;

    std::vector<uint8_t> bin;
    setReadingPeriodRequest.toBin(bin);

    m_server->send((const char *)bin.data(), bin.size());
    std::cout << "[SensorsManager] sent setReadingPeriod request to " << mac.toStdString() << std::endl;
}
