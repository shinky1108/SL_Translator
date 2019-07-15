from matplotlib import pyplot as plt
import numpy as np
import os
import tensorflow as tf
from nets import inception
from preprocessing import inception_preprocessing

#체크포인트 경로
checkpoints_dir = 'C:\\SL\\SignLanguage3\\train_ResnetInceptionImageNet_FineTune_logs\\2'
image_name = 'C:\\SL\\TestImage\\SL2\\test0.jpg' #추론할 이미지
photo_dir = 'C:\\SL\\SignLanguage3\\SignLanguage3_photos' #학습 이미지 경로
NUM_CLASSES = 1000 #이미지 분류 폴더 개수
slim = tf.contrib.slim

image_size = inception.inception_resnet_v2.default_image_size

with tf.Graph().as_default():
    image_input = tf.read_file(image_name)
    image = tf.image.decode_jpeg(image_input, channels=3)
    processed_image = inception_preprocessing.preprocess_image(image,
                                                               image_size,
                                                               image_size,
                                                               is_training=False)
    processed_images = tf.expand_dims(processed_image, 0)


    with slim.arg_scope(inception.inception_resnet_v2_arg_scope()):
        logits, _ = inception.inception_resnet_v2(processed_images,
                                                  num_classes=NUM_CLASSES,
                                                  is_training=False)
    probabilities = tf.nn.softmax(logits)

    model_path = tf.train.latest_checkpoint(checkpoints_dir)
    # 가장 마지막 ckpt를 자동으로 인식하게 하였음. (model = 'model.ckpt-225' 이거 필요 없음)

    init_fn = slim.assign_from_checkpoint_fn(
        model_path,
        slim.get_model_variables())

    with tf.Session() as sess:
        init_fn(sess)
        np_image, probabilities = sess.run([image, probabilities])
        probabilities = probabilities[0, 0:]
        sorted_inds = [i[0] for i in sorted(enumerate(-probabilities), key=lambda x: x[1])]

    #plt.figure()
    #plt.imshow(np_image.astype(np.uint8))
    #plt.axis('off')
    #plt.show()

    names = os.listdir(photo_dir)
    for i in range(3):
        index = sorted_inds[i]
        print('%d) Probability %0.2f => [%s]' %  (i,probabilities[index], names[index]))
        # %(퍼센트 붙이려면 %% )