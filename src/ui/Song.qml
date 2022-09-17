import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

Page {
    property bool musicPlayed: false
    property real pixelsPerSecond: 100

    function recalculateMusicPositionAfterMovement() {
        playMusic.seek(
                    (tablatureView.contentX - tablatureView.originX) * 1000 / pixelsPerSecond)
    }

    function recalculateViewPositionAfterScale() {
        tablatureView.contentX = playMusic.position * pixelsPerSecond / 1000 + tablatureView.originX
    }

    function startPlay() {
        playMusic.play()
        flickTimer.start()
    }

    function pausePlay() {
        playMusic.pause()
        flickTimer.stop()
    }

    property int maxScale: 400
    property int minScale: 25
    property var stringColors: ["red", "yellow", "blue", "orange", "green", "purple"]

    Rectangle {
        anchors.fill: parent
        color: AppSettings.backgroundColor
    }

    Timer {
        id: flickTimer
        interval: 25
        running: false
        repeat: true
        onTriggered: {
            if (tablatureView.contentX - tablatureView.originX >= songDurationT * pixelsPerSecond)
                stop()
            else
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

    Item {
        height: 50
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left

        Rectangle {
            id: songHeader
            anchors.fill: parent
            color: "grey"

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "transparent"
                }
                GradientStop {
                    position: 0.5
                    color: Qt.lighter(songHeader.color, 0.75)
                }
                GradientStop {
                    position: 1.0
                    color: "transparent"
                }
            }
        }

        Rectangle {
            id: plusButton
            anchors.left: parent.left
            anchors.top: parent.top
            width: 50
            height: parent.height

            enabled: pixelsPerSecond < maxScale
            color: "transparent"
            border.width: 1
            border.color: "black"

            Image {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width
                source: "qrc:/icons/plus.png"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    pixelsPerSecond *= 2
                    recalculateViewPositionAfterScaleTimer.start()
                    parent.color = "black"
                    parent.opacity = 0.5
                    plusButtonTimer.start()
                }
            }

            Timer {
                id: plusButtonTimer
                interval: 100
                running: true
                onTriggered: {
                    parent.opacity = 1
                    parent.color = "transparent"
                }
            }
        }

        Rectangle {
            id: minusButton
            anchors.left: plusButton.right
            anchors.top: parent.top
            width: 50
            height: parent.height

            enabled: pixelsPerSecond > minScale
            color: "transparent"
            border.width: 1
            border.color: "black"

            Image {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width

                source: "qrc:/icons/minus.png"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    pixelsPerSecond /= 2
                    recalculateViewPositionAfterScaleTimer.start()
                    parent.color = "black"
                    parent.opacity = 0.5
                    minusButtonTimer.start()
                }
            }

            Timer {
                id: minusButtonTimer
                interval: 100
                running: true
                onTriggered: {
                    parent.opacity = 1
                    parent.color = "transparent"
                }
            }
        }

        Rectangle {
            id: stopButton
            anchors.right: parent.right
            anchors.top: parent.top
            width: 50
            height: parent.height

            color: "transparent"
            border.width: 1
            border.color: "black"

            Image {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width

                source: "qrc:/icons/stop.png"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    tablatureView.contentX = -(applicationWindow.width / 4 + cursor.width)
                    mainSwipeView.setCurrentIndex(kLibraryPage)
                    playMusic.stop()
                    flickTimer.stop()
                    parent.color = "black"
                    parent.opacity = 0.5
                    stopButtonTimer.start()
                }
            }

            Timer {
                id: stopButtonTimer
                interval: 100
                running: true
                onTriggered: {
                    parent.opacity = 1
                    parent.color = "transparent"
                }
            }
        }

        Rectangle {
            id: startButton
            anchors.right: stopButton.left
            anchors.top: parent.top
            width: 50
            height: parent.height

            color: "transparent"
            border.width: 1
            border.color: "black"
            visible: playMusic.playbackState !== Audio.PlayingState

            Image {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width

                source: "qrc:/icons/play.png"
            }

            MouseArea {
                id: playButtonArea
                anchors.fill: parent

                onClicked: {
                    startPlay()
                    parent.color = "black"
                    parent.opacity = 0.5
                    playButtonTimer.start()
                }
            }

            Timer {
                id: playButtonTimer
                interval: 100
                running: true
                onTriggered: {
                    parent.opacity = 1
                    parent.color = "transparent"
                }
            }
        }

        Rectangle {
            id: pauseButton
            anchors.right: stopButton.left
            anchors.top: parent.top
            width: 50
            height: parent.height

            color: "transparent"
            border.width: 1
            border.color: "black"
            visible: playMusic.playbackState === Audio.PlayingState

            Image {
                anchors.centerIn: parent
                width: parent.width / 2
                height: width

                source: "qrc:/icons/pause.png"
            }

            MouseArea {
                id: pauseButtonArea
                anchors.fill: parent

                onClicked: {
                    pausePlay()
                    parent.color = "black"
                    parent.opacity = 0.5
                    pauseButtonTimer.start()
                }
            }

            Timer {
                id: pauseButtonTimer
                interval: 100
                running: true
                onTriggered: {
                    parent.opacity = 1
                    parent.color = "transparent"
                }
            }
        }

        Text {
            anchors.top: parent.top
            anchors.centerIn: parent
            text: secsToMinAndSecs(
                      (tablatureView.contentX - tablatureView.originX)
                      / pixelsPerSecond) + "/" + secsToMinAndSecs(songDurationT)
            color: "white"
            font.pixelSize: parent.height / 1.5
        }
    }

    Item {
        height: 50
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Rectangle {
            id: songFooter
            anchors.fill: parent
            color: "grey"

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "transparent"
                }
                GradientStop {
                    position: 0.5
                    color: Qt.lighter(songHeader.color, 0.75)
                }
                GradientStop {
                    position: 1.0
                    color: "transparent"
                }
            }
        }

        Image {
            id: albumImageSongPage
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            height: parent.height
            width: height

            fillMode: Image.PreserveAspectFit
            source: albumImageT
        }
        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: albumImageSongPage.right
            height: parent.height
            anchors.leftMargin: 5

            Text {
                text: albumNameT
                color: "white"
                font.pixelSize: parent.height / 3
            }
            Text {
                text: songYearT
                color: "white"
                font.pixelSize: parent.height / 3
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            height: parent.height
            anchors.rightMargin: 5

            Text {
                anchors.right: parent.right

                text: songNameT
                color: "white"
                font.pixelSize: parent.height / 3
            }
            Text {
                anchors.right: parent.right

                text: artistNameT
                color: "white"
                font.pixelSize: parent.height / 3
            }
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: applicationWindow.height / 2

        Rectangle {
            id: decke
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 1.1

            color: "#333333"
            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "transparent"
                }
                GradientStop {
                    position: 0.05
                    color: decke.color
                }
                GradientStop {
                    position: 0.50
                    color: Qt.lighter(decke.color, 2)
                }
                GradientStop {
                    position: 0.95
                    color: decke.color
                }
                GradientStop {
                    position: 1.00
                    color: "transparent"
                }
            }
        }

        Component {
            id: noteDelegate
            Item {
                width: duration * pixelsPerSecond
                height: applicationWindow.height / 2
                z: tablatureView.count - index
                Column {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.left

                    Rectangle {
                        id: chord
                        height: parent.height / 7
                        width: height
                        radius: width / 5
                        color: "lightgrey"
                        Text {
                            text: name
                            anchors.centerIn: parent
                            font.pixelSize: parent.height / 2
                            color: "black"
                        }
                        opacity: name !== ""

                        gradient: Gradient {
                            GradientStop {
                                position: 0.0
                                color: chord.color
                            }
                            GradientStop {
                                position: 0.5
                                color: Qt.lighter(chord.color, 1.9)
                            }
                            GradientStop {
                                position: 1.0
                                color: chord.color
                            }
                        }
                    }

                    Repeater {
                        model: 6
                        delegate: Rectangle {
                            id: fret
                            height: parent.height / 7
                            width: height
                            radius: width / 5
                            color: stringColors[index]

                            gradient: Gradient {
                                GradientStop {
                                    position: 0.0
                                    color: fret.color
                                }
                                GradientStop {
                                    position: 0.5
                                    color: Qt.lighter(fret.color, 1.9)
                                }
                                GradientStop {
                                    position: 1.0
                                    color: fret.color
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                anchors.margins: 2
                                radius: width / 5
                                color: "black"
                                opacity: 0.5
                            }

                            Text {
                                text: frets[index]
                                anchors.centerIn: parent
                                font.pixelSize: parent.height
                                color: "white"
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
                id: string
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: parent.height / 7 * (1.5 + index)
                height: 5
                color: "grey"

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: string.color
                    }
                    GradientStop {
                        position: 0.5
                        color: Qt.lighter(string.color, 1.75)
                    }
                    GradientStop {
                        position: 1.0
                        color: string.color
                    }
                }
            }
        }

        ListView {
            id: tablatureView
            anchors.fill: parent
            model: tablature
            delegate: noteDelegate
            orientation: ListView.Horizontal
            // cacheBuffer must be more than song length in pixels (duration * pixelsPerSecond)
            // otherwise scale buttons will be buggy
            cacheBuffer: 1000000
            clip: true

            onMovementStarted: {
                musicPlayed |= (playMusic.playbackState === Audio.PlayingState)
                pausePlay()
            }
            onFlickStarted: {
                musicPlayed |= (playMusic.playbackState === Audio.PlayingState)
                pausePlay()
            }
            onMovementEnded: {
                recalculateMusicPositionAfterMovement()
                if (musicPlayed)
                    startPlay()
                musicPlayed = false
            }
            onFlickEnded: {
                recalculateMusicPositionAfterMovement()
                if (musicPlayed)
                    startPlay()
                musicPlayed = false
            }

            header: Rectangle {
                width: applicationWindow.width / 4 + cursor.width
            }
            footer: Rectangle {
                width: applicationWindow.width - applicationWindow.width / 4
            }
        }

        Rectangle {
            id: cursor
            height: parent.height * 1.3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 4
            width: 5

            color: "green"
            opacity: 0.75

            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "transparent"
                }
                GradientStop {
                    position: 0.05
                    color: cursor.color
                }
                GradientStop {
                    position: 0.95
                    color: cursor.color
                }
                GradientStop {
                    position: 1.00
                    color: "transparent"
                }
            }
        }
    }
}
