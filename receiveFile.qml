import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.2

Item {
    id:receiveFile
    width: parent.width*2/3;
    height: parent.width*2/3-20;
    anchors.centerIn: parent;
    property string frindIpv4: ""
    property string frindName: ""
    property string fileName: ""
    Rectangle{
        anchors.fill: parent;
//        color: Material.color(Material.BlueGrey)
        color: "#D3D3D4"
        ProgressBar{
            width: parent.width;
            value: 0.5
        }
        Column{
            anchors.centerIn: parent
            Text {
                id: name
                text: fileName
                width: 200
                clip: true;
                elide: Text.ElideMiddle
            }
            Text {
                text: "from: "+frindName
            }
            Text {
                text: "address:"+frindIpv4
            }
            Row{
                spacing: 4
                anchors.horizontalCenter: parent.horizontalCenter
                Button{
                    text: "Receive"
                    Material.background: Material.Teal
                    onClicked: {
                        folderSelect.visible=true;
                    }
                }
                Button{
                    text: "Refuse"
                    Material.background: Material.Red
                    onClicked: {
                        receiveFile.destroy();
                        sendMsg(4,"broadcast",frindName);
                    }
                }
            }
            
        }
        
        
        FileDialog{
            id:folderSelect;
            folder: shortcuts.home;
            selectFolder: true;
            onAccepted: {
                var path=folderSelect.fileUrl;
                console.log(path.toString().slice(7))
                setFullPath(path.toString().slice(7))
                acceptAndConnect(frindIpv4);
            }
        }
    }
    
}
