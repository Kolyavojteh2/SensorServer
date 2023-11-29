QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CSVSaver/csvsaver.cpp \
    DevicesWidget/devicesWidget.cpp \
    DSCSHandlers/dscshandlers.cpp \
    ReadingPeriodWidget/readingperiodwidget.cpp \
    SensorDevices/distributeddevice.cpp \
    RedirectStreams/stderrredirect.cpp \
    RedirectStreams/stdoutredirect.cpp \
    SensorDevices/sensormodule.cpp \
    SensorDevices/sensorsindicator.cpp \
    SensorDevices/sensorsmanager.cpp \
    SensorsTreeModel/sensorstreemodel.cpp \
    SensorsTreeModel/treeitem.cpp \
    TCPServer/tcpserver.cpp \
    DSS_Protocol/packets/ChronoUpdateResponsePacket.cpp \
    DSS_Protocol/packets/BootstrapPacket.cpp \
    DSS_Protocol/packets/SetConfigPacket.cpp \
    DSS_Protocol/packets/GetSensorsPacketRequest.cpp \
    DSS_Protocol/packets/GetSensorHeaderPacketRequest.cpp \
    DSS_Protocol/packets/GetSensorDataPacketRequest.cpp \
    DSS_Protocol/packets/GetSensorDataPacketResponse.cpp \
    DSS_Protocol/packets/ChronoUpdateRequestPacket.cpp \
    DSS_Protocol/packets/SetSensorReadingModeRequest.cpp \
    DSS_Protocol/packets/AliveNodeResponsePacket.cpp \
    DSS_Protocol/packets/ClearSensorDataRequest.cpp \
    DSS_Protocol/packets/GetConfigPacketRequest.cpp \
    DSS_Protocol/packets/GetSensorsPacketResponse.cpp \
    DSS_Protocol/packets/GetConfigPacketResponse.cpp \
    DSS_Protocol/packets/GetSensorHeaderPacketResponse.cpp \
    DSS_Protocol/packets/AliveNodeRequestPacket.cpp \
    DSS_Protocol/DSS_Protocol.cpp \
    DSS_Protocol/utils/ConvertPacket.cpp \
    MainWindow/mainwindow.cpp \
    main.cpp \
    RedirectStreams/texteditredirect.cpp \
    Utils/utils.cpp

HEADERS += \
    CSVSaver/csvsaver.h \
    Config.h \
    DSS_Protocol/DSS_Protocol.h \
    DSS_Protocol/packets/GetConfigPacketRequest.h \
    DSS_Protocol/packets/BootstrapPacket.h \
    DSS_Protocol/packets/AliveNodeRequestPacket.h \
    DSS_Protocol/packets/GetSensorDataPacketRequest.h \
    DSS_Protocol/packets/SetSensorReadingModeRequest.h \
    DSS_Protocol/packets/ChronoUpdateRequestPacket.h \
    DSS_Protocol/packets/GetSensorsPacketRequest.h \
    DSS_Protocol/packets/ClearSensorDataRequest.h \
    DSS_Protocol/packets/SetConfigPacket.h \
    DSS_Protocol/packets/ChronoUpdateResponsePacket.h \
    DSS_Protocol/packets/GetSensorHeaderPacketRequest.h \
    DSS_Protocol/packets/AliveNodeResponsePacket.h \
    DSS_Protocol/packets/GetSensorsPacketResponse.h \
    DSS_Protocol/packets/BasePacket.h \
    DSS_Protocol/packets/GetSensorHeaderPacketResponse.h \
    DSS_Protocol/packets/GetSensorDataPacketResponse.h \
    DSS_Protocol/packets/GetConfigPacketResponse.h \
    DSS_Protocol/utils/ConvertPacket.h \
    DataProcessingManager/DataProcessingManager.h \
    DevicesWidget/devicesWidget.h \
    DSCSHandlers/dscshandlers.h \
    MainWindow/mainwindow.h \
    ReadingPeriodWidget/readingperiodwidget.h \
    SensorDevices/distributeddevice.h \
    RedirectStreams/stderrredirect.h \
    RedirectStreams/stdoutredirect.h \
    SensorDevices/sensormodule.h \
    SensorDevices/sensorsindicator.h \
    SensorDevices/sensorsmanager.h \
    SensorsTreeModel/sensorstreemodel.h \
    SensorsTreeModel/treeitem.h \
    TCPServer/tcpserver.h \
    RedirectStreams/texteditredirect.h \
    Utils/utils.h

FORMS += \
    DevicesWidget/devicesWidget.ui \
    MainWindow/mainwindow.ui \
    ReadingPeriodWidget/readingperiodwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TechnicalTaskPython
