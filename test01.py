# 출처 - https://github.com/ddd4117/GUI/blob/master/src/camera_test.py
# 수정 - webnautes
import os
import cv2
import sys
from PyQt5 import QtCore
from PyQt5 import QtWidgets
from PyQt5 import QtGui


class ShowVideo(QtCore.QObject):

    num = 1
    flag = 0
    is_run = True
    camera = cv2.VideoCapture(0)

    ret, image = camera.read()
    height , width = image.shape[:2]

    VideoSignal1 = QtCore.pyqtSignal(QtGui.QImage)

    def __init__(self, parent=None):
        super(ShowVideo, self).__init__(parent)

    @QtCore.pyqtSlot()
    def startVideo(self):
        global image

        while True:
            ret, image = self.camera.read()
            color_swapped_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

            qt_image1 = QtGui.QImage(color_swapped_image.data,
                                    self.width,
                                    self.height,
                                    color_swapped_image.strides[0],
                                    QtGui.QImage.Format_RGB888)
            self.VideoSignal1.emit(qt_image1)

            loop = QtCore.QEventLoop()
            QtCore.QTimer.singleShot(25, loop.quit) #25 ms
            loop.exec_()

    @QtCore.pyqtSlot()
    def save(self):
        global image
        global is_run
        global num

        is_run = True
        num = 1

        if not os.path.isdir("image"):
            os.mkdir("image")

        while is_run:
            path = "image/" + str(num) + ".jpg"
            cv2.imwrite(path, image)
            num = num + 1

            loop = QtCore.QEventLoop()
            QtCore.QTimer.singleShot(25, loop.quit)  # 25 ms
            loop.exec_()

    @QtCore.pyqtSlot()
    def stop(self):
        global is_run
        global num
        is_run = False

    @QtCore.pyqtSlot()
    def reset(self):
        global num
        for i in range(1, num):
            path = "image/" + str(i) + ".jpg"
            if os.path.isfile(path):
                os.remove(path)

class ImageViewer(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(ImageViewer, self).__init__(parent)
        self.image = QtGui.QImage()
        self.setAttribute(QtCore.Qt.WA_OpaquePaintEvent)

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.drawImage(0, 0, self.image)
        self.image = QtGui.QImage()

    def initUI(self):
        self.setWindowTitle('Test')

    @QtCore.pyqtSlot(QtGui.QImage)
    def setImage(self, image):
        if image.isNull():
            print("Viewer Dropped frame!")

        self.image = image
        if image.size() != self.size():
            self.setFixedSize(image.size())
        self.update()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)

    thread = QtCore.QThread()
    thread.start()
    vid = ShowVideo()
    vid.moveToThread(thread)

    image_viewer1 = ImageViewer()

    vid.VideoSignal1.connect(image_viewer1.setImage)

    push_button1 = QtWidgets.QPushButton('Start')
    push_button2 = QtWidgets.QPushButton('Stop')
    push_button3 = QtWidgets.QPushButton('Translation')
    push_button4 = QtWidgets.QPushButton('Reset')

    push_button1.clicked.connect(vid.save)
    push_button2.clicked.connect(vid.stop)
    push_button4.clicked.connect(vid.reset)

    vertical_layout = QtWidgets.QVBoxLayout()
    horizontal_layout = QtWidgets.QHBoxLayout()
    vertical_layout2 = QtWidgets.QVBoxLayout()
    horizontal_layout.addWidget(image_viewer1)
    vertical_layout.addLayout(horizontal_layout)
    horizontal_layout.addLayout(vertical_layout2)
    vertical_layout2.addWidget(push_button1)
    vertical_layout2.addWidget(push_button2)
    vertical_layout2.addWidget(push_button3)
    vertical_layout2.addWidget(push_button4)

    layout_widget = QtWidgets.QWidget()
    layout_widget.setLayout(vertical_layout)

    main_window = QtWidgets.QMainWindow()
    main_window.setCentralWidget(layout_widget)
    main_window.show()
    vid.startVideo()

    sys.exit(app.exec_())