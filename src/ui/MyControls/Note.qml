import QtQuick 2.11
import QtQuick.Controls 2.2

Rectangle {
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
        width: (tremolo || vibrato) ? parent.width : Math.min(parent.width,
                                                              height)
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: (slide || unpitchedSlide) ? undefined : parent.left
        anchors.right: (slide || unpitchedSlide) ? parent.right : undefined

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
                    else if (unpitchedSlide)
                        "qrc:/effects/slideup.png"
                    else if (tremolo)
                        "qrc:/effects/tremolo.png"
                    else if (vibrato)
                        "qrc:/effects/vibrato.png"
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
        visible: (mute || harmonic || hammerOn || pullOff || slide
                  || unpitchedSlide || tremolo || vibrato)
    }

    function calcTextForNote() {
        var noteText = frets[index].toString()
        if (noteText.length !== 1)
            noteText = noteText.substring(0, 1) + " " + noteText.substr(1, 2)

        return noteText
    }

    Text {
        id: fretText
        anchors.top: parent.top
        height: parent.height
        anchors.horizontalCenter: sustain ? undefined : parent.horizontalCenter
        anchors.left: sustain ? parent.left : undefined
        anchors.leftMargin: text.length == 1 ? height / 15 : 0

        text: calcTextForNote()
        font.wordSpacing: -parent.height * 0.3
        font.pixelSize: parent.height * 0.75
        color: "white"
        style: Text.Outline
        styleColor: Qt.darker(parent.color, 1.5)
    }
}
