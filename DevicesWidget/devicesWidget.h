#ifndef DEVICESWIDGET_H
#define DEVICESWIDGET_H

#include <QWidget>
#include <QTreeView>

namespace Ui {
class DevicesWidget;
}

class DevicesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DevicesWidget(QWidget *parent = nullptr);
    ~DevicesWidget();

    QTreeView *getTreeView() const;

private:
    Ui::DevicesWidget *ui;

signals:
    void clickedUpdateNodesData();
    void clickedCheckAlive();
    void clickedUpdateData();
    void clickedChangeSettings();
};

#endif // DEVICESWIDGET_H
