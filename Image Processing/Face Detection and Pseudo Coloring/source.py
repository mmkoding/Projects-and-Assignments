from PIL import Image, ImageOps, ImageDraw
from scipy import fftpack
import PIL
import os
import cv2
import imageio
import numpy as np
from matplotlib import pyplot as plt
import math
from scipy.linalg import hadamard
from sympy import fwht
import shutil
from os.path import join
import random

INPUT_PATH = "./THE3_Images/"
OUTPUT_PATH = "./Outputs/"


def mark_the_image(detected_image,original_image):
    face_points = []

    rows = original_image.shape[0]
    columns = original_image.shape[1]

    marksignal = 0

    for i in range(0,rows):
      for j in range(0,columns):
          if(detected_image[i][j][0] > 100 and detected_image[i][j][1] > 120 and detected_image[i][j][2] > 150):
            if(marksignal == 50):
                marksignal = 0
                face_points.append((i,j))
            marksignal += 1

    for i in range(0,len(face_points)):
        cv2.circle(original_image, face_points[i], 3, (0,0,255), -1)

    return original_image




def morphological_ops(image):
    kernel = np.ones((30,30),np.uint8)
    closing = cv2.morphologyEx(image, cv2.MORPH_CLOSE, kernel)
    kernel2 = np.ones((9,9),np.uint8)
    opening = cv2.morphologyEx(closing, cv2.MORPH_OPEN, kernel2)
    return opening



def contrast_enchance(img):

    b, g, r = cv2.split(img)

    minimum_value = np.percentile(g, 30)
    maximum_value = np.percentile(g, 98)
    g = np.clip(g, minimum_value, maximum_value)
    g = ((g - minimum_value) / (maximum_value - minimum_value)) * 255

    minimum_value = np.percentile(b, 30)
    maximum_value = np.percentile(b, 98)
    b = np.clip(b, minimum_value, maximum_value)
    b = ((b - minimum_value) / (maximum_value - minimum_value)) * 255

    minimum_value = np.percentile(r, 30)
    maximum_value = np.percentile(r, 98)
    r = np.clip(r, minimum_value, maximum_value)
    r = ((r - minimum_value) / (maximum_value - minimum_value)) * 255


    rgb_img = cv2.merge((b, g, r))

    return(rgb_img)




def detect_faces(input_path, output_path, lowContrast):  

    img = cv2.imread(input_path)
    detected_image = cv2.imread(input_path)
    
    M = img.shape[0]
    N = img.shape[1]

    if (lowContrast == 1):
        img = contrast_enchance(img)


    if (lowContrast == 2):
        for x in range(M):
            for y in range(N):
                if(img[x][y][2] > img[x][y][1] and img[x][y][2] > img[x][y][0] and img[x][y][1] > img[x][y][0] and
                abs(int(img[x][y][1])-int(img[x][y][2])) >= 50 and float(img[x][y][0]) / float(img[x][y][2]) <= 1.6 and 
                float(img[x][y][1]) / float(img[x][y][2]) <= 1.6 and int(img[x][y][0]) > 100 and int(img[x][y][1]) > 100 and 
                int(img[x][y][2]) > 100 and float(img[x][y][1]) / float(img[x][y][0]) > 1.1 and float(img[x][y][1]) / float(img[x][y][0]) < 2.5):
                   continue
                   
                detected_image[x][y][2] = 0
                detected_image[x][y][1] = 0
                detected_image[x][y][0] = 0  
    

    for x in range(M):
        for y in range(N):
            if(img[x][y][2] > img[x][y][1] and img[x][y][2] > img[x][y][0] and img[x][y][1] > img[x][y][0] and
            abs(int(img[x][y][1])-int(img[x][y][2])) >= 50 and float(img[x][y][0]) / float(img[x][y][2]) <= 1.6 and 
            float(img[x][y][1]) / float(img[x][y][2]) <= 1.6):
               continue
               
            detected_image[x][y][2] = 0
            detected_image[x][y][1] = 0
            detected_image[x][y][0] = 0



    detected_image = morphological_ops(detected_image)

    output_img = mark_the_image(detected_image, img)


    if(input_path == INPUT_PATH + "3_source.png"):        
        cv2.imwrite(output_path, output_img)
    
      
    
    
def color_images(input_path, output_path, color_map_path):
    # Convert Grey to Pseudocolor
    im_gray = cv2.imread(input_path, cv2.IMREAD_GRAYSCALE)
    im_gray = cv2.cvtColor(im_gray, cv2.COLOR_GRAY2BGR);
    color_map = cv2.imread(color_map_path)
    lut = np.zeros((256, 1, 3), dtype=np.uint8)
        
    height = color_map.shape[0]
    width = color_map.shape[1]
    
    image_arr_blue = np.zeros(height*width, dtype=np.uint8)
    image_arr_green = np.zeros(height*width, dtype=np.uint8)
    image_arr_red = np.zeros(height*width, dtype=np.uint8)
    
    for y in range(height):
        for x in range(width):
            image_arr_blue[y*width + x] = color_map[y][x][0]
            image_arr_green[y*width + x] = color_map[y][x][1]
            image_arr_red[y*width + x] = color_map[y][x][2]
            
    image_arr_blue.sort()
    image_arr_green.sort()
    image_arr_red.sort()
            
    
    size = (height*width) // 255
    
    for i in range(256):
        lut[i, 0, 0] = image_arr_blue[i*size]
    for i in range(256):
        lut[i, 0, 1] = image_arr_green[i*size]
    for i in range(256):
        lut[i, 0, 2] = image_arr_red[i*size]
    
    lut[:, 0, 0].sort()
    lut[:, 0, 1].sort()
    lut[:, 0, 2].sort()
    
    im_color = cv2.LUT(im_gray, lut)
    
    #im_color = cv2.applyColorMap(im_gray, cv2.COLORMAP_PINK)
    if(width > height):
        im_color = cv2.rotate(im_color, cv2.ROTATE_90_COUNTERCLOCKWISE) 
    
    cv2.imwrite(output_path, im_color)
    
def get_color_channel(input_path, output_path, channel):
    
    src = cv2.imread(input_path, cv2.IMREAD_UNCHANGED)
    
    if channel == "red":
        _channel = src[:,:,2]
        _img = np.zeros(src.shape)
        _img[:,:,2] = _channel
    
    elif channel == "green":
        _channel = src[:,:,1]
        _img = np.zeros(src.shape)
        _img[:,:,1] = _channel
    
    elif channel == "blue":
        _channel = src[:,:,0]
        _img = np.zeros(src.shape)
        _img[:,:,0] = _channel
        
    elif channel == "hue":
        img = cv2.imread(input_path, 1)
        img = np.float32(img)/255
        red = img[:,:,2]
        green = img[:,:,1]
        blue = img[:,:,0]
        
        hue = np.copy(red)

        for i in range(0, blue.shape[0]):
            for j in range(0, blue.shape[1]):
                hue[i][j] = 0.5 * ((red[i][j] - green[i][j]) + (red[i][j] - blue[i][j])) / \
                            math.sqrt((red[i][j] - green[i][j])**2 +
                                    ((red[i][j] - blue[i][j]) * (green[i][j] - blue[i][j])))
                hue[i][j] = math.acos(hue[i][j])

                if blue[i][j] <= green[i][j]:
                    hue[i][j] = hue[i][j]
                else:
                    hue[i][j] = ((360 * math.pi) / 180.0) - hue[i][j]
                    
        hue[:,:] = hue[:,:]*180/math.pi 
                    
        cv2.imwrite(output_path, hue)
        return
        
    elif channel == "saturation":
        img = cv2.imread(input_path, 1)
        img = np.float32(img)/255
        red = img[:,:,2]
        green = img[:,:,1]
        blue = img[:,:,0]
        saturation = np.copy(red)
        
        minimum = np.minimum(np.minimum(red, green), blue)
        saturation = 1 - (3 / (red + green + blue + 0.00000001) * minimum)
        
        saturation[:,:] = 100*saturation[:,:] 
        
        cv2.imwrite(output_path, saturation)
        return
        
    elif channel == "intensity":
        img = cv2.imread(input_path, 1)
        img = np.float32(img)/255
        red = img[:,:,2]
        green = img[:,:,1]
        blue = img[:,:,0]
        intensity = np.copy(red)
        
        intensity = np.divide(blue + green + red, 3)
        
        intensity[:,:] = 255*intensity[:,:] 
        
        cv2.imwrite(output_path, intensity)
        return
                    
      
    cv2.imwrite(output_path,_img)
    
    
def merge_hsi(hue_path, saturation_path, intensity_path, output_path):
    hue = cv2.imread(hue_path, 0)
    saturation = cv2.imread(saturation_path, 0)
    intensity = cv2.imread(intensity_path, 0)
    hsi = cv2.merge((hue, saturation, intensity))
    
    cv2.imwrite(output_path,hsi)

    
def detect_edges(input_path, output_path, type, isHSV):
    img = cv2.imread(input_path)
    # Detect edges using sobel filters.
    
    if isHSV:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    
    
    if type == "horizontal_sobel":
        horiz_edge_map = cv2.filter2D(img, -1, np.array([[1, 2, 1],
                                                    [0, 0, 0],
                                                    [-1, -2, -1]]))
        cv2.imwrite(output_path, horiz_edge_map)
    
    elif type == "vertical_sobel":
        vert_edge_map = cv2.filter2D(img, -1, np.array([[-1, 0, 1],
                                                    [-2, 0, 2],
                                                    [-1, 0, 1]]))
        cv2.imwrite(output_path, vert_edge_map)
        
    elif type == "vertical_roberts":
        vert_edge_map = cv2.filter2D(img, -1, np.array([[1, 0],
                                                        [0, -1]]))
        cv2.imwrite(output_path, vert_edge_map)
    
    elif type == "horizontal_roberts":
        vert_edge_map = cv2.filter2D(img, -1, np.array([[0, 1],
                                                        [-1,0]]))
        cv2.imwrite(output_path, vert_edge_map)
    
    elif type == "horizontal_prewitt":
        vert_edge_map = cv2.filter2D(img, -1, np.array([[1, 1, 1],
                                                    [0, 0, 0],
                                                    [-1, -1, -1]]))
        cv2.imwrite(output_path, vert_edge_map)
        
    elif type == "vertical_prewitt":
        vert_edge_map = cv2.filter2D(img, -1, np.array([[-1, 0, 1],
                                                    [-1, 0, 1],
                                                    [-1, 0, 1]]))
        cv2.imwrite(output_path, vert_edge_map)
    
        
if __name__ == '__main__':
    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)
    #PART1
    
    ######################################################################
    
    detect_faces(INPUT_PATH + "1_source.png", OUTPUT_PATH + "1_faces.png", 1)
    detect_faces(INPUT_PATH + "2_source.png", OUTPUT_PATH + "2_faces.png", 2)
    detect_faces(INPUT_PATH + "3_source.png", OUTPUT_PATH + "3_faces.png", 3)
    
    
    ######################################################################
    
    
    for i in [1,2,3,4]:
        color_images(INPUT_PATH + str(i) + ".png", OUTPUT_PATH + str(i) + "_colored.png", INPUT_PATH + str(i) + "_source.png")
    
    #for i in [1,2,3,4]:
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_red.png", "red")
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_green.png", "green")
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_blue.png", "blue")
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_hue.png", "hue")
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_saturation.png", "saturation")
        #get_color_channel(OUTPUT_PATH + str(i) + "_colored.png", OUTPUT_PATH + str(i) + "_colored_intensity.png", "intensity")
        #merge_hsi(OUTPUT_PATH + str(i) + "_colored_hue.png", OUTPUT_PATH + str(i) + "_colored_saturation.png", OUTPUT_PATH + str(i) + "_colored_intensity.png", OUTPUT_PATH + str(i) + "_colored_hsi.png")
    
        
        
    for i in [1,2,3]:
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_horizontal_sobel_" + str(i) + "_colored_edges.png", "horizontal_sobel", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_vertical_sobel_" + str(i) + "_colored_edges.png", "vertical_sobel", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_horizontal_sobel_" + str(i) + "_colored_edges.png", "horizontal_sobel", True)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_vertical_sobel_" + str(i) + "_colored_edges.png", "vertical_sobel", True)
        
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_horizontal_roberts_" + str(i) + "_colored_edges.png", "horizontal_roberts", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_vertical_roberts_" + str(i) + "_colored_edges.png", "vertical_roberts", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_horizontal_roberts_" + str(i) + "_colored_edges.png", "horizontal_roberts", True)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_vertical_roberts_" + str(i) + "_colored_edges.png", "vertical_roberts", True)

        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_horizontal_prewitt_" + str(i) + "_colored_edges.png", "horizontal_prewitt", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "RGB_vertical_prewitt_" + str(i) + "_colored_edges.png", "vertical_prewitt", False)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_horizontal_prewitt_" + str(i) + "_colored_edges.png", "horizontal_prewitt", True)
        detect_edges(INPUT_PATH + str(i) + "_source.png", OUTPUT_PATH + "HSI_vertical_prewitt_" + str(i) + "_colored_edges.png", "vertical_prewitt", True)


