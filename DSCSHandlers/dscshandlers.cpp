#include "dscshandlers.h"
#include "DSS_Protocol/DSS_Protocol.h"
#include "SensorDevices/sensorsmanager.h"
#include "Utils/utils.h"
#include "DSS_Protocol/packets/BootstrapPacket.h"
#include "DSS_Protocol/packets/GetSensorsPacketResponse.h"
#include "DSS_Protocol/packets/GetSensorHeaderPacketResponse.h"
#include "DSS_Protocol/packets/ChronoUpdateRequestPacket.h"
#include "DSS_Protocol/packets/AliveNodeResponsePacket.h"
#include "DSS_Protocol/packets/GetSensorDataPacketResponse.h"

#include "SensorDevices/distributeddevice.h"

void DSCSHandlers::bootstrapHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t bootstrap(*bin);
    if (bootstrap.type != PacketType_t::BootstrapPacket)
        return;

    QString dev(QString().fromStdString(utils::MAC::toString(bootstrap.sourceMAC)));

    manager->addDevice(dev);
    manager->addAliveDevice(dev);

    std::cout << "[bootstrap] MAC=" << dev.toStdString() << std::endl;

    bin->erase(bin->begin(), bin->begin() + bootstrap.getPacketSize());

    manager->sendChronoUpdate(dev);
    manager->updateSensorsInfo(dev);
}

void DSCSHandlers::getSensorsResponseHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t sensorsResponse(*bin);
    if (sensorsResponse.type != PacketType_t::GetSensorsPacketResponse)
        return;

    GetSensorsPacketResponse_t *packet = dynamic_cast<GetSensorsPacketResponse_t *>(sensorsResponse.packet);
    if (packet == nullptr)
    {
        std::cerr << "[sensorsResponse] dynamic_cast<GetSensorsPacketResponse_t *> fail." << std::endl;
        return;
    }

    QString mac = QString().fromStdString(utils::MAC::toString(sensorsResponse.sourceMAC));

    for (auto &it : packet->sensors)
    {
        QString sensor = QString().fromStdString(it);

        manager->addSensor(mac, sensor);
        manager->updateIndicatorsInfo(mac, sensor);
    }

    std::cout << "[sensorsResponse] updated Sensors for " << mac.toStdString() << std::endl;

    bin->erase(bin->begin(), bin->begin() + sensorsResponse.getPacketSize());
}

void DSCSHandlers::getIndicatorsResponseHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t indicatorsResponse(*bin);
    if (indicatorsResponse.type != PacketType_t::GetSensorHeaderPacketResponse)
        return;

    GetSensorHeaderPacketResponse_t *packet = dynamic_cast<GetSensorHeaderPacketResponse_t *>(indicatorsResponse.packet);
    if (packet == nullptr)
    {
        std::cerr << "[indicatorsResponse] dynamic_cast<GetSensorHeaderPacketResponse_t *> fail." << std::endl;
        return;
    }

    QString mac = QString().fromStdString(utils::MAC::toString(indicatorsResponse.sourceMAC));
    QString sensor = QString().fromStdString(packet->sensorName);

    for (auto &it : packet->sensorDataNames)
    {
        QString indicator = QString().fromStdString(it);
        manager->addIndicator(mac, sensor, indicator);
    }

    std::cout << "[indicatorsResponse] updated Indicators for "
              << mac.toStdString() << ":" << sensor.toStdString() << std::endl;

    bin->erase(bin->begin(), bin->begin() + indicatorsResponse.getPacketSize());
}

void DSCSHandlers::chronoUpdateRequestHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t chronoRequest(*bin);
    if (chronoRequest.type != PacketType_t::ChronoUpdateRequestPacket)
        return;

    QString mac = QString().fromStdString(utils::MAC::toString(chronoRequest.sourceMAC));
    manager->sendChronoUpdate(mac);

    bin->erase(bin->begin(), bin->begin() + chronoRequest.getPacketSize());
    std::cout << "[chronoUpdateRequest] updated device time for " << mac.toStdString() << std::endl;
}

void DSCSHandlers::aliveResponseHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t aliveResponse(*bin);
    if (aliveResponse.type != PacketType_t::AliveNodeResponsePacket)
        return;

    QString mac = QString().fromStdString(utils::MAC::toString(aliveResponse.sourceMAC));
    manager->addAliveDevice(mac);

    bin->erase(bin->begin(), bin->begin() + aliveResponse.getPacketSize());
    std::cout << "[aliveNodeResponse] device " << mac.toStdString() << " is alive."<< std::endl;
}

static void byte2byte(std::vector<uint8_t> &inputBin, std::vector<QVariant> &outputBin)
{
    outputBin.reserve(inputBin.size());
    for (auto it : inputBin)
        outputBin.push_back(it);
}

static void byte2uint16_t(std::vector<uint8_t> &inputBin, std::vector<QVariant> &outputBin)
{
    outputBin.reserve(inputBin.size() / 2);
    for (std::size_t i = 0; i < inputBin.size(); i += 2)
    {
        uint16_t eachValue = inputBin[i];  // LSB 0x00
        eachValue |= inputBin[i + 1] << 8; // MSB 0x01

        outputBin.push_back(eachValue);
    }
}

static void byte2uint32_t(std::vector<uint8_t> &inputBin, std::vector<QVariant> &outputBin)
{
    outputBin.reserve(inputBin.size() / 4);
    for (std::size_t i = 0; i < inputBin.size(); i += 4)
    {
        uint32_t eachValue = inputBin[i];   // LSB 0x00
        eachValue |= inputBin[i + 1] << 8;  //     0x01
        eachValue |= inputBin[i + 2] << 16; //     0x02
        eachValue |= inputBin[i + 3] << 24; // MSB 0x03

        outputBin.push_back(eachValue);
    }
}

static void byte2double(std::vector<uint8_t> &inputBin, std::vector<QVariant> &outputBin)
{
    outputBin.reserve(inputBin.size() / 8);
    for (std::size_t i = 0; i < inputBin.size(); i += 8)
    {
        // Convert
        uint32_t doubleArray[2];
        doubleArray[0] |= inputBin[i];           // LSB 0x00
        doubleArray[0] |= inputBin[i + 1] << 8;  //     0x01
        doubleArray[0] |= inputBin[i + 2] << 16; //     0x02
        doubleArray[0] |= inputBin[i + 3] << 24; //     0x03
        doubleArray[1] |= inputBin[i];           //     0x04
        doubleArray[1] |= inputBin[i + 1] << 8;  //     0x05
        doubleArray[1] |= inputBin[i + 2] << 16; //     0x06
        doubleArray[1] |= inputBin[i + 3] << 24; // MSB 0x07

        outputBin.push_back((double)*((double*)(&doubleArray)));
    }
}

static void convertByteVectorToQVariantVector(std::vector<uint8_t> &inputBin, const uint8_t singleDataSize, std::vector<QVariant> &outputBin)
{
    switch (singleDataSize) {
    case 1:
        byte2byte(inputBin, outputBin);
        break;

    case 2:
        byte2uint16_t(inputBin, outputBin);
        break;

    case 4:
        byte2uint32_t(inputBin, outputBin);
        break;

    case 8:
        byte2double(inputBin, outputBin);
        break;
    default:
        std::cerr << "Wrong data size" << std::endl;
        break;
    }
}

void DSCSHandlers::getIndicatorDataResponseHandler(void *arg)
{
    HandlerArguments_t *args = (HandlerArguments_t*)arg;
    std::vector<uint8_t> *bin = args->binInput;
    SensorsManager *manager = args->manager;

    DSS_Protocol_t getIndicatorDataResponse(*bin);
    if (getIndicatorDataResponse.type != PacketType_t::GetSensorDataPacketResponse)
        return;

    GetSensorDataPacketResponse_t *packet = dynamic_cast<GetSensorDataPacketResponse_t *>(getIndicatorDataResponse.packet);
    if (packet == nullptr)
    {
        std::cerr << "[indicatorsResponse] dynamic_cast<GetSensorDataPacketResponse_t *> fail." << std::endl;
        return;
    }

    QString mac = QString().fromStdString(utils::MAC::toString(getIndicatorDataResponse.sourceMAC));
    QString sensor = QString().fromStdString(packet->sensorName);
    QString indicator = QString().fromStdString(packet->dataName);

    std::vector<QVariant> data;
    convertByteVectorToQVariantVector(packet->dataValue, packet->sizeData, data);
    std::vector<time_t> &t = packet->dataTime;

    for (std::size_t i = 0; i < t.size(); ++i)
        manager->addData(mac, sensor, indicator, t[i], data[i]);

    bin->erase(bin->begin(), bin->begin() + getIndicatorDataResponse.getPacketSize());
    if (t.empty())
        return;

    time_t minTime = *(t.begin());
    time_t maxTime = *(t.rbegin());

    manager->clearDataOnDevice(mac, sensor, indicator, minTime, maxTime);
}
