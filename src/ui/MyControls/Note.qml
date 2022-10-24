import QtQuick 2.11
import QtQuick.Controls 2.2

Rectangle {
    id: note
    border.width: height / 50
    border.color: Qt.darker(color, 1.5)

    Rectangle {
        id: squareRect
        anchors.fill: parent
        anchors.rightMargin: parentNote ? 0 : parent.radius
        anchors.leftMargin: childNote ? 0 : parent.radius

        color: parent.color
    }

    Rectangle {
        height: parent.height
        width: (tremolo || vibrato) ? parent.width : Math.min(fretText.width,
                                                              height)
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left

        Image {
            anchors.fill: parent

            sourceSize.width: parent.height
            fillMode: (tremolo
                       || vibrato) ? Image.TileHorizontally : Image.Stretch
            source: if (mute)
                        "qrc:/effects/mute.png"
                    else if (harmonic)
                        "qrc:/effects/harmonic.png"
                    else if (hammerOn)
                        "qrc:/effects/hammeron.png"
                    else if (pullOff)
                        "qrc:/effects/pulloff.png"
                    else if (tremolo)
                        "qrc:/effects/tremolo.png"
                    else if (vibrato)
                        "qrc:/effects/vibrato.png"
                    else if (accent)
                        "qrc:/effects/accent.png"
                    else if (pinchHarmonic)
                        "qrc:/effects/pinchharmonic.png"
                    else if (palmMute)
                        "qrc:/effects/palmmute.png"
                    else
                        "qrc:/effects/empty.png"
            opacity: 0.7
        }
        color: "transparent"
        visible: (mute || harmonic || hammerOn || pullOff || tremolo || vibrato
                  || accent || pinchHarmonic || palmMute)
    }

    Rectangle {
        height: parent.height
        width: Math.min(fretText.width, height)
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        Image {
            anchors.fill: parent

            sourceSize.width: parent.height
            source: if (unpitchedSlide)
                        "qrc:/effects/slideup.png"
                    else if (slide) {
                        if (frets[index] > nextFrets[index])
                            "qrc:/effects/slidedown.png"
                        else
                            "qrc:/effects/slideup.png"
                    } else
                        "qrc:/effects/empty.png"
            opacity: 0.7
        }
        color: "transparent"
        visible: (slide || unpitchedSlide)
    }

    function calcTextForNote() {
        var noteText = frets[index].toString()
        if (noteText.length !== 1)
            noteText = noteText.substring(0, 1) + " " + noteText.substr(1, 2)

        return noteText
    }

    Row {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: parent.height / 2
        width: parent.width
        anchors.leftMargin: bendStarts[0] * pixelsPerSecond
        visible: bend
        opacity: 0.8
        Repeater {
            model: bendSteps.length
            delegate: Item {
                height: parent.height
                width: (index == bendSteps.length
                        - 1) ? parent.width - bendStarts[index]
                               * pixelsPerSecond : (bendStarts[index + 1]
                                                    - bendStarts[index]) * pixelsPerSecond
                Rectangle {
                    anchors.top: parent.top
                    height: parent.height / 4
                    width: parent.width

                    color: "white"
                    visible: bendSteps[index] !== 0
                }
                Image {
                    anchors.bottom: parent.bottom
                    height: parent.height * 0.75

                    fillMode: Image.PreserveAspectFit
                    source: if (bendSteps[index] === 1)
                                "qrc:/effects/bend_1.png"
                            else if (bendSteps[index] === 2)
                                "qrc:/effects/bend_2.png"
                            else if (bendSteps[index] === 3)
                                "qrc:/effects/bend_3.png"
                            else
                                "qrc:/effects/empty.png"
                }
            }
        }
    }

    Text {
        id: fretText
        anchors.top: parent.top
        height: parent.height
        anchors.horizontalCenter: sustain ? undefined : parent.horizontalCenter
        anchors.left: sustain ? parent.left : undefined
        width: height / 1.3

        horizontalAlignment: Text.AlignHCenter
        text: calcTextForNote()
        font.wordSpacing: -parent.height * 0.3
        font.pixelSize: parent.height * 0.75
        color: "white"
        style: Text.Outline
        styleColor: Qt.darker(parent.color, 1.5)
        visible: frets[index]
    }
}
