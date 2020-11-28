# coding: utf-8
# # Object Detection Demo
# License: Apache License 2.0 (https://github.com/tensorflow/models/blob/master/LICENSE)
# source: https://github.com/tensorflow/models
import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile
import math
from udp_utils import send_to_visualization_engine, generate_bytes_command, generate_bytes_packet, send_to_keyboard_emulator
from time import time

from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image
from grabscreen import grab_screen
import cv2
from directkeys import W, A, S, D, SPACE, PressKey, ReleaseKey
from time import time
# This is needed since the notebook is stored in the object_detection folder.
sys.path.append("..")
sys.path.append(
    "C:\\Users\\allen\\AppData\\Roaming\\Python\\Python37\\site-packages\\tensorflow\\models\\research\\object_detection")
sys.path.append(
    "C:\\Users\\allen\\AppData\\Roaming\\Python\\Python37\\site-packages\\tensorflow\\models\\research")
try:
    from object_detection.utils import visualization_utils as vis_util
    from object_detection.utils import label_map_util
except:
    from utils import label_map_util
    from utils import visualization_utils as vis_util

# ## Object detection imports
# Here are the imports from the object detection module.


# # Model preparation
# What model to download.
MODEL_NAME = 'ssd_mobilenet_v1_coco_11_06_2017'
MODEL_FILE = MODEL_NAME + '.tar.gz'
DOWNLOAD_BASE = 'http://download.tensorflow.org/models/object_detection/'

# Path to frozen detection graph. This is the actual model that is used for the object detection.
PATH_TO_CKPT = MODEL_NAME + '/frozen_inference_graph.pb'

# List of the strings that is used to add correct label for each box.
PATH_TO_LABELS = os.path.join('data', 'mscoco_label_map.pbtxt')

NUM_CLASSES = 90
# ## Load a (frozen) Tensorflow model into memory.
detection_graph = tf.Graph()
with detection_graph.as_default():
    od_graph_def = tf.compat.v1.GraphDef()
    with tf.compat.v2.io.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
        serialized_graph = fid.read()
        od_graph_def.ParseFromString(serialized_graph)
        tf.import_graph_def(od_graph_def, name='')


# ## Loading label map
# Label maps map indices to category names, so that when our convolution network predicts `5`, we know that this corresponds to `airplane`.  Here we use internal utility functions, but anything that returns a dictionary mapping integers to appropriate string labels would be fine
label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
categories = label_map_util.convert_label_map_to_categories(
    label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
category_index = label_map_util.create_category_index(categories)


# Helper code
def load_image_into_numpy_array(image):
    (im_width, im_height) = image.size
    return np.array(image.getdata()).reshape(
        (im_height, im_width, 3)).astype(np.uint8)

x = 0
with detection_graph.as_default():

    with tf.compat.v1.Session(graph=detection_graph) as sess:
        while True:
            # Constantly reading from screenshot folder's "screenshot.png" file
            img = cv2.imread("./ScreenShot%s.png" % x)
            x += 1
            if x == 10:
                x = 0
            #print("i: ", x)
            if (img is not None):
                # Feeding image
                image_np = img #cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
                # Expand dimensions since the model expects images to have shape: [1, None, None, 3]
                image_np_expanded = np.expand_dims(image_np, axis=0)
                image_tensor = detection_graph.get_tensor_by_name(
                    'image_tensor:0')
                # Each box represents a part of the image where a particular object was detected.
                boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
                # Each score represent how level of confidence for each of the objects.
                # Score is shown on the result image, together with the class label.
                scores = detection_graph.get_tensor_by_name(
                    'detection_scores:0')
                classes = detection_graph.get_tensor_by_name(
                    'detection_classes:0')
                num_detections = detection_graph.get_tensor_by_name(
                    'num_detections:0')
                # Actual detection.
                (boxes, scores, classes, num_detections) = sess.run(
                    [boxes, scores, classes, num_detections],
                    feed_dict={image_tensor: image_np_expanded})
                # Visualization of the results of a detection.
                vis_util.visualize_boxes_and_labels_on_image_array(
                    image_np,
                    np.squeeze(boxes),
                    np.squeeze(classes).astype(np.int32),
                    np.squeeze(scores),
                    category_index,
                    use_normalized_coordinates=True,
                    line_thickness=5)
                for i in range(len(scores[0])):
                    if scores[0][i] > 0.5:
                        bounding_box = boxes[0][i]
                        area = math.sqrt(((bounding_box[2] - bounding_box[0]) * 8) ** 2 + (
                            (bounding_box[3] - bounding_box[1]) * 6) ** 2)
                        name = category_index[classes[0][i]]['name']
                        if name == 'person':
                            distance = 5 / area
                        elif name == 'car' or name == 'truck':
                            distance = 10 / area
                        else:
                            distance = 7 / area
                        x_offset = ((bounding_box[3] + bounding_box[1])/2 - 0.5)*10

                        packet = generate_bytes_packet({
                            'type': int(classes[0][i]),
                            'dist': float(distance)*20,
                            'angle': float(x_offset)*30
                        })

                        send_to_visualization_engine(packet)

                        if(name == 'person' and distance < 12 and x_offset < 1.5 and x_offset > -1.5):
                            # PressKey(SPACE)
                            if (x_offset > 0):
                                cmd = generate_bytes_command('a ')
                                # PressKey(A)
                            else:
                                cmd = generate_bytes_command('d ')
                                # PressKey(D)
                            print(f"[{time()}] \33[91m WARNING \33[0m]: Pedestrian ahead! Emergency Brake Applied")
                            print(name, " | confidence: ", scores[0][i], "distance: ",distance, "angle", x_offset)
                            send_to_keyboard_emulator(cmd)
                        # print('sending command: ',command)
                        # send_to_keyboard_emulator(command)
                cv2.imshow('window', image_np)
                #print("--- %s seconds ---" % (time() - start_time))
            if cv2.waitKey(25) & 0xFF == ord('q'):
                cv2.destroyAllWindows()
                break