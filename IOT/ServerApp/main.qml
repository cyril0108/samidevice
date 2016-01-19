import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
//import com.azul.iot 1.0

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 640
    height: 500
    title: qsTr("Server App")
    property variant paramMap: []
    property var cmdMap
    property var pickedObjects

    ServerAppForm {
        anchors.fill: parent
        //loginBtn.onClicked: ServerApp.getProportionProfilePath()//messageDialog.show(qsTr("loginBtn pressed"))
        //deviceListView.mouseArea1.onClicked: deviceListView.currentIndex = index;
        //commandListView.mouseArea2.onClicked: commandListView.currentIndex = index;
        //setCommandButton.onClicked: sendCommandToDevice();
        /*
        function sendCommandToDevice()
        {
            console.log( "sendCommandToDevice" )
            if ( deviceListView.currentIndex >= 0 )
            {
                var myMap;
                //myMap.push();
                //([["command", "adjustTemperature"], ["param", "35"], ["seq", 1234]]);
                var uid = deviceListModel.get(deviceListView.currentIndex).uid;
                ServerApp.sendCommandToDevice(uid, myMap);
                newObject.destroy(1000);
            }
        }*/
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }



}

