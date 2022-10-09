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
        anchors.fill: parent
        anchors.margins: parent.height / 25
        Image {
            anchors.fill: parent
            source: if (mute)
                        "qrc:/effects/mute.png"
                    else if (harmonic)
                        "qrc:/effects/harmonic.png"
                    else if (hammerOn)
                        "qrc:/effects/hammeron.png"
                    else
                        //if (pullOff)
                        "qrc:/effects/pulloff.png"
            opacity: 0.7
        }
        color: "transparent"
        visible: (mute || harmonic || hammerOn || pullOff)
    }

    function calcTextForNote() {
        var noteText = frets[index].toString()
        if (noteText.length !== 1)
            noteText = noteText.substring(0, 1) + " " + noteText.substr(1, 2)

        if (slide) {
            if (frets[index] > nextFrets[index])
                noteText += " ↘ "
            else
                noteText += " ↗ "
        }

        if (unpitchedSlide)
            noteText += " ↘ "

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
