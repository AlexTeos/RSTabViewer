import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

ApplicationWindow {
    width: 800
    height: 360
    visible: true
    title: qsTr("Booktionary")
    id: applicationWindow

    property int kLibraryPage: 0
    property int kSongPage: 1

    function secsToMinAndSecs(totalSeconds) {
        var date = new Date(1970, 0, 1)
        date.setSeconds(totalSeconds)
        return date.toTimeString().replace(/.*(\d{2}:\d{2}).*/, "$1")
    }

    Component.onCompleted: {
        mainSwipeView.forceActiveFocus()
        AppSettings.wWidth = Qt.binding(function () {
            return width
        })
        AppSettings.wHeight = Qt.binding(function () {
            return height
        })
    }

    Audio {
        id: playMusic
    }

    property int songDurationT: 0
    property string songNameT: ""
    property string artistNameT: ""
    property string albumNameT: ""
    property string songYearT: ""
    property string albumImageT: ""

    SwipeView {
        id: mainSwipeView
        anchors.fill: parent

        interactive: false

        Library {
            id: libraryPage

            Keys.onReleased: {
                if (event.key === Qt.Key_Back) {
                    applicationWindow.close()
                    event.accepted = true
                }
            }
        }

        Song {
            id: songPage

            Keys.onReleased: {
                if (event.key === Qt.Key_Back) {
                    playMusic.stop()
                    mainSwipeView.setCurrentIndex(kLibraryPage)
                    event.accepted = true
                }
            }
        }
    }
}
