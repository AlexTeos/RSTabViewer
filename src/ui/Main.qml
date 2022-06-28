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

    property int kLibraryPage         : 0
    property int kSongPage            : 1

    Component.onCompleted: {
        AppSettings.wWidth = Qt.binding(function() {return width})
        AppSettings.wHeight = Qt.binding(function() {return height})
    }

    Audio {
       id: playMusic
       autoLoad: true
    }

    SwipeView {
          id: mainSwipeView
          anchors.fill: parent

          interactive: false

          Library{
              id: libraryPage
          }

          Song{
              id: songPage
          }
      }
}
