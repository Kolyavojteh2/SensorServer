#ifndef SENSORTREEMODEL_H
#define SENSORTREEMODEL_H

#include <QAbstractItemModel>

#include "treeitem.h"

class SensorsTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SensorsTreeModel(QObject *parent = nullptr);
    ~SensorsTreeModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Find
    TreeItem *findChildByName(TreeItem *root, const QString& name) const;

    // Devices
    bool addDevice(const QString &name);
    TreeItem *getDevice(const QString &name) const;
    bool removeDevice(const QString &name);

    // Sensors
    bool addSensor(const QString &device, const QString& sensor);
    TreeItem *getSensor(const QString &device, const QString &sensor) const;
    bool removeSensor(const QString &device, const QString &sensor);

    // Indicators
    bool addIndicator(const QString &device, const QString &sensor, const QString& indicator,
                      const QVariant& value = QVariant(), const QString& measuring = QString());
    bool setIndicatorValue(const QString &device, const QString &sensor, const QString& indicator, const QVariant& value);
    bool setIndicatorMeasuring(const QString &device, const QString &sensor, const QString& indicator, const QString& measuring);

    TreeItem *getIndicator(const QString &device, const QString &sensor, const QString &indicator) const;
    bool removeIndicator(const QString &device, const QString &sensor, const QString &indicator);

private:
    TreeItem *rootItem;

    //void setupModelData(const QStringList &lines, TreeItem *parent);
    TreeItem *getItem(const QModelIndex &index) const;
    QList<QVariant> m_headers;
};

#endif // SENSORTREEMODEL_H
