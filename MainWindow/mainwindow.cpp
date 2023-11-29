#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLayout>
#include <iostream>
#include <QHBoxLayout>
#include <QSplitter>
#include <QDateTime>
#include <QDir>

#include "ReadingPeriodWidget/readingperiodwidget.h"
#include "CSVSaver/csvsaver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Log
    //m_log = new TextEditRedirect;
    //m_log->redirectStdErr();
    //m_log->redirectStdOut();

    //m_log->setReadOnly(true);

    // DeviceWidget
    m_devicesWithSensors = new DevicesWidget;

    // CentralWidget Layout
    QHBoxLayout *box = new QHBoxLayout;
    QSplitter *centralWidgetSplitter = new QSplitter(Qt::Orientation::Horizontal);
    centralWidgetSplitter->addWidget(m_devicesWithSensors);
    //centralWidgetSplitter->addWidget(m_log);

    box->addWidget(centralWidgetSplitter);
    this->centralWidget()->setLayout(box);

    // Devices Model
    m_sensorsModel = new SensorsTreeModel(this);
    m_devicesWithSensors->getTreeView()->setModel(m_sensorsModel);
    m_devicesWithSensors->getTreeView()->setAnimated(true);

    // Test Model data
    /*
    QString dev = "my_dev";
    QString sens = "my_sens";
    QString indic1 = "indicator_1";
    QString measuring = "mm";

    m_sensorsModel->addDevice(dev);
    m_sensorsModel->addSensor(dev, sens);
    m_sensorsModel->addIndicator(dev, sens, indic1, 13, measuring);
    m_sensorsModel->addIndicator(dev, sens, "int2", 847, measuring);
    m_sensorsModel->setIndicatorValue(dev, sens, indic1, 345);

    m_sensorsModel->addDevice("dev");
    */

    // TCP server
    m_server = new TCPServer;

    // Sensor manager
    m_sensorManager = new SensorsManager(m_sensorsModel, m_server);
    connect(m_devicesWithSensors, SIGNAL(clickedUpdateData()), this, SLOT(slot_updateIndicatorsData()));

    // Reading period
    connect(m_devicesWithSensors, SIGNAL(clickedChangeSettings()), this, SLOT(slot_openReadingPeriodWidget()));

    // Timer update indicators data
    m_updateDataTimer = new QTimer;
    m_updateDataTimer->setInterval(5000);
    m_updateDataTimer->start();
    connect(m_updateDataTimer, SIGNAL(timeout()), this, SLOT(slot_updateIndicatorsData()));

    // Auto save timer
    m_autoSaveTimer = new QTimer;
    m_autoSaveTimer->setInterval(60 * 1000);
    m_autoSaveTimer->start();
    connect(m_autoSaveTimer, SIGNAL(timeout()), this, SLOT(slot_autoSaveData()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if (m_server)
        delete m_server;
    if (m_log)
        delete m_log;
    if (m_devicesWithSensors)
        delete m_devicesWithSensors;
    if (m_sensorsModel)
        delete m_sensorsModel;
    if (m_updateDataTimer)
        delete m_updateDataTimer;
}

void MainWindow::slot_autoSaveData()
{
    QDateTime currentTime = QDateTime::currentDateTime();

    auto &devices = m_sensorManager->getDevices();
    for (auto &dev : devices)
    {
        QString deviceMAC = dev.getMAC();

        auto &sensors = dev.getSensors();
        for (auto &sens : sensors)
        {
            QString sensorName = sens.getName();

            auto &indicators = sens.getIndicators();
            for (auto &indic : indicators)
            {
                QString indicatorName = indic.getName();

                QString dataDirPath(deviceMAC + QString(QDir::separator()) + sensorName + QString(QDir::separator()) + indicatorName);
                QDir dirFile(dataDirPath);
                dirFile.mkpath(".");

                QString filename;
                filename.append(dataDirPath + QString(QDir::separator()));
                filename.append(currentTime.toString(Qt::DateFormat::ISODate) + ".csv");

                QList<QString> title;
                title.append("time_t");
                title.append(indic.getName());

                CSVSaver::saveToFile(filename, indic.getAllData(), title);
            }
        }
    }
}

void MainWindow::slot_updateIndicatorsData()
{
    std::cout << "UpdateIndicatorData" << std::endl;

    auto &devices = m_sensorManager->getDevices();
    for (auto &dev : devices)
    {
        auto &sensors = dev.getSensors();
        for (auto &sens : sensors)
        {
            auto &indicators = sens.getIndicators();
            for (auto &indic : indicators)
                m_sensorManager->sendGetIndicatorsDataRequest(dev.getMAC(), sens.getName(), indic.getName());
        }
    }
}

void MainWindow::slot_sendReadingPeriod(bool isCyclic, unsigned int period)
{
    auto &devices = m_sensorManager->getDevices();
    for (auto &dev : devices)
        m_sensorManager->setReadingPeriod(dev.getMAC(), isCyclic, period);
}

void MainWindow::slot_openReadingPeriodWidget()
{
    ReadingPeriodWidget *dialog = new ReadingPeriodWidget;
    dialog->setWindowModality(Qt::WindowModality::WindowModal);
    connect(dialog, SIGNAL(acceptedSettings(bool,uint)), this, SLOT(slot_sendReadingPeriod(bool,uint)));

    this->setEnabled(false);
    dialog->exec();
    this->setEnabled(true);
}
