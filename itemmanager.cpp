#include "itemmanager.h"
#include <QMessageBox>

#if defined(Q_OS_MAC)
# include <sys/time.h> // utimes
#elif defined(Q_OS_WIN32)
# include <windows.h>
#else
#error "not impliment1"
#endif

// 登録されたファイルを管理

static const QString DATETIME_FORMAT = "yyyy-MM-dd hh:mm";

ItemManager::ItemManager(QObject *parent)
    : QAbstractItemModel(parent)
{
}

// QAbstractItemModel interface

QModelIndex ItemManager::index(int row, int column, const QModelIndex & /*parent*/ /*= QModelIndex()*/) const
{
    return createIndex(row, column);
}

QModelIndex ItemManager::parent(const QModelIndex & /*child*/) const
{
    return QModelIndex();
}

int ItemManager::rowCount(const QModelIndex & /*parent*/ /*= QModelIndex()*/) const
{
    return m_itemList.count();
}

int ItemManager::columnCount(const QModelIndex & /*parent*/ /*= QModelIndex()*/) const
{
    return ColumnNum;
}

QVariant ItemManager::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    const ItemInfoType& info = m_itemList.at(index.row());

    switch (index.column())
    {
    case ColumnFileName:
        if (Qt::DisplayRole == role) {
            return QVariant(QFileInfo(info.filePath).fileName());
        }
        break;
    case ColumnState:
        if (Qt::DisplayRole == role) {
            return QVariant(
                        info.timestampOfCreated  != info.timestampOfCreatedCurrent  ||
                        info.timestampOfModified != info.timestampOfModifiedCurrent ||
                        info.timestampOfRead     != info.timestampOfReadCurrent
                    ? tr("Modified") : tr("Normal"));
        }
        break;
    case ColumnTimestampOfCreated:
        if (Qt::DisplayRole == role) {
            return QVariant(info.timestampOfModified.toString(DATETIME_FORMAT));
        }
        break;
    case ColumntimestampOfModified:
        if (Qt::DisplayRole == role) {
            return QVariant(info.timestampOfModified.toString(DATETIME_FORMAT));
        }
        break;
    case ColumntimestampOfRead:
        if (Qt::DisplayRole == role) {
            return QVariant(info.timestampOfRead.toString(DATETIME_FORMAT));
        }
        break;
    }

    return QVariant();
}

QVariant ItemManager::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (Qt::DisplayRole == role)
    {
        switch (section)
        {
        case ColumnFileName:            return tr("File name");
        case ColumnState:               return tr("State");
        case ColumnTimestampOfCreated:  return tr("Created");
        case ColumntimestampOfModified: return tr("Last modified");
        case ColumntimestampOfRead:     return tr("Last accessed");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
// 実装

bool ItemManager::append(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
    {
        return false;
    }

    ItemInfoType info;
    info.filePath            = fileInfo.canonicalFilePath();
    info.timestampOfCreated  = fileInfo.created();
    info.timestampOfModified = fileInfo.lastModified();
    info.timestampOfRead     = fileInfo.lastRead();

    updateTimestamp(info);

    beginInsertRows(QModelIndex(), m_itemList.count(), m_itemList.count());

    m_itemList.append(info);

    endInsertRows();

    return true;
}

bool ItemManager::remove(const QString& filePath)
{
    return false;
}

bool ItemManager::remove(const QModelIndex& index)
{
    resetTimestamp(m_itemList[index.row()]);

    beginRemoveRows(QModelIndex(), index.row(), index.row());

    m_itemList.removeAt(index.row());

    endRemoveRows();

    return true;
}

bool ItemManager::clear()
{
    for (QList<ItemInfoType>::iterator ite = m_itemList.begin();
         ite != m_itemList.end(); ++ite)
    {
        resetTimestamp(*ite);
    }

    beginRemoveRows(QModelIndex(), 0, m_itemList.count() - 1);

    m_itemList.clear();

    endRemoveRows();

    return true;
}

bool ItemManager::empty() const
{
    return m_itemList.empty();
}

void ItemManager::updateTimestamp(ItemInfoType &info)
{
    QFileInfo fileInfo(info.filePath);
    info.timestampOfCreatedCurrent  = fileInfo.created();
    info.timestampOfModifiedCurrent = fileInfo.lastModified();
    info.timestampOfReadCurrent     = fileInfo.lastRead();
}

bool ItemManager::resetTimestamp(const ItemInfoType &info)
{
#if defined(Q_OS_MAC)
    struct timeval times[2];
    // アクセス日時
    times[0].tv_sec = info.timestampOfRead.toTime_t();
    times[0].tv_usec= info.timestampOfRead.toMSecsSinceEpoch() % 100 * 1000;
    // 更新日時
    times[1].tv_sec = info.timestampOfModified.toTime_t();
    times[1].tv_usec= info.timestampOfModified.toMSecsSinceEpoch() % 100 * 1000;
    // 反映(アクセス日時は反映できないor反映してもすぐ変わる模様)
    return !::utimes(info.filePath.toLocal8Bit(), times);
#elif defined(Q_OS_WIN32)
    QVector<wchar_t> fileNameW(info.filePath.size() + 1);
    info.filePath.toWCharArray(&fileNameW[0]);
    HANDLE hFile = ::CreateFile(
                        &fileNameW[0],
                        GENERIC_WRITE, 0,
                        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        BOOL r;
        // time_t の基準日から FILETIME への変更
        LONGLONG offsetOfTimeT = 0;
        SYSTEMTIME stBase = { 1970, 1, 0, 1, 0, 0, 0, 0 };
        ::SystemTimeToFileTime(&stBase, (FILETIME*)&offsetOfTimeT);
        // タイムスタンプを FILETIME の基準日からに変更し適用
        LONGLONG nCreationTime = 0, nLastAccessTime = 0, nLastWriteTime = 0;
        nCreationTime   = offsetOfTimeT + info.timestampOfCreated.toMSecsSinceEpoch()  * 10000LL;
        nLastAccessTime = offsetOfTimeT + info.timestampOfRead.toMSecsSinceEpoch()     * 10000LL;
        nLastWriteTime  = offsetOfTimeT + info.timestampOfModified.toMSecsSinceEpoch() * 10000LL;
        r = ::SetFileTime(hFile, (const FILETIME*)&nCreationTime, (const FILETIME*)&nLastAccessTime,
                                 (const FILETIME*)&nLastWriteTime);
        ::CloseHandle(hFile);
        return FALSE != r;
    }
    return false;
#else
#error "not impliment1"
#endif
}
