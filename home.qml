import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Item {
    id:homeCom
    anchors.fill: parent

    property alias username: username.text

    Row{
        Rectangle{
            width: homeCom.width
            height: 100
            color: Material.color(Material.Cyan)

            Text{
                anchors.centerIn: parent
                id:username
                font.pointSize: 30
                color: Material.color(Material.Brown)
            }

            //@ test
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    var chat=Qt.createComponent("chat.qml")
                    chat.createObject(homeCom);
                }
            }
        }
    }
}
