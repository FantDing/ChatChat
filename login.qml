/*
login page: start page
*/

import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:loginCom;
    anchors.fill: parent
    signal login(var name);
    Column{
        anchors.centerIn: parent
        Row{
            spacing: 8
            Label{
                text: "username"
                topPadding: 10
            }

            TextField{
                focus: true;
                id:username;
                width: 200;
            }
        }

        Button{
            id:btnLogin
            anchors.right: parent.right
            text:"login"
            onClicked:{
                if(username.text==""){
                    console.log("empty input")
                    return
                }
                loginCom.visible=false;
                //@ emit login signal
                loginCom.login(username.text);
            }
            Material.background: Material.Teal
        }
    }
}