# 출처 - https://github.com/ddd4117/GUI/blob/master/src/camera_test.py
# 수정 - webnautes
import os
import cv2
import sys
import time
import re

from PIL import ImageFont, Image, ImageDraw
from PyQt5 import QtCore
from PyQt5 import QtWidgets
from PyQt5 import QtGui
from PyQt5.QtCore import QRect, QPoint

from nets import inception
from preprocessing import inception_preprocessing
from matplotlib import pyplot as plt
import numpy as np
import tensorflow as tf
import urllib.request  # 표준 라이브러리
import json
import glob

# 학습한 모델 설정
checkpoints_dir = 'D:\\train_inception_v1_SignLanguage2_FineTune_logs\\all4'


class ShowVideo(QtCore.QObject):
    num = 1
    flag = 0
    is_run = True
    camera = cv2.VideoCapture(0)
    print(camera)
    timer = 5
    text = ""
    is_trans = False
    trans_text = ""

    ret, image = camera.read()
    height, width = image.shape[:2]

    VideoSignal1 = QtCore.pyqtSignal(QtGui.QImage)
    VideoSignal2 = QtCore.pyqtSignal(QtWidgets.QTextEdit)

    def __init__(self, parent=None):
        super(ShowVideo, self).__init__(parent)

    @QtCore.pyqtSlot()
    def startVideo(self):
        global image
        global text
        global color_swapped_image
        global is_trans
        global trans_text

        is_trans = False
        text = ""

        while True:
            ret, image = self.camera.read()
            color_swapped_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            cv2.putText(color_swapped_image, text, (300, 240), cv2.FONT_HERSHEY_PLAIN, 5.0, (255, 0, 0), 5);
            if is_trans:
                font = ImageFont.truetype("gulim.ttc", 32)
                img_pil = Image.fromarray(color_swapped_image)
                draw = ImageDraw.Draw(img_pil)
                draw.text((50, 400), trans_text, font=font, fill=(255, 255, 255, 0))
                color_swapped_image = np.array(img_pil)
            qt_image1 = QtGui.QImage(color_swapped_image.data,
                                     self.width,
                                     self.height,
                                     color_swapped_image.strides[0],
                                     QtGui.QImage.Format_RGB888)
            self.VideoSignal1.emit(qt_image1)

            loop = QtCore.QEventLoop()
            QtCore.QTimer.singleShot(1, loop.quit)  # 25 ms
            loop.exec_()

    @QtCore.pyqtSlot()
    def save(self):
        global image
        global is_run
        global num
        global text
        global is_trans

        is_trans = False
        is_run = True
        num = 1
        timer = 5

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
                timer = 5
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
        self.Api_trans()

    @QtCore.pyqtSlot()
    def Api_trans(self):
        global num
        global color_swapped_image
        global is_trans
        global trans_text

        is_trans = False
        j = 1
        inf_array = []
        com_array = []
        res_array = []
        client_id = "vIj2xvkAVXo8ArNz48NH"  # 개발자센터에서 발급받은 Client ID 값
        client_secret = "xZAQ6OTeHy"  # 개발자센터에서 발급받은 Client Secret 값
        # 요청할 주소
        url = "https://openapi.naver.com/v1/papago/n2mt"

        # 이미지 평가 코드
        slim = tf.contrib.slim

        image_size = inception.inception_v1.default_image_size

        for i in range(1, num):
            with tf.Graph().as_default():
                image_input = tf.read_file('./images/' + str(i) + '.jpg')
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
                    os.path.join(checkpoints_dir, 'model.ckpt-5000'),

                    slim.get_model_variables('InceptionV1'))

                with tf.Session() as sess:
                    init_fn(sess)
                    np_image, probabilities = sess.run([image, probabilities])
                    probabilities = probabilities[0, 0:]
                    sorted_inds = [i[0] for i in sorted(enumerate(-probabilities), key=lambda x: x[1])]

                names = os.listdir("C:\\SignLanguage2\\SignLanguage2_photos")

                index = sorted_inds[0]
                print(str(j) + '번째 결과 : [%s] => %0.2f%%  ' % (names[index], probabilities[index]))
                if probabilities[index] >= 0.50:
                    inf_array.append(names[index])
                    print(inf_array)
                if (i % 3) == 0:
                    if len(inf_array) < 2:
                        print("동작이 인식되지 않았습니다. 재촬영을 해주세요.")
                    elif len(inf_array) == 2:
                        if inf_array.count(inf_array[0]) == 2:
                            com_array.append(inf_array[0])
                    elif len(inf_array) == 3:
                        if inf_array.count(inf_array[0]) >= 2:
                            com_array.append(inf_array[0])
                        elif inf_array.count(inf_array[1]) >= 2:
                            com_array.append(inf_array[1])
                    else:
                        print("오류!")
                        return
                    j = j + 1
                    inf_array.clear()

        for i in com_array:
            a = i.replace(".", "")
            p = re.compile("[^0-9]")
            print("".join(p.findall(a)))
            res_array.append("".join(p.findall(a)))

        print(res_array)
        if not res_array:
            print("인식된 동작이 없습니다.")
            self.reset()
        else:
            # 번역할 언어와 내용
            # text = input("번역할 내용을 입력하세요: ")
            text = " ".join(res_array)
            encText = urllib.parse.quote(text)
            srcLang = 'en'
            tarLang = 'ko'
            rq_data = "honorific=true&" + "source={}&target={}&text=".format(srcLang, tarLang) + encText  # 높임말인데 안될수도잉씀 나도 몰름
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
                self.reset()
                is_trans = 1
                return trans_text
            else:  # 실패
                print("Error Code:" + rescode)

    @QtCore.pyqtSlot()
    def reset(self):
        global num
        for i in range(1, num):
            path1 = "image/" + str(i) + ".jpg"
            path2 = "images/" + str(i) + ".jpg"

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


class TextSetter(QtWidgets.QTextEdit):
    def __init__(self, parent=None):
        super(TextSetter, self).__init__(parent)
        self.text = QtWidgets.QTextEdit()

    @QtCore.pyqtSlot(QtWidgets.QTextEdit)
    def setText(self, str):
        painter = QtGui.QPainter(self)
        painter.drawText(QPoint(0, 0), str)
        self.text.setText(str)
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

    push_button1.clicked.connect(vid.save)
    push_button2.clicked.connect(vid.stop)

    vertical_layout = QtWidgets.QVBoxLayout()
    horizontal_layout = QtWidgets.QHBoxLayout()
    vertical_layout2 = QtWidgets.QVBoxLayout()
    horizontal_layout.addWidget(image_viewer1)
    vertical_layout.addLayout(horizontal_layout)
    horizontal_layout.addLayout(vertical_layout2)
    vertical_layout2.addWidget(push_button1)
    vertical_layout2.addWidget(push_button2)

    layout_widget = QtWidgets.QWidget()
    layout_widget.setLayout(vertical_layout)
    main_window = QtWidgets.QMainWindow()
    main_window.setCentralWidget(layout_widget)
    main_window.show()
    vid.startVideo()
    sys.exit(app.exec_())