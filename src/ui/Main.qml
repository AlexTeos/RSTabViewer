import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    width: 800
    height: 360
    visible: true
    title: qsTr("Booktionary")
    id: applicationWindow

    Component.onCompleted: {
        AppSettings.wWidth = Qt.binding(function() {return width})
        AppSettings.wHeight = Qt.binding(function() {return height})
    }

    Timer {
        id: flickTimer
        interval: 50;
        running: false;
        repeat: true
        onTriggered: tablatureView.flick(-200, 0)
    }

    Button{
        anchors.left: parent.left
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'start'

        onClicked: {
            flickTimer.start()
        }
    }

    Button{
        anchors.right: parent.right
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'stop'

        onClicked: {
            flickTimer.stop()
        }
    }

    Rectangle{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height/2
        color: "#f2d0ea"

        Component {
            id: noteDelegate
            Item {
                //color: "#e594eb"
                width: (timeEnd - timeStart) * 300
                height: parent.height
                //opacity: 0.5
                //radius: 10
                Column {
                    anchors.fill: parent

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/4
                        color: "black"
                        Text {
                            text: name
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            font.bold: true
                            color: "white"
                            }
                        opacity: name !== ""
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret0
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret0 !== 255 ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret1
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret1 !== 255 ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret2
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret2 !== 255 ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret3
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret3 !== 255 ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret4
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret4 !== 255 ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: fret5
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: fret5 !== 255 ? 100 : 0
                    }
                }
            }
        }

        Rectangle {
            id: cursor
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: parent.width/4
            width: 5
            color: "green"
            opacity: 0.5
        }

        Rectangle {
            id: fret0Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 1.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        Rectangle {
            id: fret1Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 2.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        Rectangle {
            id: fret2Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 3.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        Rectangle {
            id: fret3Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 4.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        Rectangle {
            id: fret4Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 5.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        Rectangle {
            id: fret5Cursor
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: parent.height/7 * 6.5
            height: 5
            color: "black"
            opacity: 0.5
        }

        ListView {
            id: tablatureView
            anchors.fill: parent
            model: noteModel
            delegate: noteDelegate
            orientation: ListView.Horizontal

            header:
                Rectangle{
                //color: "green"
                //height: parent.height
                width: applicationWindow.width/4 + cursor.width
            }
            footer:
                Rectangle{
                //color: "green"
                //height: parent.height
                width: applicationWindow.width - applicationWindow.width/4
            }
        }
    }

    ListModel {
        id: noteModel

        ListElement {
            name: ""
            timeStart: 15.375
            timeEnd: 15.593
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 7
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 15.593
            timeEnd: 15.703
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 255
            fret5: 10
        }
        ListElement {
            name: ""
            timeStart: 15.703
            timeEnd: 15.812
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 255
            fret5: 7
        }
        ListElement {
            name: ""
            timeStart: 15.812
            timeEnd: 15.921
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 10
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 15.921
            timeEnd: 16.031
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 7
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.031
            timeEnd: 16.141
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 9
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.141
            timeEnd: 16.25
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 7
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.25
            timeEnd: 16.36
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 10
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.36
            timeEnd: 16.468
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 255
            fret4: 7
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.468
            timeEnd: 16.579
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 9
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.579
            timeEnd: 16.687
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 7
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.687
            timeEnd: 16.798
            fret0: 255
            fret1: 255
            fret2: 9
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.798
            timeEnd: 16.906
            fret0: 255
            fret1: 255
            fret2: 7
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 16.906
            timeEnd: 17.017
            fret0: 255
            fret1: 9
            fret2: 255
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 17.017
            timeEnd: 17.125
            fret0: 255
            fret1: 7
            fret2: 255
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: ""
            timeStart: 17.125
            timeEnd: 20.538
            fret0: 255
            fret1: 5
            fret2: 255
            fret3: 255
            fret4: 255
            fret5: 255
        }
        // A3
        ListElement {
            name: "A3"
            timeStart: 20.538
            timeEnd: 20.75
            fret0: 255
            fret1: 4
            fret2: 7
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: "A3"
            timeStart: 20.75
            timeEnd: 23.927
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 4
            fret4: 7
            fret5: 255
        }
        // B5
        ListElement {
            name: "B5"
            timeStart: 23.927
            timeEnd: 24.146
            fret0: 255
            fret1: 2
            fret2: 4
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: "B5"
            timeStart: 24.146
            timeEnd: 27.385
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 2
            fret4: 4
            fret5: 255
        }
        // A5
        ListElement {
            name: "A5"
            timeStart: 27.385
            timeEnd: 27.600
            fret0: 255
            fret1: 0
            fret2: 2
            fret3: 255
            fret4: 255
            fret5: 255
        }
        ListElement {
            name: "A5"
            timeStart: 27.600
            timeEnd: 30.239
            fret0: 255
            fret1: 255
            fret2: 255
            fret3: 0
            fret4: 2
            fret5: 255
        }
    }


}
