#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RedirectStreams/texteditredirect.h"
#include "DevicesWidget/devicesWidget.h"
#include "TCPServer/tcpserver.h"
#include "SensorsTreeModel/sensorstreemodel.h"
#include "SensorDevices/sensorsmanager.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    TextEditRedirect *m_log = nullptr;

    DevicesWidget *m_devicesWithSensors = nullptr;
    SensorsTreeModel *m_sensorsModel = nullptr;
    SensorsManager *m_sensorManager = nullptr;
    TCPServer *m_server = nullptr;

    QTimer* m_updateDataTimer = nullptr;
    QTimer* m_autoSaveTimer = nullptr;

private slots:
    void slot_updateIndicatorsData();
    void slot_sendReadingPeriod(bool isCyclic, unsigned int period);
    void slot_openReadingPeriodWidget();
    void slot_autoSaveData();
};
#endif // MAINWINDOW_H
