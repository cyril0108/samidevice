import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
//import com.azul.iot 1.0

Item {
    id: serverAppForm
    objectName: "serverAppForm"
    x: 0
    width: 640
    height: 500

    //property alias setCommandButton: setCommandButton
    //property alias deviceListView: deviceListView
    //property alias commandListView: commandListView
    //property alias commandTextField: commandTextField
    property Item listHeaderItem: null
      Component {
        id: listHeaderComponent
        Item {
          id: headerItem
          Text { text: "545465465" }
          width: deviceListView.width; height: 10
          Component.onCompleted: serverAppForm.listHeaderItem = headerItem
        }
      }

    ListView {
        id: deviceListView
        x: 8
        y: 31
        width: 295
        height: 459
        signal updateCommandListView(int nIndex)
        model: DevicesInfoList
        header: deviceListHeader
        delegate: Item {
            x: 5
            width: 295
            height: 30
            scale:  mouseArea1.containsMouse ? 0.95 : 1.0
            smooth: mouseArea1.containsMouse
            property variant myData: model
            Row {
                id: row1
                Rectangle {
                    width: 30
                    height: 30
                    gradient: Gradient {
                        GradientStop { position: 0.00; color: "green"; }
                        GradientStop { position: 1.00; color: "#c4c4c4"; }
                    }
                }

                Text { text: model.name;     anchors.verticalCenter: parent.verticalCenter }
                Text { text: model.uid;      anchors.verticalCenter: parent.verticalCenter }
                Text { text: model.ip;       anchors.verticalCenter: parent.verticalCenter }
                spacing: 10
            }
            MouseArea {
                id: mouseArea1
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    deviceListView.currentIndex = index;
                    deviceListView.updateCommandListView( index );
                    DeviceCmdList.updateDeviceIndex(index);
                }
            }
        }
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
            }
        }
        //delegate: contactDelegate
        highlight: Rectangle
        {
            color:"black"
            radius: 5
            opacity: 0.2
            focus: true
        }
    }

    ListView {
        id: commandListView
        x: 309
        y: 31
        width: 293
        height: 437
        model: DeviceCmdList
        header: cmdListHeader
        delegate: Item {
            x: 5
            width: 293
            height: 30
            scale:  mouseArea2.containsMouse ? 0.95 : 1.0
            smooth: mouseArea2.containsMouse
            property variant myData: model
            Row {
                id: row2
                Text { text: model.displayName; }
                Text { text: model.name; visible: false }
                spacing: 5
            }
            MouseArea {
                id: mouseArea2
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    commandListView.currentIndex = index;
               }
            }
        }
        
        highlight: Rectangle
        {
            color:"black"
            radius: 5
            opacity: 0.2
            focus: true
        }
    }

    Button {
        id: setCommandButton
        x: 463
        y: 470
        width: 139
        height: 20
        text: qsTr("SendCommandToDevice")
        onClicked: sendCommandToDevice();
        visible:  commandListView.count > 0
    }

    TextField {
        id: commandTextField
        x: 309
        y: 470
        width: 154
        height: 20
        placeholderText: qsTr("Input Digits")
        font.bold: true
        inputMethodHints: Qt.ImhDigitsOnly
        validator: RegExpValidator{regExp: /\d+/}
        visible:  commandListView.count > 0
    }

    Component {
        id: deviceListHeader
        Rectangle {
            Text { text: "  Device List"; color: "white"; anchors.verticalCenter: parent.verticalCenter }
            width:  parent.width
            height: 25
            gradient: Gradient {
                GradientStop { position: 0.00; color: "#000000"; }
                GradientStop { position: 1.00; color: "#6d6d6d"; }
            }
        }
    }

    Component {
        id: cmdListHeader
        Rectangle {
            Text { text: "  Command List"; color: "white"; anchors.verticalCenter: parent.verticalCenter }
            width:  parent.width
            height: 25
            gradient: Gradient {
                GradientStop { position: 0.00; color: "#000000"; }
                GradientStop { position: 1.00; color: "#6d6d6d"; }
            }
        }
    }
    function sendCommandToDevice()
    {
        if ( deviceListView.currentIndex != -1 && commandTextField.text != "")
        {
            console.log( "sendCommandToDevice" )
            console.log( deviceListView.currentIndex )
            console.log( commandTextField.text )
            var uid = deviceListView.currentItem.myData.uid;
            var cmdKey = commandListView.currentItem.myData.name;
            ServerApp.sendCommandToDevice( uid, cmdKey, commandTextField.text );
        }
    }
}

