#include "sensorstreemodel.h"

#define HEADER_SENSOR_STR "Device/Sensor"
#define HEADER_VALUE_STR "Value"
#define HEADER_UNIT_OF_MEASURING_STR "Unit of measuring"

SensorsTreeModel::SensorsTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_headers.append(HEADER_SENSOR_STR);
    m_headers.append(HEADER_VALUE_STR);
    m_headers.append(HEADER_UNIT_OF_MEASURING_STR);

    rootItem = new TreeItem(m_headers);
}

SensorsTreeModel::~SensorsTreeModel()
{
    delete rootItem;
}

QVariant SensorsTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0 || section >= m_headers.size())
        return QVariant();

    if (orientation != Qt::Orientation::Horizontal)
        return QVariant();

    if (role != Qt::ItemDataRole::DisplayRole)
        return QVariant();

    return m_headers[section];
}

TreeItem *SensorsTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

int SensorsTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const TreeItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

int SensorsTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

Qt::ItemFlags SensorsTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    return QAbstractItemModel::flags(index);
}

QModelIndex SensorsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex SensorsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool SensorsTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);
    if (result) {
        emit dataChanged(index, index);
    }
    return result;
}

QVariant SensorsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

bool SensorsTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool result;

    beginInsertRows(parent, row, row + count - 1);
    result = parentItem->insertChildren(row, count, rootItem->columnCount());
    endInsertRows();

    return result;
}

bool SensorsTreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);

    return true;
}

bool SensorsTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool result;

    beginRemoveRows(parent, row, row + count - 1);
    result = parentItem->removeChildren(row, count);
    endRemoveRows();

    return result;
}

bool SensorsTreeModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(count);
    Q_UNUSED(parent);

    return true;
}

TreeItem *SensorsTreeModel::findChildByName(TreeItem *root, const QString& name) const
{
    TreeItem *node = nullptr;
    for (int i = 0; i < root->childCount(); ++i)
    {
        TreeItem *candidate = root->child(i);
        if (candidate && candidate->data(0) == name)
        {
            node = candidate;
            break;
        }
    }

    return node;
}

// Devices
bool SensorsTreeModel::addDevice(const QString &name)
{
    if (!rootItem->insertChildren(rootItem->childCount(), 1, 3))
        return false;

    TreeItem *device = rootItem->child(rootItem->childCount() - 1);
    device->setData(0, name);

    emit layoutChanged();

    return true;
}

TreeItem *SensorsTreeModel::getDevice(const QString &name) const
{
    return findChildByName(rootItem, name);
}

bool SensorsTreeModel::removeDevice(const QString &name)
{
    TreeItem *dev = getDevice(name);
    return rootItem->removeChildren(dev->childNumber(), 1);
}

// Sensors
bool SensorsTreeModel::addSensor(const QString &device, const QString& sensor)
{
    TreeItem *dev = getDevice(device);
    if (dev == nullptr)
    {
        addDevice(device);
        dev = getDevice(device);
    }

    if (!dev->insertChildren(dev->childCount(), 1, 3))
        return false;

    TreeItem *sens = dev->child(dev->childCount() - 1);
    sens->setData(0, sensor);

    emit layoutChanged();

    return true;
}

TreeItem *SensorsTreeModel::getSensor(const QString &device, const QString &sensor) const
{
    return findChildByName(getDevice(device), sensor);
}

bool SensorsTreeModel::removeSensor(const QString &device, const QString &sensor)
{
    TreeItem *dev = getDevice(device);
    TreeItem *sens = getSensor(device, sensor);

    return dev->removeChildren(sens->childNumber(), 1);
}

// Indicators
bool SensorsTreeModel::addIndicator(const QString &device, const QString &sensor, const QString& indicator,
                                    const QVariant& value, const QString& measuring)
{
    TreeItem *sens = getSensor(device, sensor);
    if (sens == nullptr)
    {
        addSensor(device, sensor);
        sens = getSensor(device, sensor);
    }

    if (!sens->insertChildren(sens->childCount(), 1, 3))
        return false;

    TreeItem *indic = sens->child(sens->childCount() - 1);
    indic->setData(0, indicator);
    indic->setData(1, value);
    indic->setData(2, measuring);

    emit layoutChanged();

    return true;
}

bool SensorsTreeModel::setIndicatorValue(const QString &device, const QString &sensor, const QString& indicator, const QVariant& value)
{
    TreeItem *indic = getIndicator(device, sensor, indicator);
    if (!indic)
        return false;

    indic->setData(1, value);

    emit layoutChanged();
    return true;
}

bool SensorsTreeModel::setIndicatorMeasuring(const QString &device, const QString &sensor, const QString& indicator, const QString& measuring)
{
    TreeItem *indic = getIndicator(device, sensor, indicator);
    if (!indic)
        return false;

    indic->setData(2, measuring);

    emit layoutChanged();
    return true;
}

TreeItem *SensorsTreeModel::getIndicator(const QString &device, const QString &sensor, const QString &indicator) const
{
    return findChildByName(getSensor(device, sensor), indicator);
}

bool SensorsTreeModel::removeIndicator(const QString &device, const QString &sensor, const QString &indicator)
{
    TreeItem *sens = getSensor(device, sensor);
    TreeItem *indic = getIndicator(device, sensor, indicator);

    return sens->removeChildren(indic->childNumber(), 1);
}

