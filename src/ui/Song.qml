import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import "MyControls"

Page {
    property bool musicPlayed: false
    property real pixelsPerSecond: 100
    property real tickPeriod: 20
    property real shiftPerTick: pixelsPerSecond * tickPeriod / 1000

    function recalculateViewPosition() {
        tablatureView.contentX = playMusic.position * pixelsPerSecond / 1000 + tablatureView.originX
    }

    function recalculateMusicPosition() {
        playMusic.seek(
                    (tablatureView.contentX - tablatureView.originX) * 1000 / pixelsPerSecond)
    }

    function startPlay() {
        playMusic.play()
        flickTimer.start()
    }

    function pausePlay() {
        playMusic.pause()
        flickTimer.stop()
    }

    property int maxScale: parent.width / 2
    property int minScale: parent.width / 32
    property var stringColors: ["crimson", "gold", "blue", "orange", "green", "purple"]

    Rectangle {
        anchors.fill: parent
        color: AppSettings.backgroundColor
    }

    Timer {
        id: flickTimer
        interval: tickPeriod
        running: false
        repeat: true
        onTriggered: {
            recalculateViewPosition()
            //if (tablatureView.contentX - tablatureView.originX >= songDurationT * pixelsPerSecond)
            //    stop()
            //else
            //    tablatureView.contentX += shiftPerTick
        }
    }

    Timer {
        id: recalculateViewPositionAfterScaleTimer
        interval: 10
        running: false
        repeat: false
        onTriggered: {
            recalculateViewPosition()
        }
    }

    Item {
        height: parent.height / 8
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
            height: parent.height
            width: height

            enabled: pixelsPerSecond < maxScale
            color: "transparent"
            border.width: width / 50
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
            height: parent.height
            width: height

            enabled: pixelsPerSecond > minScale
            color: "transparent"
            border.width: width / 50
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
            height: parent.height
            width: height

            color: "transparent"
            border.width: width / 50
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
                    tablatureView.contentX = -(parent.width / 4 + cursor.width)
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
            height: parent.height
            width: height

            color: "transparent"
            border.width: width / 50
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
            height: parent.height
            width: height

            color: "transparent"
            border.width: width / 50
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
        height: parent.height / 8
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
            anchors.leftMargin: 5

            Text {
                text: albumNameT
                color: "white"
                font.pixelSize: songFooter.height / 3
            }
            Text {
                text: songYearT
                color: "white"
                font.pixelSize: songFooter.height / 3
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 5

            Text {
                anchors.right: parent.right

                text: songNameT
                color: "white"
                font.pixelSize: songFooter.height / 3
            }
            Text {
                anchors.right: parent.right

                text: artistNameT
                color: "white"
                font.pixelSize: songFooter.height / 3
            }
        }
    }

    Item {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height / 1.7

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
                height: tablatureView.height
                z: tablatureView.count - index

                Item {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left

                    Repeater {
                        model: 6
                        delegate: Note {
                            height: parent.height / 6 - parent.height / 90
                            width: Math.max(height / 1.3,
                                            pixelsPerSecond * sustain)
                            y: parent.y + parent.height / 6 * index + parent.height / 180

                            effects: chordNote ? mask[index] : mask
                            radius: height / 5
                            color: stringColors[index]
                            visible: frets[index] !== 0xFF
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
                anchors.topMargin: parent.height / 6 * (0.5 + index)
                height: parent.height / 72
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
                recalculateMusicPosition()
                if (musicPlayed)
                    startPlay()
                musicPlayed = false
            }
            onFlickEnded: {
                recalculateMusicPosition()
                if (musicPlayed)
                    startPlay()
                musicPlayed = false
            }

            header: Rectangle {
                width: tablatureView.width / 4 + cursor.width
            }
            footer: Rectangle {
                width: tablatureView.width - tablatureView.width / 4
            }
        }

        Rectangle {
            id: cursor
            height: parent.height * 1.3
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: parent.width / 4
            width: parent.width / 160

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
