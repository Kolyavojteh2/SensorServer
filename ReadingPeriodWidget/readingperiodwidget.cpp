#include "readingperiodwidget.h"
#include "ui_readingperiodwidget.h"

#include <iostream>

ReadingPeriodWidget::ReadingPeriodWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadingPeriodWidget)
{
    ui->setupUi(this);

    connect(ui->p_buttonBox, SIGNAL(accepted()), this, SLOT(acceptSettings()));

    connect(ui->p_buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

ReadingPeriodWidget::~ReadingPeriodWidget()
{
    delete ui;
}

void ReadingPeriodWidget::acceptSettings()
{
    bool isCyclic = ui->p_checkIsCyclick->isChecked();
    unsigned int period = ui->p_spinBoxPeriod->value();

    emit acceptedSettings(isCyclic, period);

    this->close();
}
