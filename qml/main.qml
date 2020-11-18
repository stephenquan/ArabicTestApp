import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Window {
    id: app

    visible: true
    width: 800
    height: 600
    title: qsTr("Arabic Test App")

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: ArabicTestApp {
        }
    }
}
