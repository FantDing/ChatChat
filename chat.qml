import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:chatCom
    anchors.fill: parent
    property alias theModel: charView.model
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
                }
            }
            Button{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                text:"close"
                onClicked: {
                    chatCom.visible=false;
                    chatCom.destroy();
                }
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
            onCurrentIndexChanged: {
                //1. is the zero index
                //2. is clear
                if(currentIndex==-1||currentIndex==0){
                    return;
                }
            }
            //life-cycle
            Component.onCompleted:{
                //@ show the last item in at the bottom
                positionViewAtEnd()
            }
            
            Component{
                id:chatDelegate
                Rectangle{
                    height: 40
                    width: parent.width
                    Text {
                        anchors.right: parent.right
                        text: content
                        font.pointSize: 16
                    }
                }
            }//chatDelegate-end
        }//chatView-end
        Row{
            // input & send
            width: parent.width-100;
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
                    chatModel.pushBack(send_content.text)
                    send_content.text=""
                    send_content.focus=true;
                }
            }
        }
    }
    Component.onDestruction: {
        console.log('destroyed')
    }
    
    Component.onCompleted: {
        console.log("generate")
    }
}
