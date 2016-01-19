#include "DevicesInfoList.h"
#include "DeviceHandle.h"

struct UIDMatch {
    UIDMatch(const QString& s1)
    : deviceUID(s1) {}

    bool operator()(const DeviceHandle& x) const
    {
        return  x.getUID() == deviceUID;
    }
    QString deviceUID;
};

/*******************************************/
/*/////////////DevicesInfoList/////////////*/
/*******************************************/
DevicesInfoList::DevicesInfoList(QObject* pParent)
: QAbstractListModel(pParent)
{
}

int DevicesInfoList::rowCount(const QModelIndex &parent) const
{
    return devicesList.count();
}

QVariant DevicesInfoList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= devicesList.size())
        return QVariant();

    switch (role) {
        case NameRole: // The default display role now displays the first name as well
            return devicesList.at(index.row()).getName();
        case UIDRole:
            return devicesList.at(index.row()).getUID();
        case IPRole:
            return devicesList.at(index.row()).getIP();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> DevicesInfoList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UIDRole]  = "uid";
    roles[NameRole] = "name";
    roles[IPRole]   = "ip";
    return roles;
}

QVariant DevicesInfoList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}
/*
Qt::ItemFlags DevicesInfoList::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
*/
bool DevicesInfoList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        //devicesList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    } 
    return false;
}

bool DevicesInfoList::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        //devicesList.insert(position, "");
    }
    endInsertRows();
    return true;
 
}

bool DevicesInfoList::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        devicesList.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void DevicesInfoList::addDevice(DeviceHandle& kDevice)
{ 
    devicesList.push_back(kDevice);
    emit dataChanged(index(devicesList.size()), index(devicesList.size()));
    emit layoutChanged();
}

bool DevicesInfoList::isExist(const QString& uid)
{
    auto result = std::find_if(devicesList.begin(), devicesList.end(), UIDMatch(uid));
    return result == std::end(devicesList);
}

/*******************************************/
/*/////////////DeviceCmdList///////////////*/
/*******************************************/
DeviceCmdList::DeviceCmdList(QObject* pParent)
: DevicesInfoList(pParent)
{
}

int DeviceCmdList::rowCount(const QModelIndex &parent) const
{
    if (devicesList.count() > 0)
    {
        return devicesList.at(DeviceIndex).getCmdDisplayName().size();
    }
    return 0;
}

QVariant DeviceCmdList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (devicesList.count() > 0)
    {
        if (index.row() >= devicesList.at(DeviceIndex).getCmdDisplayName().size())
            return QVariant();
    }
    else return QVariant();

    switch (role) {
    case DisplayNameRole: // The default display role now displays the first name as well
        return devicesList.at(DeviceIndex).getCmdDisplayName().at(index.row());
    case NameRole:
        return devicesList.at(DeviceIndex).getCmdKeys().at(index.row());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DeviceCmdList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayNameRole] = "displayName";
    roles[NameRole] = "name";
    return roles;
}

QVariant DeviceCmdList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

bool DeviceCmdList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        //cmdList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool DeviceCmdList::insertRows(int position, int rows, const QModelIndex &parent)
{
    return false;
}

bool DeviceCmdList::removeRows(int position, int rows, const QModelIndex &parent)
{
    return true;
}

void DeviceCmdList::addDevice(const DeviceHandle& kDevice)
{
    devicesList.push_back(kDevice);
    emit dataChanged(index(devicesList.at(DeviceIndex).getCmdDisplayName().size()), index(devicesList.at(DeviceIndex).getCmdDisplayName().size()));
    emit layoutChanged();
}

void DeviceCmdList::updateDeviceIndex(int nIndex)
{
    beginResetModel();
    
    endResetModel();
    DeviceIndex = nIndex;
    emit dataChanged(index(devicesList.at(DeviceIndex).getCmdDisplayName().size()), index(devicesList.at(DeviceIndex).getCmdDisplayName().size()));
    emit layoutChanged();

}