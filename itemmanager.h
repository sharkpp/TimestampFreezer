#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class ItemManager
    : public QAbstractItemModel
{
    Q_OBJECT

    enum {
        ColumnFileName = 0,
        ColumnState,
        ColumnTimestampOfCreated,
        ColumntimestampOfModified,
        ColumntimestampOfRead,
        ColumnNum
    } ColumnTypeEnum;

    typedef struct ItemInfoType {
        QString filePath;
        QDateTime timestampOfCreated;
        QDateTime timestampOfModified;
        QDateTime timestampOfRead;
        QDateTime timestampOfCreatedCurrent;
        QDateTime timestampOfModifiedCurrent;
        QDateTime timestampOfReadCurrent;
    } ItemInfoType;

public:
    explicit ItemManager(QObject *parent = 0);
    
    bool append(const QString& filePath);

    bool remove(const QString& filePath);
    bool remove(const QModelIndex &index);

    bool clear();

 //   bool updatew();

protected:

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    
public slots:

private:

    void updateTimestamp(ItemInfoType &info);

    bool resetTimestamp(const ItemInfoType &info);

private:

    QList<ItemInfoType> m_itemList;
};

#endif // ITEMMANAGER_H
