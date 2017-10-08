import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:chatCom
    anchors.fill: parent
    property alias theModel: charView.model
    property alias friendName: friendName.text
    property string frindIpv4: ""
    Rectangle{
        //@ because next Column'items has space,you will
        // see parent in the space. So, this Rectangle can
        // cover parent
        anchors.fill: parent
        color: "white"
        z:-1
    }
    
    Column{
        spacing: 4
        anchors.fill: parent
        Rectangle{
            //head
            height: 50
            width: parent.width
            Button{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                text: "back"
                onClicked: {
                    chatCom.visible=false;
                    chatCom.destroy();
                }
            }
            Label{
                id:friendName;
                anchors.centerIn: parent
                font.pointSize: 20
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                text:"clear"
                onClicked: {
                    //                    chatCom.visible=false;
                    chatModel.clear();
                }
                Material.background: Material.Red
            }
        }//head-end
        ListView{
            //chat record
            id:charView
            height: 400
            clip: true;
            width: parent.width
            //        model: 
            delegate: chatDelegate
            
            Component.onCompleted: {
                positionViewAtEnd();
            }
            
            
            Component{
                id:chatDelegate
                Rectangle{
                    height: 40
                    width: parent.width-10
                    Text {
                        anchors.right: direction?parent.right:undefined
                        text: content
                        font.pointSize: 16
                    }
                }
            }//chatDelegate-end
        }//chatView-end
        Row{
            // input & send
            height: 50
            spacing: 20
            anchors.horizontalCenter: parent.horizontalCenter
            TextArea{
                id:send_content
                focus: true
                width: 200
            }
            Button{
                text:"send"
                onClicked: {
                    console.log("click")
                    if(send_content.text.trim().length==0){
                        send_content.focus=true;
                        return;
                    }
                    chatModel.pushBack(send_content.text,true);//add to model
                    sendMsg(2,"broadcast",friendName.text,send_content.text);//send
                    send_content.text="";
                    send_content.focus=true;
                    charView.positionViewAtEnd();
                }
            }
        }
        
        Row{
            //function area
            Button{
                id:sendFile
                text:"file";
                onClicked: {
                    var comp=Qt.createComponent("chooseFile.qml");
                    comp.createObject(chatCom
                                      ,{
                                          "frindIpv4":frindIpv4,
                                          "frindName":friendName.text
                                      }
                                      );
                    initalizeTcp();
                }
            }
        }
        
    }
}
