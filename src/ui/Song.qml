import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

Page {
    property bool musicPlayed: false
    property real pixelsPerSecond: 100

    function recalculateMusicPositionAfterMovement(){
        playMusic.seek((tablatureView.contentX - tablatureView.originX) * 1000 / pixelsPerSecond)
    }

    function recalculateViewPositionAfterScale(){
        tablatureView.contentX = playMusic.position * pixelsPerSecond / 1000 + tablatureView.originX
    }

    property int maxScale:      400
    property int minScale:      100

    Timer {
        id: flickTimer
        interval: 25
        running: false
        repeat: true
        onTriggered: {
            tablatureView.contentX += pixelsPerSecond * interval / 1000
        }
    }

    Timer {
        id: recalculateViewPositionAfterScaleTimer
        interval: 10
        running: false
        repeat: false
        onTriggered: {
            recalculateViewPositionAfterScale()
        }
    }

    Button{
        id:plusButton
        anchors.right: minusButton.left
        anchors.top: parent.top
        width: 50
        height: 50
        text: '+'
        enabled: pixelsPerSecond < maxScale

        onClicked: {
            pixelsPerSecond *= 2
            recalculateViewPositionAfterScaleTimer.start()
        }
    }

    Button{
        id:minusButton
        anchors.right: parent.right
        anchors.top: parent.top
        width: 50
        height: 50
        text: '-'
        enabled: pixelsPerSecond > minScale

        onClicked: {
            pixelsPerSecond /= 2
            recalculateViewPositionAfterScaleTimer.start()
        }
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
        id:pauseButton
        anchors.left: startButton.right
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'pause'

        onClicked: {
            playMusic.pause()
            flickTimer.stop()
        }
    }

    Button{
        id:stopButton
        anchors.left: pauseButton.right
        anchors.top: parent.top
        width: 50
        height: 50
        text: 'stop'

        onClicked: {
            tablatureView.contentX = -(applicationWindow.width/4 + cursor.width)
            mainSwipeView.setCurrentIndex(kLibraryPage)
            playMusic.stop()
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

                    Repeater {
                       model: 6
                       delegate: Rectangle{
                           height: parent.height/7
                           width: height
                           radius: width/2
                           color: "white"
                           Text {
                               text: frets[index]
                               anchors.centerIn: parent
                               font.pixelSize: parent.height
                               color: "black"
                               }
                           opacity: frets[index] !== 0xFF ? 100 : 0
                       }
                    }
                }
            }
        }

        Repeater {
           model: 6
           delegate: Rectangle {
                   anchors.right: parent.right
                   anchors.left: parent.left
                   anchors.top: parent.top
                   anchors.topMargin: parent.height/7 * (1.5 + index)
                   height: 5
                   color: "black"
                   opacity: 0.5
            }
        }

        ListView {
            id: tablatureView
            anchors.fill: parent
            model: tablature
            delegate: noteDelegate
            orientation: ListView.Horizontal
            cacheBuffer: 1000000
            clip: true

            onMovementStarted:{
                musicPlayed |= (playMusic.playbackState === Audio.PlayingState)
                pauseButton.clicked()
            }
            onFlickStarted:{
                musicPlayed |= (playMusic.playbackState === Audio.PlayingState)
                pauseButton.clicked()
            }
            onMovementEnded:{
                recalculateMusicPositionAfterMovement()
                if(musicPlayed)
                    startButton.clicked()
                musicPlayed = false
            }
            onFlickEnded:{
                recalculateMusicPositionAfterMovement()
                if(musicPlayed)
                    startButton.clicked()
                musicPlayed = false
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
