#pragma once

#include <QObject>
#include <QString>

class DeviceHandle : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QString deviceUID            READ getUID      WRITE setUID);
    Q_PROPERTY(QString deviceName           READ getName     WRITE setName);
    Q_PROPERTY(QString deviceIP             READ getIP       WRITE setIP);
    Q_PROPERTY(QList<QString> deviceCmdKeys READ getCmdKeys  WRITE setCmdKeys);

public:
    DeviceHandle(QObject* parent = 0);
    DeviceHandle(const DeviceHandle& kHandle);
    DeviceHandle(const QString& deviceUID,
                  const QString& deviceName, 
                  const QString& deviceIP, 
                  const QString& deviceType, 
                  const QList<QString>& deviceCmdKeys,
                  const QList<QString>& deviceCmdDisplayNames);
    virtual ~DeviceHandle();

    DeviceHandle& operator=(const DeviceHandle& kHandle);


public Q_SLOTS:
    QString getUID() const                       { return deviceUID; }
    void setUID(const QString& qstrName)         { deviceUID = qstrName; }
    QString getName() const                      { return deviceName; }
    void setName(const QString& qstrName)        { deviceName = qstrName; }
    QString getIP() const                        { return deviceIP; }
    void setIP(const QString& qstrName)          { deviceIP = qstrName; }
    QList<QString> getCmdKeys() const                { return deviceCmdKeys; }
    void setCmdKeys(const QList<QString>& qstrName)  { deviceCmdKeys = qstrName; }
    QList<QString> getCmdDisplayName() const                { return deviceCmdDisplayNames; }
    void setCmdDisplayName(const QList<QString>& qstrName)  { deviceCmdDisplayNames = qstrName; }

private:
    QString deviceUID;
    QString deviceName;
    QString deviceIP;
    QString deviceType;
    QList<QString> deviceCmdKeys;
    QList<QString> deviceCmdDisplayNames;
};

Q_DECLARE_METATYPE(DeviceHandle);
