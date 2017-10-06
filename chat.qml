import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:chatCom
    anchors.fill: parent
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
        Rectangle{
            //chat record
            width: parent.width;
            implicitHeight: 300;
            color: "red"
        }
        Row{
            width: parent.width-100;
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
                    send_content.text=""
                }
            }
        }



    }
    Component.onDestruction: {
        console.log('destroyed')
    }
}
