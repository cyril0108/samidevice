import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
//import com.azul.iot 1.0

Item {
    id: fakeDeviceForm
    objectName: "fakeDeviceForm"
    width: 960
    height: 540

    property Item listHeaderItem: null
    Item {
        id: listHeaderComponent
        Item {
            id: headerItem
            Text {
                text: "545465465"
            }
            width: deviceListView.width
            height: 10
            Component.onCompleted: fakeDeviceForm.listHeaderItem = headerItem
        }
    }

    Rectangle {
        id: deviceListHeader
        anchors.top: parent.top
        width: parent.width / 2
        height: parent.height / 2
        Rectangle {
            Text {
                text: "  Device List"
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }
            width: parent.width
            height: 25
            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "#000000"
                }
                GradientStop {
                    position: 1.00
                    color: "#6d6d6d"
                }
            }
        }
    }

    Rectangle {
        id: cmdListHeader
        anchors.top: parent.top
        anchors.left: deviceListHeader.right
        width: parent.width / 2
        height: parent.height / 2
        Rectangle {
            Text {
                text: "  Command List"
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }
            width: parent.width
            height: 25
            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "#000000"
                }
                GradientStop {
                    position: 1.00
                    color: "#6d6d6d"
                }
            }
        }
    }

    ListView {
        id: deviceListView
        anchors.top: parent.top
        anchors.topMargin: 50
        width: parent.width / 2
        height: parent.height / 2
        model: FakeDevicesInfoList
        header: deviceListHeader
        delegate: Item {
            x: 5
            width: 295
            height: 30
            scale: mouseArea1.containsMouse ? 0.95 : 1.0
            smooth: mouseArea1.containsMouse
            property variant myData: model
            Row {
                id: row1
                Rectangle {
                    width: 30
                    height: 30
                    gradient: Gradient {
                        GradientStop {
                            position: 0.00
                            color: "green"
                        }
                        GradientStop {
                            position: 1.00
                            color: "#c4c4c4"
                        }
                    }
                }

                Text {
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: model.uid
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: model.ip
                    anchors.verticalCenter: parent.verticalCenter
                }
                spacing: 10
            }
            MouseArea {
                id: mouseArea1
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    if( deviceListView.currentIndex != index)
                    {
                        FakeDeviceCmdList.removeAllRows()
                    }
                    deviceListView.currentIndex = index
                }
            }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "opacity"
                    to: 0
                    duration: 1000
                }
                NumberAnimation {
                    properties: "x,y"
                    to: 100
                    duration: 1000
                }
            }
        }
        //delegate: contactDelegate
        highlight: Rectangle {
            color: "black"
            radius: 5
            opacity: 0.2
            focus: true
            width: parent.width
        }
    }

    ListView {
        id: commandListView
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: deviceListView.right
        width: parent.width / 2
        height: parent.height / 2
        model: FakeDeviceCmdList
        header: cmdListHeader
        visible: deviceListView.currentIndex != -1
        delegate: Item {
            x: 5
            width: 293
            height: 30
            scale: mouseArea2.containsMouse ? 0.95 : 1.0
            smooth: mouseArea2.containsMouse
            property variant myData: model
            Row {
                id: row2
                Text {
                    text: model.displayName
                }
                Text {
                    text: model.name
                    visible: false
                }
                spacing: 5
            }
            MouseArea {
                id: mouseArea2
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    commandListView.currentIndex = index
                }
            }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
            }
        }
        highlight: Rectangle {
            color: "black"
            radius: 5
            opacity: 0.2
            focus: true
            width: parent.width
        }
    }

    TextField {
        id: commandTextField
        anchors.top: deviceListView.bottom
        anchors.bottomMargin: 30
        width: 154
        height: 40
        placeholderText: qsTr("Input Digits")
        font.bold: true
        inputMethodHints: Qt.ImhDigitsOnly
        validator: RegExpValidator {
            regExp: /\d+/
        }
        visible: commandListView.count > 0 && commandListView.currentIndex != -1
    }
    Button {
        id: setCommandButton
        anchors.top: deviceListView.bottom
        anchors.bottomMargin: 30
        anchors.left: commandTextField.right
        width: 139
        height: 40
        text: qsTr("Send Command")
        onClicked: sendCommandToDevice()
        visible: commandListView.count > 0 && commandListView.currentIndex != -1
    }

    Button {
        id: getCommandsButton
        width: 139
        height: 40
        text: qsTr("List Command")
        anchors.top: getDevicesButton.bottom
        anchors.topMargin: 10
        onClicked: listCommandBtn()
        visible: deviceListView.currentIndex != -1
    }

    Button {
        id: getDevicesButton
        width: 139
        height: 40
        text: qsTr("List Device")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 90
        onClicked: listDevices()
    }

    Rectangle {
        id: queryView
        anchors.top: deviceListView.bottom
        anchors.bottomMargin: 30
        anchors.left: setCommandButton.right
        anchors.leftMargin: 100
        property variant myData: model
        width: 400
        height: 200
        Text {
            id: queryViewText
            text: ""
            anchors.verticalCenter: parent.verticalCenter
        }
        //visible: commandListView.count > 0 && commandListView.currentIndex != -1
    }

    function sendCommandToDevice() {
        if (deviceListView.currentIndex != -1) {
            console.log("sendCommandToDevice")
            console.log(deviceListView.currentIndex)
            console.log(commandTextField.text)
            var uid = deviceListView.currentItem.myData.uid
            var cmdKey = commandListView.currentItem.myData.name
            FakeDeviceApp.sendCommandToDevice("command", "sendCommandToDevice", "uid", uid, "deviceCmd",cmdKey, "param",commandTextField.text)
        }
    }

    function listDevices() {
        console.log("listDevices")
        FakeDeviceApp.sendCommandToDevice("command", "listDevices")
    }

    function listCommandBtn() {
        console.log("listCommandBtn")
        var uid = deviceListView.currentItem.myData.uid
        FakeDeviceApp.sendCommandToDevice("command", "listCommands", "uid", uid)
    }

    function updateTextField(kText, bClear)
    {
        console.log( "updateTextField" )
        queryViewText.text = kText;
    }

    Connections {
        target: FakeDeviceEventSender
        onCommandReturned: updateTextField(kText, bClear);
    }

}
