#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QStringList>
#include "DeviceHandle.h"

class DevicesInfoList : public QAbstractListModel
{
    Q_OBJECT
public:
    DevicesInfoList(QObject *parent = 0);
    enum DevicelRoles {
        UIDRole = Qt::UserRole + 1,
        NameRole,
        IPRole,
        TypeRole
    };

    //// pure virtuals implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void addDevice(DeviceHandle& kDevice);
    bool isExist(const QString& uid);

public Q_SLOTS:
    void removeAllRows();

protected:
    QList<DeviceHandle> devicesList;
};

class DeviceCmdList : public DevicesInfoList
{
    Q_OBJECT
public:
    DeviceCmdList(QObject *parent = 0);
    enum DevicelRoles {
        DisplayNameRole = Qt::UserRole + 1,
        NameRole
    };

    //// pure virtuals implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void addDevice(const DeviceHandle& kDevice);

public Q_SLOTS:
    void updateDeviceIndex(int nIndex);
    void removeAllRows();

protected:
    int DeviceIndex = 0;
};
