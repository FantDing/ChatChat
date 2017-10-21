import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    id:root
    visible: true
    width: 380
    height: 600
    title: qsTr("ChatChat")
    property var currentPage: null
    property bool hasLogin: false
    onClosing:{
        if(hasLogin)
        {
            sendMsg(1,"broadcast");
        }
    }
    
    Component.onCompleted: {
        //@ set start position
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);

        var login=Qt.createComponent("login.qml");
        currentPage=login.createObject(root);
        currentPage.login.connect(onLogin);
    }

    function onLogin(name){
        hasLogin=true;
        currentPage.destroy()
        //@ show home page
        var home=Qt.createComponent("home.qml");
        currentPage=home.createObject(root,{
                                          "username":name
                                      });

    }
}
