import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import Qt.labs.folderlistmodel 1.0
Page {
    property var sngTypes: ["Bass", "Lead", "Rhythm", "Vocals", "Combo", "Showlights"];

    Component.onCompleted: libraryView.currentIndex = -1

    Rectangle{
        anchors.fill: parent
        color: AppSettings.backgroundColor

        GridView {
            id: libraryView
            anchors.fill: parent

            cellWidth: parent.width / Math.trunc(parent.width / 128)
            cellHeight: cellWidth + 32
            model: songs
            delegate: songDelegate
        }

        Component {
            id: songDelegate
            Item{
                width: libraryView.cellWidth
                height: libraryView.cellHeight

                Column {
                    anchors.fill: parent

                    Image {
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: 128
                        width: 128

                        fillMode: Image.PreserveAspectFit
                        source: "file:" + albumImage

                        Column {
                            anchors.fill: parent
                            Repeater{
                                model: instruments
                                delegate: Item {
                                    height:parent.height / 3
                                    width:parent.width
                                    Rectangle {
                                        id: instrumentButton
                                        anchors.fill: parent

                                        color:"#111111"
                                        opacity: 0.75

                                        border.color: "black"
                                        border.width: 1

                                        gradient: Gradient {
                                            GradientStop { position: 0.0; color: instrumentButton.color }
                                            GradientStop { position: 0.5; color: Qt.lighter(instrumentButton.color, 3) }
                                            GradientStop { position: 1.0; color: instrumentButton.color }
                                        }
                                    }
                                    Text {
                                        anchors.centerIn: parent

                                        text: sngTypes[instruments[index]]
                                        color: "white"
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            songs.setTablature(libraryView.currentIndex, instruments[index]);
                                            playMusic.stop()
                                            playMusic.source = "file:" + track
                                            mainSwipeView.setCurrentIndex(kSongPage)
                                        }
                                    }
                                }
                            }
                            visible: index === libraryView.currentIndex
                        }
                    }

                    Rectangle {
                        id: songTitle
                        width:128
                        height:32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#333333"
                        Column {
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter

                                maximumLineCount: 1
                                wrapMode: Text.WrapAnywhere
                                text: artistName
                                font.pixelSize: 10
                                color: AppSettings.textColor
                            }
                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter

                                maximumLineCount: 1
                                wrapMode: Text.WrapAnywhere
                                text: songName
                                font.pixelSize: 10
                                color: AppSettings.textColor
                            }
                        }

                        gradient: Gradient {
                            GradientStop { position: 0.0; color: songTitle.color }
                            GradientStop { position: 0.5; color: Qt.lighter(songTitle.color, 1.5) }
                            GradientStop { position: 1.0; color: songTitle.color }
                        }
                    }
                }

                Rectangle {
                    color: "black"
                    anchors.fill: parent
                    border.width: 5
                    opacity: if(libraryView.currentIndex == -1)
                                 0
                             else
                                (index !== libraryView.currentIndex) * 0.5
                }

                MouseArea {
                    anchors.fill: parent
                    z:-1
                    onClicked: {
                        if(libraryView.currentIndex !== index){
                            libraryView.currentIndex = index
                            playMusic.source = "file:" + trackTeaser
                            playMusic.play()

                            songDurationT = duration
                            songNameT = songName
                            artistNameT = artistName
                            albumNameT = albumName
                            songYearT = songYear
                            albumImageT = "file:" + albumImage
                        } else {
                            libraryView.currentIndex = -1
                            playMusic.stop()
                        }
                    }
                }
            }
        }
    }
}
