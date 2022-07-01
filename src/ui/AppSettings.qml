pragma Singleton

import QtQuick 2.15

Item {
    id: appSettings
    property int wHeight
    property int wWidth

    readonly property color backgroundColor: "black"
    readonly property color textColor: "white"
    readonly property color menuItemColor: "gray"
}
