#include "devicesWidget.h"
#include "ui_devicesWidget.h"

DevicesWidget::DevicesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevicesWidget)
{
    ui->setupUi(this);

    // CheckAlive and UpdateNodesData signals
    connect(ui->p_buttonCheckAlive, SIGNAL(clicked()), this, SIGNAL(clickedCheckAlive()));
    connect(ui->p_buttonUpdateNodesData, SIGNAL(clicked()), this, SIGNAL(clickedUpdateNodesData()));

    // Expand and Collapse tree
    connect(ui->p_buttonCollapseAll, SIGNAL(clicked()), ui->p_treeViewAliveNodes, SLOT(collapseAll()));
    connect(ui->p_buttonExpandAll, SIGNAL(clicked()), ui->p_treeViewAliveNodes, SLOT(expandAll()));

    // Update data signal
    connect(ui->p_buttonUpdateData, SIGNAL(clicked()), this, SIGNAL(clickedUpdateData()));

    // Reading period
    connect(ui->p_buttonChangeReadingPeriod, SIGNAL(clicked()), this, SIGNAL(clickedChangeSettings()));
}

DevicesWidget::~DevicesWidget()
{
    delete ui;
}

QTreeView *DevicesWidget::getTreeView() const
{
    return ui->p_treeViewAliveNodes;
}
