import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ArcGIS.AppFramework.Prototype 1.0

Page {
    QtObject {
        id: styles

        property color headerBackgroundColor: "#e0e0e0"
        property color headerTextColor: "black"
        property double headerTextPointSize: 16
        property double heading1TextPointSize: 14
        property double textPointSize: 12
    }

    QtObject {
        id: properties

        property string arabicText: ""
        property url fileUrl: "itemsets.csv"
    }

    header: Frame {
        background: Rectangle {
            color: styles.headerBackgroundColor
        }

        RowLayout {
            width: parent.width

            Text {
                Layout.fillWidth: true

                text: app.title
                font.pointSize: styles.headerTextPointSize
                color: styles.headerTextColor
            }
        }
    }

    Flickable {
        id: flickable

        anchors.fill: parent
        anchors.margins: 10

        contentWidth: columnLayout.width
        contentHeight: columnLayout.height
        clip: true

        ColumnLayout {
            id: columnLayout

            width: flickable.width

            /*
            Text {
                Layout.fillWidth: true

                text: qsTr("Arabic Text")
                font.pointSize: styles.heading1TextPointSize
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
            */

            Text {
                Layout.fillWidth: true

                text: properties.arabicText
                font.pointSize: styles.textPointSize
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }

    TextFile {
        id: textFile
    }

    Component.onCompleted: {
        textFile.open(properties.fileUrl);
        properties.arabicText = textFile.readAll();
        console.log(properties.arabicText);
        textFile.close();
    }
}
