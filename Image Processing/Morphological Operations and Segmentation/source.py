import warnings

warnings.filterwarnings("ignore")

from scipy import fftpack
from PIL import Image
import os
import cv2
import numpy as np
from matplotlib import pyplot as plt
import math
from sympy import fwht
import shutil
from os.path import join
import random
from sklearn.cluster import MeanShift, estimate_bandwidth
from sklearn.datasets import make_blobs
from itertools import cycle
import matplotlib.pylab as pylab
import scipy.linalg
import scipy.spatial
from skimage.segmentation import felzenszwalb, slic, quickshift
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import data, segmentation, color
from skimage.future import graph


INPUT_PATH = "./THE4_Images/"
OUTPUT_PATH = "./Outputs/"



def object_counter(input_path, output_path, image_name):

    if image_name == "A1":
        a1 = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)

        a1_thresh = 117
        a1_binarized = np.uint8((a1 > a1_thresh) * 255)

        a1_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (30, 30))
        a1_out = cv2.morphologyEx(a1_binarized, cv2.MORPH_ERODE, a1_kernel)

        a1_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (70, 70))
        a1_out = cv2.morphologyEx(a1_out, cv2.MORPH_DILATE, np.array(a1_kernel, dtype=np.uint8))

        a1_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (10, 10))
        a1_out = cv2.morphologyEx(a1_out, cv2.MORPH_CLOSE, np.array(a1_kernel, dtype=np.uint8))

        count1, _ = cv2.connectedComponents(a1_out)

        print("The number of flowers in image A1 is", count1-1)
        
        cv2.imwrite(output_path, a1_out)
        
    elif image_name == "A2":
        a2 = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)

        a2_thresh = 117
        a2_binarized = np.uint8((a2 > a2_thresh) * 255)


        a2_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (10, 10))
        a2_out = cv2.morphologyEx(a2_binarized, cv2.MORPH_CLOSE, a2_kernel)

        a2_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (60, 60))
        a2_out = cv2.morphologyEx(a2_out, cv2.MORPH_ERODE, np.array(a2_kernel, dtype=np.uint8))

        a2_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (50, 50))
        a2_out = cv2.morphologyEx(a2_out, cv2.MORPH_OPEN, np.array(a2_kernel, dtype=np.uint8))

        a2_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (70, 70))
        a2_out = cv2.morphologyEx(a2_out, cv2.MORPH_DILATE, np.array(a2_kernel, dtype=np.uint8))

        a2_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (130, 130))
        a2_out = cv2.morphologyEx(a2_out, cv2.MORPH_DILATE, np.array(a2_kernel, dtype=np.uint8))


        count2, _ = cv2.connectedComponents(a2_out)

        print("The number of flowers in image A2 is", count2-1)
        
        cv2.imwrite(output_path, a2_out)
        
    elif image_name == "A3":
        a3 = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)

        a3_thresh = 144
        a3_binarized = np.uint8((a3 > a3_thresh) * 255)
        
        a3_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (100, 100))
        a3_out = cv2.morphologyEx(a3_binarized, cv2.MORPH_OPEN, a3_kernel)

        a3_kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (30, 30))
        a3_out = cv2.morphologyEx(a3_out, cv2.MORPH_DILATE, np.array(a3_kernel, dtype=np.uint8))

        count3, _ = cv2.connectedComponents(a3_out)

        print("The number of flowers in image A3 is", count3-1)
        
        cv2.imwrite(output_path, a3_out)



def segment_image(input_path, output_path, method, shrink_bandwidth, num_of_segments):

    image = cv2.imread(input_path)

    if method == "meanshift":



        image_r = Image.open(input_path)
        image_copy_r = Image.open(input_path)

        width, height = image_r.size

        if width >= 700:
            downscale = width // 500
            width //= downscale
            height //= downscale

        image = image_r.resize((width, height), resample=Image.BICUBIC)
        image_copy = image_copy_r.resize((image.size), resample=Image.BICUBIC)

        
        image = np.array(image)
        image_copy = np.array(image_copy)
        original_shape = image.shape

        X = np.reshape(image, [-1, 3])


        n_clusters_ = 0
        i = 0
        bandwidth = 0

        while(n_clusters_ < 2): 
            if i == 0:
                bandwidth = estimate_bandwidth(X, quantile=0.1, n_samples=100)
                bandwidth -= shrink_bandwidth
            else:
                bandwidth -= 2

            ms = MeanShift(bandwidth=bandwidth, bin_seeding=True)
            ms.fit(X)


            labels = ms.labels_
            cluster_centers = ms.cluster_centers_

            labels_unique = np.unique(labels)
            n_clusters_ = len(labels_unique)
            i += 1

        segmented_image = np.reshape(
            labels, original_shape[:2]
        )

        for i in range(n_clusters_):
            image_copy[np.where(segmented_image == i)] = np.mean(
                image[np.where(segmented_image == i)], axis=0
            )

        ##################### BOUNDARY OVERLAY EXTRACTION #####################
        gray = cv2.cvtColor(image_copy, cv2.COLOR_BGR2GRAY)
        threshold, binary = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
        
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        dilated = cv2.dilate(binary, kernel, iterations=1)

        dilated_3d = cv2.merge([dilated, dilated, dilated])
    
        overlay = cv2.subtract(image, dilated_3d)
        
        binarized_overlay = np.uint8((overlay > 0) * 255)

        dilated_overlay = cv2.dilate(binarized_overlay, kernel, iterations=1)
        
        binarized_overlay_new = cv2.subtract(dilated_overlay, binarized_overlay)
        
        
        boundary_overlay = cv2.add(image_copy, binarized_overlay_new)
        
        j = []
        for i in [image, image_copy, boundary_overlay]: 
            j.append(cv2.cvtColor(i, cv2.COLOR_RGB2BGR))
                
        
        result = cv2.hconcat(j)
        cv2.imwrite(output_path, result)

          
    elif method == "ncut":
        img = image

        labels1 = segmentation.slic(img, compactness=30, n_segments=num_of_segments)

        g = graph.rag_mean_color(img, labels1, mode='similarity')
        labels2 = graph.cut_normalized(labels1, g)
        out2 = color.label2rgb(labels2, img, kind='avg')

        
        ##################### BOUNDARY OVERLAY EXTRACTION #####################
        
        gray = cv2.cvtColor(out2, cv2.COLOR_BGR2GRAY)
        threshold, binary = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
        
        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (5, 5))
        dilated = cv2.dilate(binary, kernel, iterations=1)

        dilated_3d = cv2.merge([dilated, dilated, dilated])
    
        overlay = cv2.subtract(image, dilated_3d)
        
        binarized_overlay = np.uint8((overlay > 0) * 255)

        dilated_overlay = cv2.dilate(binarized_overlay, kernel, iterations=1)
        
        binarized_overlay_new = cv2.subtract(dilated_overlay, binarized_overlay)
        
        
        boundary_overlay = cv2.add(out2, binarized_overlay_new)
        
        #######################################################################
        
        j = []
        for i in [img, out2, boundary_overlay]: 
            j.append(i)
        
        result = cv2.hconcat(j)
        cv2.imwrite(output_path, result)
        
        

   
if __name__ == '__main__':
    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)

    ######################################################################
    object_counter(INPUT_PATH + "A1.png", OUTPUT_PATH + "A1.png", "A1")
    object_counter(INPUT_PATH + "A2.png", OUTPUT_PATH + "A2.png", "A2")
    object_counter(INPUT_PATH + "A3.png", OUTPUT_PATH + "A3.png", "A3")
    
    ######################################################################

    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_meanshift_0.png", "meanshift",0,0)        
    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_meanshift_5.png", "meanshift",5,0)    
    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_meanshift_10.png", "meanshift",10,0)
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_meanshift_-10.png", "meanshift",-10,0)        
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_meanshift_0.png", "meanshift",0,0)    
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_meanshift_10.png", "meanshift",10,0)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_meanshift_-10.png", "meanshift", -10,0)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_meanshift_0.png", "meanshift", 0,0)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_meanshift_10.png", "meanshift", 10,0)
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_meanshift_-10.png", "meanshift",-10,0)        
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_meanshift_0.png", "meanshift",0,0)    
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_meanshift_10.png", "meanshift",10,0)
    
    
    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_ncut_100.png", "ncut",0,100)
    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_ncut_200.png", "ncut",0,200)
    segment_image(INPUT_PATH + "B1.jpg", OUTPUT_PATH + "B1_algorithm_ncut_400.png", "ncut",0,400)
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_ncut_100.png", "ncut",0,100)
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_ncut_200.png", "ncut",0,200)
    segment_image(INPUT_PATH + "B2.jpg", OUTPUT_PATH + "B2_algorithm_ncut_400.png", "ncut",0,400)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_ncut_100.png", "ncut",0,100)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_ncut_200.png", "ncut",0,200)
    segment_image(INPUT_PATH + "B3.jpg", OUTPUT_PATH + "B3_algorithm_ncut_400.png", "ncut",0,400)
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_ncut_100.png", "ncut",0,100)
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_ncut_200.png", "ncut",0,200)
    segment_image(INPUT_PATH + "B4.jpg", OUTPUT_PATH + "B4_algorithm_ncut_400.png", "ncut",0,400)