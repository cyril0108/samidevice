#include "FakeDevicesInfoList.h"
#include "FakeDeviceHandle.h"

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
FakeDevicesInfoList::FakeDevicesInfoList(QObject* pParent)
: QAbstractListModel(pParent)
{
}

int FakeDevicesInfoList::rowCount(const QModelIndex &parent) const
{
    return devicesList.count();
}

QVariant FakeDevicesInfoList::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> FakeDevicesInfoList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UIDRole]  = "uid";
    roles[NameRole] = "name";
    roles[IPRole]   = "ip";
    return roles;
}

QVariant FakeDevicesInfoList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

bool FakeDevicesInfoList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        //devicesList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    } 
    return false;
}

bool FakeDevicesInfoList::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        //devicesList.insert(position, "");
    }
    endInsertRows();
    return true;
 
}

bool FakeDevicesInfoList::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        devicesList.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void FakeDevicesInfoList::addDevice(DeviceHandle& kDevice)
{ 
    devicesList.push_back(kDevice);
    emit dataChanged(index(devicesList.size()), index(devicesList.size()));
    emit layoutChanged();
}

bool FakeDevicesInfoList::isExist(const QString& uid)
{
    auto result = std::find_if(devicesList.begin(), devicesList.end(), UIDMatch(uid));
    return result == std::end(devicesList);
}

void FakeDevicesInfoList::removeAllRows()
{
    removeRows(0, devicesList.size(), index(0, 0));
}

/*******************************************/
/*/////////////DeviceCmdList///////////////*/
/*******************************************/
FakeDeviceCmdList::FakeDeviceCmdList(QObject* pParent)
: FakeDevicesInfoList(pParent)
{
}

int FakeDeviceCmdList::rowCount(const QModelIndex &parent) const
{
    if (devicesList.count() > 0)
    {
        return devicesList.at(DeviceIndex).getCmdDisplayName().size();
    }
    return 0;
}

QVariant FakeDeviceCmdList::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> FakeDeviceCmdList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayNameRole] = "displayName";
    roles[NameRole] = "name";
    return roles;
}

QVariant FakeDeviceCmdList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}

bool FakeDeviceCmdList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {

        //cmdList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

bool FakeDeviceCmdList::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        //devicesList.insert(position, "");
    }
    endInsertRows();
    return true;
}

bool FakeDeviceCmdList::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        devicesList.removeAt(position);
    }
    endRemoveRows();
    return true;
}

void FakeDeviceCmdList::addDevice(const DeviceHandle& kDevice)
{
    devicesList.push_back(kDevice);
    emit dataChanged(index(devicesList.at(DeviceIndex).getCmdDisplayName().size()), index(devicesList.at(DeviceIndex).getCmdDisplayName().size()));
    emit layoutChanged();
}

void FakeDeviceCmdList::updateDeviceIndex(int nIndex)
{
    //beginResetModel();
   // endResetModel();
    //DeviceIndex = nIndex;
    //emit dataChanged(index(devicesList.at(DeviceIndex).getCmdDisplayName().size()), index(devicesList.at(DeviceIndex).getCmdDisplayName().size()));
    //emit layoutChanged();
}

void FakeDeviceCmdList::removeAllRows()
{
    if (devicesList.size() > 0)
    {
        removeRows(0, devicesList.at(0).getCmdDisplayName().size(), index(0, 0));
    }
}