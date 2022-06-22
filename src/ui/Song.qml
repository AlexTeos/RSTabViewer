import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

Page {
    property real musicPositionInPixels: tablatureView.contentX
    property real pixelsPerSecond: 200

    function recalculatePositionAfterMovement(){
        musicPositionInPixels = tablatureView.contentX
        playMusic.seek((musicPositionInPixels + (applicationWindow.width/4 + cursor.width)) * 1000 / pixelsPerSecond)
    }

    Timer {
        id: flickTimer
        interval: 25
        running: false
        repeat: true
        onTriggered: {
            musicPositionInPixels += pixelsPerSecond * interval / 1000
            tablatureView.contentX = musicPositionInPixels
        }
    }

    Audio {
       id: playMusic
       autoLoad: true
       source: "123732889.ogg"
    }

    Button{
        id:startButton
        anchors.left: parent.left
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'start'

        onClicked: {
            playMusic.play()
            flickTimer.start()
        }
    }

    Button{
        id:stopButton
        anchors.right: parent.right
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'stop'

        onClicked: {
            playMusic.pause()
            flickTimer.stop()
        }
    }

    Rectangle{
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: applicationWindow.height/2
        color: "#f2d0ea"

        Component {
            id: noteDelegate
            Rectangle {
                color: "#e594eb"
                width: duration * pixelsPerSecond
                height: applicationWindow.height/2
                opacity: 0.75
                radius: 20
                Column {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.left

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
                            text: frets[0]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[0] !== 0xFF ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: frets[1]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[1] !== 0xFF ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: frets[2]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[2] !== 0xFF ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: frets[3]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[3] !== 0xFF ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: frets[4]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[4] !== 0xFF ? 100 : 0
                    }

                    Rectangle{
                        height: parent.height/7
                        width: height
                        radius: width/2
                        color: "white"
                        Text {
                            text: frets[5]
                            anchors.centerIn: parent
                            font.pixelSize: parent.height
                            color: "black"
                            }
                        opacity: frets[5] !== 0xFF ? 100 : 0
                    }
                }
            }
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
            model: tablature
            delegate: noteDelegate
            orientation: ListView.Horizontal
            cacheBuffer:1000

            onMovementStarted:{
                stopButton.clicked()
            }
            onFlickStarted:{
                stopButton.clicked()
            }
            onMovementEnded:{
                recalculatePositionAfterMovement()
                startButton.clicked()
            }
            onFlickEnded:{
                recalculatePositionAfterMovement()
                startButton.clicked()
            }

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
    }
}
