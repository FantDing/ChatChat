import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:homeCom
    anchors.fill: parent
    property alias username: username.text
    property var currentPage: null
    
    function onSignal(friendName,friendIpv4,fileName){
        console.log(fileName+" "+friendName+" "+friendIpv4)
        var com=Qt.createComponent("receiveFile.qml");
        com.createObject(homeCom
                         ,{
                             "frindName":friendName,
                             "frindIpv4":friendIpv4,
                             "fileName":fileName
                         }
                         );
    }
    Component.onCompleted: {
        onFileCome.connect(onSignal)
    }
    
    Column{
        Rectangle{
            //header
            id:header
            width: homeCom.width
            height: 100
            color: Material.color(Material.Cyan)
            Text{
                anchors.centerIn: parent
                id:username
                font.pointSize: 30
                color: Material.color(Material.Brown)
            }
        }//head-end
        ScrollView{
            // friends list
            width: homeCom.width
            height: homeCom.height-header.height
            contentWidth: homeCom.width
            ListView{
                Component.onCompleted: {
                    onFriendExit.connect(onFriendE);
                    function onFriendE(index){
                        if(index===friendsView.currentIndex){
                            //必须先unvisibel,最后才能destroy.否则界面会卡住
                            currentPage.visible=false;
                            currentPage.destroy();
                        }
                    }
                }

                id:friendsView
                anchors.fill: parent
                
                model: friendModel
                delegate: friendsDelegate
                
                spacing: 10
                //clip makes item does go beyond the area
                clip: true;
                
                Component {
                    id:friendsDelegate
                    Rectangle{
                        id:rec
                        //to use "friendsView.currentItem.name", must expose the property 
                        property alias name: txt_nickName.text
                        property alias address: txt_ipv4.text
                        height: 50
                        width: parent.width
                        Rectangle{
                            //border: to border a friend
                            id:borderLine
                            anchors{
                                fill: parent
                                leftMargin: -1
                                rightMargin: -1
                            }
                            border{
                                width: 1
                                color: Qt.rgba(0.2,0.2,0.2,0.2)
                            }
                        }
                        
                        // show name\ipv4\newMsgCount
                        Column{
                            width: 200
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            Text {
                                id:txt_nickName
                                text:nickName
                                font.pointSize: 15
                            }
                            Text {
                                id:txt_ipv4
                                text: ipv4
                                font.pointSize: 10
                                color: "gray"
                            }
                        }
                        
                        Rectangle{
                            id:msgCount
                            visible: txt_newMsgCount.text==="0"?false:true;
                            width: 18
                            height: 18
                            radius: 9
                            color: "red"
                            anchors.right: parent.right
                            anchors.rightMargin: 8
                            anchors.verticalCenter: parent.verticalCenter
                            Text {
                                id: txt_newMsgCount
                                color: "white"
                                anchors.centerIn: parent
                                text: newMsgCount
                                font.bold: true;
                            }
                        }

                        
                        
                        MouseArea{
                            anchors.fill: parent;
                            
                            onClicked: {
                                //The index is exposed as an accessible index property
                                friendsView.currentIndex=index
                                setCurrentFriend(friendsView.currentItem.name+friendsView.currentItem.address);
                                var charCom=Qt.createComponent("chat.qml")
                                currentPage=charCom.createObject(homeCom
                                                                     ,{
                                                                         "theModel":chatModel,
                                                                         "friendName":friendsView.currentItem.name,
                                                                         "frindIpv4":friendsView.currentItem.address
                                                                     }
                                                                     );
                            }
                        }
                    }
                }//friendsDelegate-end
            }//friendsView-end
        }//scroll-end
    }
}
