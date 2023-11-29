#ifndef READINGPERIODWIDGET_H
#define READINGPERIODWIDGET_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class ReadingPeriodWidget;
}

class ReadingPeriodWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ReadingPeriodWidget(QWidget *parent = nullptr);
    ~ReadingPeriodWidget();

private:
    Ui::ReadingPeriodWidget *ui;

signals:
    void acceptedSettings(bool isCyclic, unsigned int period);

private slots:
    void acceptSettings();
};

#endif // READINGPERIODWIDGET_H
