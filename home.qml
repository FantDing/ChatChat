import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:homeCom
    anchors.fill: parent
    property alias username: username.text
    property bool isStartTime: true
    
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
                        property alias name: txt_nickName.text
                        property alias address: txt_ipv4.text
                        height: 50
                        width: parent.width
                        Rectangle{
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
                        Column{
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            Text {
                                id:txt_nickName
                                text:nickName
                                font.pointSize: 16
                            }
                            Text {
                                id:txt_ipv4
                                text: ipv4
                                font.pointSize: 12
                                color: "gray"
                            }
                        }
                        MouseArea{
                            anchors.fill: parent;
                            onClicked: {
                                //The index is exposed as an accessible index property
                                friendsView.currentIndex=index
                                setCurrentFriend(friendsView.currentItem.name+friendsView.currentItem.address);
                                console.log(friendsView.currentItem.name+":"+friendsView.currentItem.address);
                                var charCom=Qt.createComponent("chat.qml")
                                var currentPage=charCom.createObject(homeCom
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
