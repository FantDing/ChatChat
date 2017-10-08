import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.2

Item {
    id:chooseFile
    width: parent.width*2/3;
    height: parent.width*2/3-20;
    anchors.centerIn: parent;
    property string frindIpv4: ""
    property string frindName: ""
    
    Rectangle{
        anchors.fill: parent;
        color: Material.color(Material.BlueGrey)
        ProgressBar{
            width: parent.width;
            value: 0.5
        }
        Column{
            anchors.centerIn: parent
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                id: txt_status
                font.pointSize: 16
            }
            Row{
                //filename & choose btn
                Label{
                    id:fileFullName;
                    text: "choose your file..."
                    topPadding: 10
                    clip: true;
                    elide: Text.ElideMiddle
                    width: 150
                }
                
                Button{
                    text: "choose";
                    onClicked: {
                        fileDialog.open();
                    }
                }
                
            }
            Row{
                // send & close btn
                Button{
                    text:"send";
                    onClicked: {
                        sendMsg(3,"broadcast",frindName,fileFullName.text)
//                    online:    sendMsg(3,frindIpv4,frindName,fileFullName.text)
                    }
                }
                Button{
                    text:"close";
                    onClicked: chooseFile.destroy();
                }
            }
        }
        FileDialog{
            //does not visible
            id:fileDialog
            title: "choose a file to send";
            folder: shortcuts.home
            onAccepted: {
                fileFullName.text=fileDialog.fileUrl;
                setFileName(fileFullName.text);
            }
        }
    }
    function onSignal(status){
        txt_status.text=status;
    }

    Component.onCompleted: {
        onFileStatus.connect(onSignal);
    }
    
}
