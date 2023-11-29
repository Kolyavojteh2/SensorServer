#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include <QList>

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = nullptr);
    virtual ~TreeItem();

    virtual TreeItem *child(int row);
    virtual int childCount() const;
    virtual int columnCount() const;

    virtual QVariant data(int column) const;
    virtual bool setData(int column, const QVariant &value);

    virtual bool insertChildren(int position, int count, int columns);
    virtual bool insertColumns(int position, int columns);

    virtual bool removeChildren(int position, int count);
    virtual bool removeColumns(int position, int columns);

    virtual TreeItem *parent();
    virtual int childNumber() const;

protected:
    QList<TreeItem *> m_childItems;
    TreeItem *m_parentItem;

    QList<QVariant> m_itemData;
};
#endif // TREEITEM_H
