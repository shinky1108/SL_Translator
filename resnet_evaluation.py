
import os
import tensorflow as tf
from datasets import imagenet
from nets import inception_resnet_v2
from preprocessing import inception_preprocessing

# set your .ckpt file
#checkpoints_dir = '/home/username/models/slim'
#checkpoints_dir = 'C:\\slim(3)'
checkpoints_dir = 'C:\\SignLanguage2_2\\train_inception_v1_SL_FineTune_logs\\all3'
model = 'model.ckpt-14000'

slim = tf.contrib.slim

batch_size = 3
image_size = 299

with tf.Graph().as_default():
    with slim.arg_scope(inception_resnet_v2.inception_resnet_v2_arg_scope()):
        # set your image
        imgPath = 'C:\\TestImage\\SL2_1\\stuffy3.jpg'
        testImage_string = tf.gfile.FastGFile(imgPath, 'rb').read()
        testImage = tf.image.decode_jpeg(testImage_string, channels=3)
        processed_image = inception_preprocessing.preprocess_image(testImage, image_size, image_size, is_training=False)
        processed_images = tf.expand_dims(processed_image, 0)

        logits, _ = inception_resnet_v2.inception_resnet_v2(processed_images, num_classes=1000, is_training=False)
        probabilities = tf.nn.softmax(logits)

        init_fn = slim.assign_from_checkpoint_fn(
        os.path.join(checkpoints_dir, model), slim.get_model_variables('InceptionResnetV2'))

        with tf.Session() as sess:
            init_fn(sess)

            np_image, probabilities = sess.run([processed_images, probabilities])
            probabilities = probabilities[0, 0:]
            sorted_inds = [i[0] for i in sorted(enumerate(-probabilities), key=lambda x: x[1])]

            names = os.listdir("C:\\SignLanguage2_2\\SignLanguage2_2_photos")
            for i in range(5):
                index = sorted_inds[i]
                print('결과 : [%s] => %0.2f%%  ' % (names[index], probabilities[index]))


            #names = imagenet.create_readable_names_for_imagenet_labels()
            #for i in range(15):
            #    index = sorted_inds[i]
            #    print((probabilities[index], names[index]))