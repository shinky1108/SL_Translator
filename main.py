# 출처 - https://github.com/ddd4117/GUI/blob/master/src/camera_test.py
# 수정 - webnautes
import os
import cv2
import sys
import time
from PyQt5 import QtCore
from PyQt5 import QtWidgets
from PyQt5 import QtGui
from nets import inception
from preprocessing import inception_preprocessing
from matplotlib import pyplot as plt
import numpy as np
import tensorflow as tf
import urllib.request #표준 라이브러리
import json
import glob

#checkpoints_dir = 'C:\\SL\\SignLanguage2\\train_inception_v1_SignLanguage2_FineTune_logs\\all'
#checkpoints_dir = 'C:\\SL\\SignLanguage2\\train_inception_v1_SignLanguage2_FineTune_logs\\all2'
#checkpoints_dir = 'C:\\SL\\SignLanguage2\\train_inception_v1_SignLanguage2_FineTune_logs\\all3'
checkpoints_dir = 'C:\\SignLanguage2\\train_inception_v1_SignLanguage2_FineTune_logs\\all4'
TestImage = './images/1.jpg'

class ShowVideo(QtCore.QObject):
    num = 1
    flag = 0
    is_run = True
    camera = cv2.VideoCapture(1)
    print(camera)
    timer = 3
    text = ""

    ret, image = camera.read()
    height , width = image.shape[:2]

    VideoSignal1 = QtCore.pyqtSignal(QtGui.QImage)

    def __init__(self, parent=None):
        super(ShowVideo, self).__init__(parent)

    @QtCore.pyqtSlot()
    def startVideo(self):
        global image
        global text
        text = ""

        while True:
            ret, image = self.camera.read()
            color_swapped_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            cv2.putText(color_swapped_image, text, (300, 240), cv2.FONT_HERSHEY_PLAIN, 5.0, (255, 0, 0), 5);
            qt_image1 = QtGui.QImage(color_swapped_image.data,
                                    self.width,
                                    self.height,
                                    color_swapped_image.strides[0],
                                    QtGui.QImage.Format_RGB888)
            self.VideoSignal1.emit(qt_image1)

            loop = QtCore.QEventLoop()
            QtCore.QTimer.singleShot(1, loop.quit) #25 ms
            loop.exec_()

    @QtCore.pyqtSlot()
    def save(self):
        global image
        global is_run
        global num
        global text

        is_run = True
        num = 1
        timer = 3

        if not os.path.isdir("image"):
            os.mkdir("image")

        while is_run:
            text = str(timer)
            timer = timer - 1

            if timer == 0:
                for i in range(1, 4):
                    path = "image/" + str(num) + ".jpg"
                    cv2.imwrite(path, image)
                    num = num + 1
                    time.sleep(0.1)
                timer = 3
            loop = QtCore.QEventLoop()
            QtCore.QTimer.singleShot(1000, loop.quit)  # 25 ms
            loop.exec_()

    @QtCore.pyqtSlot()
    def stop(self):
        global is_run
        global num
        global text

        text = ""
        is_run = False
        self.Con_Image()

    @QtCore.pyqtSlot()
    def Api_trans(self):
        global num
        j = 1
        client_id = "vIj2xvkAVXo8ArNz48NH"  # 개발자센터에서 발급받은 Client ID 값
        client_secret = "xZAQ6OTeHy"  # 개발자센터에서 발급받은 Client Secret 값
        # 요청할 주소
        url = "https://openapi.naver.com/v1/papago/n2mt"

        #이미지 평가 코드
        slim = tf.contrib.slim

        image_size = inception.inception_v1.default_image_size

        for i in range(1, num) :
            with tf.Graph().as_default():
                image_input = tf.read_file('./images/'+str(i)+'.jpg')
                image = tf.image.decode_jpeg(image_input, channels=3)
                processed_image = inception_preprocessing.preprocess_image(image,
                                                                           image_size,
                                                                           image_size,
                                                                           is_training=False)
                processed_images = tf.expand_dims(processed_image, 0)

                with slim.arg_scope(inception.inception_v1_arg_scope()):
                    logits, _ = inception.inception_v1(processed_images, num_classes=10, is_training=False)
                probabilities = tf.nn.softmax(logits)

                init_fn = slim.assign_from_checkpoint_fn(
                    # os.path.join(checkpoints_dir, 'model.ckpt-500'),
                    # os.path.join(checkpoints_dir, 'model.ckpt-1000'),
                    # os.path.join(checkpoints_dir, 'model.ckpt-5000'),
                    os.path.join(checkpoints_dir, 'model.ckpt-5000'),

                    slim.get_model_variables('InceptionV1'))

                with tf.Session() as sess:
                    init_fn(sess)
                    np_image, probabilities = sess.run([image, probabilities])
                    probabilities = probabilities[0, 0:]
                    sorted_inds = [i[0] for i in sorted(enumerate(-probabilities), key=lambda x: x[1])]

                #plt.figure()
                #plt.imshow(np_image.astype(np.uint8))
                #plt.axis('off')
                #plt.show()

                names = os.listdir("C:\\SignLanguage2\\SignLanguage2_photos")
                # F:\\SL\SignLanguage3\\SignLanguage_photos
                #for i in range(5):
                index = sorted_inds[0]
                print(str(j) + '번째 결과 : [%s] => %0.2f%%  ' % (names[index], probabilities[index]))
                if (i % 3) == 0:
                    j = j+1
                #self.edit_text.setText(self.edit_text.toPlainText() + "," + names[index])
                #edit_text.setText("aa")

        # 번역할 언어와 내용
        text = input("번역할 내용을 입력하세요: ")
        encText = urllib.parse.quote(text)
        srcLang = 'en'
        tarLang = 'ko'
        rq_data = "source={}&target={}&text=".format(srcLang, tarLang) + encText

        # 웹 요청
        request = urllib.request.Request(url)
        request.add_header("X-Naver-Client-Id", client_id)
        request.add_header("X-Naver-Client-Secret", client_secret)

        # 결과 받아오는 부분
        response = urllib.request.urlopen(request, data=rq_data.encode("utf-8"))

        # 응답 성공적일 때
        rescode = response.getcode()
        if (rescode == 200):  # 성공
            response_body = response.read()
            rs_data = response_body.decode('utf-8')
            rs_data = json.loads(rs_data)  # 딕셔너리화
            trans_text = rs_data['message']['result']['translatedText']
            print("번역된 내용: ", trans_text)
            return trans_text
        else:  # 실패
            print("Error Code:" + rescode)

    @QtCore.pyqtSlot()
    def reset(self):
        global num
        for i in range(1, num):
            path = "image/" + str(i) + ".jpg"
            if os.path.isfile(path):
                os.remove(path)


    def Con_Image(self):
        img_Path = './image/'
        img_Ext = '*.jpg'
        save_Path = "./images/"
        images = []
        i = 1

        for a in glob.glob(os.path.join(img_Path, img_Ext)):
            images.append(a)
        cnt = len(images)

        if not os.path.isdir("images"):
            os.mkdir("images")

        if cnt == 0:
            print("폴더 내의 파일이 없습니다.")

        else:
            for image in images:
                path = "./images/" + str(i) + ".jpg"
                img = cv2.imread(image, 0)
                ret, thr = cv2.threshold(img, 127, 255, cv2.THRESH_TOZERO)
                cv2.imwrite(save_Path + str(i) + ".jpg", thr)
                i = i + 1

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
    edit_text = QtWidgets.QTextEdit('Result')

    push_button1.clicked.connect(vid.save)
    push_button2.clicked.connect(vid.stop)
    push_button3.clicked.connect(vid.Api_trans)
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
    vertical_layout.addWidget(edit_text)

    layout_widget = QtWidgets.QWidget()
    layout_widget.setLayout(vertical_layout)

    main_window = QtWidgets.QMainWindow()
    main_window.setCentralWidget(layout_widget)
    main_window.show()
    vid.startVideo()

    sys.exit(app.exec_())