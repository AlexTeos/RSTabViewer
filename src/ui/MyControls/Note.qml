import QtQuick 2.11
import QtQuick.Controls 2.2

Rectangle {
    id: note
    border.width: height / 50
    border.color: Qt.darker(color, 1.5)

    property int chordFlag: 0x00000002
    property int fretHandMuteFlag: 0x00000008
    property int tremoloFlag: 0x00000010
    property int harmonicFlag: 0x00000020
    property int palmMuteFlag: 0x00000040
    property int hammerOnFlag: 0x00000200
    property int pullOffFlag: 0x00000400
    property int slideFlag: 0x00000800
    property int bendFlag: 0x00001000
    property int sustainFlag: 0x00002000
    property int pinchHarmonicFlag: 0x00008000
    property int vibratoFlag: 0x00010000
    property int muteFlag: 0x00020000
    property int unpitchedSlideFlag: 0x00400000
    property int singleFlag: 0x00800000
    property int chordNotesFlag: 0x01000000
    property int accentFlag: 0x04000000
    property int parentFlag: 0x08000000
    property int childFlag: 0x10000000

    property var effects

    function isChord(m) {
        return m & chordFlag
    }
    function isTremolo(m) {
        return m & tremoloFlag
    }
    function isHarmonic(m) {
        return m & harmonicFlag
    }
    function isPalmMute(m) {
        return m & palmMuteFlag
    }
    function isHammerOn(m) {
        return m & hammerOnFlag
    }
    function isPullOff(m) {
        return m & pullOffFlag
    }
    function isSlide(m) {
        return m & slideFlag
    }
    function isBend(m) {
        return m & bendFlag
    }
    function isSustain(m) {
        return m & sustainFlag
    }
    function isPinchHarmonic(m) {
        return m & pinchHarmonicFlag
    }
    function isVibrato(m) {
        return m & vibratoFlag
    }
    function isMute(m) {
        return m & muteFlag || m & fretHandMuteFlag
    }
    function isUnpitchedSlide(m) {
        return m & unpitchedSlideFlag
    }
    function isSingle(m) {
        return m & unpitchedSlideFlag
    }
    function isChordNotes(m) {
        return m & chordNotesFlag
    }
    function isAccent(m) {
        return m & accentFlag
    }
    function isParent(m) {
        return m & parentFlag
    }
    function isChild(m) {
        return m & childFlag
    }

    Rectangle {
        id: squareRect
        anchors.fill: parent
        anchors.rightMargin: isParent(effects) ? 0 : parent.radius
        anchors.leftMargin: isChild(effects) ? 0 : parent.radius

        color: parent.color
    }

    Rectangle {
        height: parent.height
        width: (isTremolo(effects) || isVibrato(
                    effects)) ? parent.width : Math.min(fretText.width, height)
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left

        Image {
            anchors.fill: parent

            sourceSize.width: parent.height
            fillMode: (isTremolo(effects) || isVibrato(
                           effects)) ? Image.TileHorizontally : Image.Stretch
            source: if (isMute(effects))
                        "qrc:/effects/mute.png"
                    else if (isHarmonic(effects))
                        "qrc:/effects/harmonic.png"
                    else if (isHammerOn(effects))
                        "qrc:/effects/hammeron.png"
                    else if (isPullOff(effects))
                        "qrc:/effects/pulloff.png"
                    else if (isTremolo(effects))
                        "qrc:/effects/tremolo.png"
                    else if (isVibrato(effects))
                        "qrc:/effects/vibrato.png"
                    else if (isAccent(effects))
                        "qrc:/effects/accent.png"
                    else if (isPinchHarmonic(effects))
                        "qrc:/effects/pinchharmonic.png"
                    else if (isPalmMute(effects))
                        "qrc:/effects/palmmute.png"
                    else
                        "qrc:/effects/empty.png"
            opacity: 0.7
        }
        color: "transparent"
        visible: (isMute(effects) || isHarmonic(effects) || isHammerOn(
                      effects) || isPullOff(effects) || isTremolo(
                      effects) || isVibrato(effects) || isAccent(
                      effects) || isPinchHarmonic(
                      effects) || isPalmMute(effects))
    }

    Rectangle {
        height: parent.height
        width: Math.min(fretText.width, height)
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right

        Image {
            anchors.fill: parent

            sourceSize.width: parent.height
            source: if (isUnpitchedSlide(effects))
                        "qrc:/effects/slidedown.png"
                    else if (isSlide(effects)) {
                        if (frets[index] > nextFrets[index])
                            "qrc:/effects/slidedown.png"
                        else
                            "qrc:/effects/slideup.png"
                    } else
                        "qrc:/effects/empty.png"
            opacity: 0.7
        }
        color: "transparent"
        visible: (isSlide(effects) || isUnpitchedSlide(effects))
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
        visible: isBend(effects)
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
