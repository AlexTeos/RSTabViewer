import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import Qt.labs.folderlistmodel 1.0
Page {
    function secsToMinAndSecs(totalSeconds){
        var date = new Date(1970,0,1);
        date.setSeconds(totalSeconds);
        return date.toTimeString().replace(/.*(\d{2}:\d{2}).*/, "$1");
    }

    Component.onCompleted: libraryView.currentIndex = -1

    Rectangle{
        anchors.fill: parent
        color: "#f2d0ea"

        ListView {
            id: libraryView
            anchors.fill: parent
            model: songs
            delegate: songDelegate
            orientation: ListView.Vertical
            spacing: 5
        }

        Component {
            id: songDelegate
            Rectangle {
                color: "#e594eb"
                width: libraryView.width
                height: 30
                radius: 20
                opacity: 1 - (index !== libraryView.currentIndex) * 0.5

                Text {
                    text: songName + "-" + artistName + " ["+ secsToMinAndSecs(duration) + "] ("+ albumName + "/"+ songYear + ")"
                    anchors.centerIn: parent
                    font.pixelSize: 20
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(libraryView.currentIndex !== index){
                            libraryView.currentIndex = index
                            playMusic.source = "file:" + trackTeaser
                            playMusic.play()
                        } else {
                            songs.setTablature(index, 1);
                            playMusic.stop()
                            playMusic.source = "file:" + track
                            mainSwipeView.setCurrentIndex(kSongPage)
                        }
                    }
                }
            }
        }
    }
}
