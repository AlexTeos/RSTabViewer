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

    Component.onCompleted: {
        AppSettings.wWidth = Qt.binding(function() {return width})
        AppSettings.wHeight = Qt.binding(function() {return height})
    }

    SwipeView {
          id: swipeView
          anchors.fill: parent

          interactive: true

          Library{
              id: libraryPage
          }

          Song{
              id: songPage
          }
      }
}
