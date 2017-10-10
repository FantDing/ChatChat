import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:chatCom
    anchors.fill: parent
    property alias theModel: charView.model
    property alias friendName: friendName.text
    property string frindIpv4: ""
    
    
    Component.onCompleted: {
        onUpdateChatView.connect(toEnd);
        function toEnd(){
            charView.positionViewAtEnd();   
        }
    }
    
    //    @ it's too fast to update the view
    //    Connections{
    //        target: theModel;
    //        onRowsInserted:{
    //            console.log("add...")
    //            charView.positionViewAtEnd();
    //        }
    //    }
    
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
                    //新消息显示为0(函数在c++中定义)
                    clearNewMsgCount(index);
                    
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
            delegate: chatDelegate
            
            Component.onCompleted: {
                positionViewAtEnd();
            }
            Component{
                id:chatDelegate
                Rectangle{
                    width: parent.width-20
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: childrenRect.height
                    Label {
                        id:textContent
                        text: content
                        width: parent.width-40
                        font.pointSize: 13
                        wrapMode: Text.WrapAnywhere
                        Component.onCompleted: {
                            if(direction==true){
                                //是我发送的文字
                                textContent.horizontalAlignment=Text.AlignRight;
                                textContent.anchors.right=parent.right
                            }
                        }
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
                anchors.bottom: btn_send.bottom
                focus: true
                width: 200
                
                KeyNavigation.priority: KeyNavigation.BeforeItem;
                KeyNavigation.tab: btn_send;
                
            }
            Button{
                id:btn_send
                text:"send"
                Keys.onEnterPressed: {
                    clicked();
                }

                onClicked: {
                    console.log("click")
                    if(send_content.text.trim().length==0){
                        send_content.focus=true;
                        return;
                    }
                    chatModel.pushBack(send_content.text,true);//add to model
                    var type=2;
                    if(frindIpv4+friendName.text===friendName.text){
                        //robot
                        type=5;
                    }
                    sendMsg(type,"broadcast",friendName.text,send_content.text);//send
                    send_content.text="";
                    send_content.focus=true;
                    charView.positionViewAtEnd();
                }
            }
        }
        
        Row{
            //function area
            anchors.horizontalCenter: parent.horizontalCenter
            Button{
                id:sendFile
                text:"F";
                background: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 40
                    opacity: enabled ? 1 : 0.3
                    border.color: sendFile.down ? "#17a81a" : "#21be2b"
                    border.width: 1
                    radius: 22
                }
                onClicked: {
                    var comp=Qt.createComponent("chooseFile.qml");
                    comp.createObject(chatCom
                                      ,{
                                          "frindIpv4":frindIpv4,
                                          "frindName":friendName.text
                                      }
                                      );
                }
            }
        }
        
    }
}
