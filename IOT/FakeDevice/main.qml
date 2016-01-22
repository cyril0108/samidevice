import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 960
    height: 540
    title: qsTr("Fake Device")

    FakeDeviceForm {
        anchors.fill: parent
    }
}

