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


# Mustafa Mert Köse 2381705
# Murat Akkoyun 2380020

INPUT_PATH = "./THE2_images/"
OUTPUT_PATH = "./Outputs/"

def read_rgb_image(img_path, rgb = True):
    img = Image.open(img_path).convert("RGB")
    return img


def read_image_opencv(img_path):
    img = cv2.imread(img_path,0)
    return img

def write_image(img, output_path, rgb = True):
    img = img.save(output_path)

def write_image_opencv(img, output_path):
    img = cv2.imwrite(output_path, img)
    
    
def fourier_transform(input_path, output_path):

    img = cv2.imread(input_path) #fourier 1
    b, g, r = cv2.split(img)
    
    f = np.fft.fft2(r)
    fshift_r = np.fft.fftshift(f)
    magnitude_spectrum_r = np.log1p(np.abs(fshift_r))
    
    f = np.fft.fft2(g)
    fshift_g = np.fft.fftshift(f)
    magnitude_spectrum_g = np.log1p(np.abs(fshift_g))
    
    f = np.fft.fft2(b)
    fshift_b = np.fft.fftshift(f)
    magnitude_spectrum_b = np.log1p(np.abs(fshift_b))
    
    plt.imshow(magnitude_spectrum_r, cmap = 'gray')
    plt.savefig(OUTPUT_PATH + "F1r.png")
    
    plt.imshow(magnitude_spectrum_g, cmap = 'gray')
    plt.savefig(OUTPUT_PATH + "F1G.png")
    
    plt.imshow(magnitude_spectrum_b, cmap = 'gray')
    plt.savefig(OUTPUT_PATH + "F1b.png")
    
    img_r = cv2.imread(OUTPUT_PATH + "F1r.png",0)
    img_g = cv2.imread(OUTPUT_PATH + "F1g.png",0)
    img_b = cv2.imread(OUTPUT_PATH + "F1b.png",0)
    
    os.remove(OUTPUT_PATH + "F1r.png")
    os.remove(OUTPUT_PATH + "F1g.png")
    os.remove(OUTPUT_PATH + "F1b.png")
    
    re = np.fft.ifftshift(magnitude_spectrum_r)
    ge = np.fft.ifftshift(magnitude_spectrum_g)
    be = np.fft.ifftshift(magnitude_spectrum_b)
      
    magnitude_spectrum = cv2.merge((img_b, img_g, img_r))
    magnitude_spectrum = cv2.imwrite(output_path + ".png", magnitude_spectrum)
    
    f_out_r = np.abs(np.fft.ifft2(fshift_r)) #Restore the image by taking inverse fourier transform
    f_out_g = np.abs(np.fft.ifft2(fshift_g))
    f_out_b = np.abs(np.fft.ifft2(fshift_b))
    
    f_out = cv2.merge((f_out_b, f_out_g, f_out_r))
    #f_out = cv2.imwrite(output_path + "_out.png", f_out) 
    
    
def cosine_transform(input_path, output_path):

    np_img = cv2.imread(input_path)
    M = np_img.shape[0]
    N = np_img.shape[1]

    img_dimension = 0

    if(M > N):
        img_dimension = M
    else:
        img_dimension = N


    R_img = np.zeros([img_dimension, img_dimension])
    G_img = np.zeros([img_dimension, img_dimension])
    B_img = np.zeros([img_dimension, img_dimension])
    C_matrix = np.zeros([img_dimension, img_dimension])
    C_transpose_matrix = np.zeros([img_dimension, img_dimension])
    R_cosine_transform_matrix = np.zeros([img_dimension, img_dimension])
    G_cosine_transform_matrix = np.zeros([img_dimension, img_dimension])
    B_cosine_transform_matrix = np.zeros([img_dimension, img_dimension])
    RGB_cosine_transform_matrix = np.zeros([img_dimension, img_dimension])
    output_matrix = np.zeros([img_dimension, img_dimension, 3])
    output_matrix_original_image = np.zeros([img_dimension, img_dimension, 3])

    for m in range(0,M):
        for n in range(0,N):
            R_img[m][n] = np_img[m][n][2]
            G_img[m][n] = np_img[m][n][1]
            B_img[m][n] = np_img[m][n][0]


    K_n = 0

    for m in range(0,img_dimension):
        for n in range(0,img_dimension):
            if(n == 0):
                K_n = 1/img_dimension**(1/2)
            else:
                K_n = (2/img_dimension)**(1/2)

            C_matrix[m][n] = K_n*math.cos(((2*m+1)*n*math.pi)/(2*img_dimension))

    
    C_transpose_matrix = C_matrix.transpose()


    R_cosine_transform_matrix = ((C_transpose_matrix.dot(R_img)).dot(C_matrix)) # Cosine tranform applied to channel R
    G_cosine_transform_matrix = ((C_transpose_matrix.dot(G_img)).dot(C_matrix)) # Cosine tranform applied to channel G
    B_cosine_transform_matrix = ((C_transpose_matrix.dot(B_img)).dot(C_matrix)) # Cosine tranform applied to channel B


    for m in range(0,M):
        for n in range(0,N):
            output_matrix[m][n][2] = R_cosine_transform_matrix[m][n]
            output_matrix[m][n][1] = G_cosine_transform_matrix[m][n]
            output_matrix[m][n][0] = B_cosine_transform_matrix[m][n]
            
    ############################################################ THIS PART IS FOR RETRIEVEING THE ORIGINAL IMAGE BY APPLYING REVERSE TRANSFORM
    R_original_image = ((C_matrix.dot(R_cosine_transform_matrix)).dot(C_transpose_matrix)) # Reverse cosine tranform applied to channel R         
    G_original_image = ((C_matrix.dot(G_cosine_transform_matrix)).dot(C_transpose_matrix)) # Reverse cosine tranform applied to channel G
    B_original_image = ((C_matrix.dot(B_cosine_transform_matrix)).dot(C_transpose_matrix)) # Reverse cosine tranform applied to channel B                

    for m in range(0,M):
        for n in range(0,N):
            output_matrix_original_image[m][n][2] = R_original_image[m][n]
            output_matrix_original_image[m][n][1] = G_original_image[m][n]
            output_matrix_original_image[m][n][0] = B_original_image[m][n]


    cv2.imwrite(output_path + ".png", output_matrix)
    #cv2.imwrite(output_path + "_reverse.png", output_matrix_original_image)
        
 


def hadamard_transform(input_path, output_path):

    np_img = cv2.imread(input_path)
    M = np_img.shape[0]
    N = np_img.shape[1]

    img_dimension = 0

    if(M > N):
        img_dimension = M
    else:
        img_dimension = N


    while(1):
        if(math.log2(img_dimension).is_integer() == False):
            img_dimension += 1
            #print(img_dimension)
        else:
            break

    #print(img_dimension)    
    R_img = np.zeros([img_dimension, img_dimension])
    G_img = np.zeros([img_dimension, img_dimension])
    B_img = np.zeros([img_dimension, img_dimension])
    H_matrix = np.zeros([img_dimension, img_dimension])

    #hadamard_dimension = math.log2(img_dimension)
    H_matrix = hadamard(img_dimension)

    R_hadamard_transform_matrix = np.zeros([img_dimension, img_dimension])
    G_hadamard_transform_matrix = np.zeros([img_dimension, img_dimension])
    B_hadamard_transform_matrix = np.zeros([img_dimension, img_dimension])

    output_matrix = np.zeros([img_dimension, img_dimension, 3])
    output_matrix_original_image = np.zeros([img_dimension, img_dimension, 3])

    for m in range(0,M):
        for n in range(0,N):
            R_img[m][n] = np_img[m][n][2]
            G_img[m][n] = np_img[m][n][1]
            B_img[m][n] = np_img[m][n][0]

    R_hadamard_transform_matrix = ((H_matrix.dot(R_img)).dot(H_matrix)) # hadamard tranform applied to channel R
    G_hadamard_transform_matrix = ((H_matrix.dot(G_img)).dot(H_matrix)) # hadamard tranform applied to channel G
    B_hadamard_transform_matrix = ((H_matrix.dot(B_img)).dot(H_matrix)) # hadamard tranform applied to channel B


    for m in range(0,M):
        for n in range(0,N):
            output_matrix[m][n][2] = R_hadamard_transform_matrix[m][n]
            output_matrix[m][n][1] = G_hadamard_transform_matrix[m][n]
            output_matrix[m][n][0] = B_hadamard_transform_matrix[m][n]

    R_original_image = ((H_matrix.dot(R_hadamard_transform_matrix)).dot(H_matrix))/(img_dimension**2) # reverse hadamard tranform applied to channel R         
    G_original_image = ((H_matrix.dot(G_hadamard_transform_matrix)).dot(H_matrix))/(img_dimension**2) # reverse hadamard tranform applied to channel G
    B_original_image = ((H_matrix.dot(B_hadamard_transform_matrix)).dot(H_matrix))/(img_dimension**2) # reverse hadamard tranform applied to channel B             

    for m in range(0,M):
        for n in range(0,N):
            output_matrix_original_image[m][n][2] = R_original_image[m][n]
            output_matrix_original_image[m][n][1] = G_original_image[m][n]
            output_matrix_original_image[m][n][0] = B_original_image[m][n]


    cv2.imwrite(output_path + ".png", output_matrix)
    #cv2.imwrite(output_path + "_reverse.png", output_matrix_original_image)



def ideal_pass_filtering(input_path, output_path, filtering_size, isHighPass, filtering_type):


    np_img = cv2.imread(input_path, 0)
    M = np_img.shape[0]
    N = np_img.shape[1]
    H = np.zeros((M,N), dtype=np.float32)
    D0 = filtering_size #cutoff frequency
    
    F = np.fft.fftshift(np.fft.fft2(np_img))
    
    
    if(filtering_type == "ideal"):
        for u in range(M): 
            for v in range(N):
                D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
                if D <= D0:
                    H[u,v] = 1
                else:
                    H[u,v] = 0
        if(isHighPass == 1):
            H = 1 - H
        
                    
    elif(filtering_type == "gaussian"):
        for u in range(M):
            for v in range(N):
                D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
                H[u,v] = np.exp(-D**2/(2*D0*D0)) 
        if(isHighPass == 1):
            H = 1 - H
                
                
                
                
    elif(filtering_type == "butterworth"):
        
        n = 2 #order
        
        if(isHighPass == 0):
            n = 2 #order
            for u in range(M):
                for v in range(N):
                    D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
                    H[u,v] = 1 / (1 + (D/D0)**(2*n))
        
        elif(isHighPass == 1):
            for u in range(M):
                for v in range(N):
                    D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
                    H[u,v] = 1 / (1 + (D0/D)**(2*n))
                    
                    
    elif(filtering_type == "band"):
        n=2
        L = np.zeros((M,N), dtype=np.float32)
        for u in range(M):
            for v in range(N):
                D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
                H[u,v] = 1 / (1 + (D/D0)**(2*n))
                L[u,v] = 1 / (1 + (50/D)**(2*n))
        
        if(isHighPass == 1):
            H = H - L
                    
    
    GShift = F * H
    G = np.fft.ifftshift(GShift)
    
    g = np.abs(np.fft.ifft2(G))
    
    cv2.imwrite(output_path, g)
    
    
def ideal_filter_merger(input_path, output_path, val, isHighPass, filtering_type):
    img = cv2.imread(input_path)
    b, g, r = cv2.split(img)
    
    r= cv2.imwrite(OUTPUT_PATH + "3_r.png", r)
    g=cv2.imwrite(OUTPUT_PATH + "3_g.png", g)
    b=cv2.imwrite(OUTPUT_PATH + "3_b.png", b)
    
           
    ideal_pass_filtering(OUTPUT_PATH + "3_r.png", OUTPUT_PATH + "ILP_r1_r.png", val, isHighPass, filtering_type)
    ideal_pass_filtering(OUTPUT_PATH + "3_g.png", OUTPUT_PATH + "ILP_r1_g.png", val, isHighPass, filtering_type)
    ideal_pass_filtering(OUTPUT_PATH + "3_b.png", OUTPUT_PATH + "ILP_r1_b.png", val, isHighPass, filtering_type)
    
    os.remove(OUTPUT_PATH + "3_r.png")
    os.remove(OUTPUT_PATH + "3_g.png")
    os.remove(OUTPUT_PATH + "3_b.png")
    
    img_r = cv2.imread(OUTPUT_PATH + "ILP_r1_r.png",0)
    img_g = cv2.imread(OUTPUT_PATH + "ILP_r1_g.png",0)
    img_b = cv2.imread(OUTPUT_PATH + "ILP_r1_b.png",0)
    
    os.remove(OUTPUT_PATH + "ILP_r1_r.png")
    os.remove(OUTPUT_PATH + "ILP_r1_g.png")
    os.remove(OUTPUT_PATH + "ILP_r1_b.png")
    
    rgb_img = cv2.merge((img_b, img_g, img_r))
    rgb_img = cv2.imwrite(output_path, rgb_img)
    
    
def band_reject_test(input_path, output_path, filtering_size, bandwidth_frequency):

    np_img = cv2.imread(input_path, 0)
    M = np_img.shape[0]
    N = np_img.shape[1]
    H = np.zeros((M,N), dtype=np.float32)
    L = np.zeros((M,N), dtype=np.float32)
    D0 = filtering_size #cutoff frequency
    
    F = np.fft.fftshift(np.fft.fft2(np_img))

    n = 2 #order
    
    for u in range(M):
        for v in range(N):
            D = np.sqrt((u-M/2)**2 + (v-N/2)**2)
            H[u,v] = 1 / (1 + (D/D0)**(2*n))
            L[u,v] = 1 / (1 + (bandwidth_frequency/D)**(2*n))
    
    H = H - L
            
    GShift = F * H
    G = np.fft.ifftshift(GShift)
    
    g = np.abs(np.fft.ifft2(G))
    
    
    cv2.imwrite(output_path + "test_" + str(filtering_size) + "_"  + str(bandwidth_frequency) + ".png", g)

def space_domain_filtering(input_path, output_path):

    img = cv2.imread(input_path)                    ## laplacian filtering
    b, g, r = cv2.split(img)

    # kernel 1
    kernel = np.array([[0, 1, 0],
                       [1, -4, 1],
                       [0, 1, 0]])

    LaplacianImage = cv2.filter2D(src=b,
                                  ddepth=-1, 
                                  kernel=kernel)

    c = -1
    g_value = b + c*LaplacianImage

    filtered_b = np.clip(g_value, 0, 255)

##################################################

    LaplacianImage = cv2.filter2D(src=g,
                                  ddepth=-1, 
                                  kernel=kernel)


    g_value = g + c*LaplacianImage

    filtered_g = np.clip(g_value, 0, 255)

##################################################

    LaplacianImage = cv2.filter2D(src=r,
                                  ddepth=-1, 
                                  kernel=kernel)


    g_value = r + c*LaplacianImage

    filtered_r = np.clip(g_value, 0, 255)

##################################################

    if(input_path == INPUT_PATH + "6.png"):             ### Contrast stretching

        minimum_value = np.percentile(filtered_g, 30)
        maximum_value = np.percentile(filtered_g, 98)
        filtered_g = np.clip(filtered_g, minimum_value, maximum_value)
        filtered_g = ((filtered_g - minimum_value) / (maximum_value - minimum_value)) * 255

        minimum_value = np.percentile(filtered_b, 30)
        maximum_value = np.percentile(filtered_b, 98)
        filtered_b = np.clip(filtered_b, minimum_value, maximum_value)
        filtered_b = ((filtered_b - minimum_value) / (maximum_value - minimum_value)) * 255

        minimum_value = np.percentile(filtered_r, 30)
        maximum_value = np.percentile(filtered_r, 98)
        filtered_r = np.clip(filtered_r, minimum_value, maximum_value)
        filtered_r = ((filtered_r - minimum_value) / (maximum_value - minimum_value)) * 255


        rgb_img = cv2.merge((filtered_b, filtered_g, filtered_r))
        rgb_img = cv2.imwrite(output_path, rgb_img)

    elif(input_path == INPUT_PATH + "7.png"):

        rgb_img = cv2.merge((filtered_b, filtered_g, filtered_r))
        rgb_img = cv2.imwrite(output_path, rgb_img)

        minimum_value = np.percentile(filtered_g, 2)
        maximum_value = np.percentile(filtered_g, 80)
        filtered_g = np.clip(filtered_g, minimum_value, maximum_value)
        filtered_g = ((filtered_g - minimum_value) / (maximum_value - minimum_value)) * 255

        minimum_value = np.percentile(filtered_b, 2)
        maximum_value = np.percentile(filtered_b, 80)
        filtered_b = np.clip(filtered_b, minimum_value, maximum_value)
        filtered_b = ((filtered_b - minimum_value) / (maximum_value - minimum_value)) * 255

        minimum_value = np.percentile(filtered_r, 2)
        maximum_value = np.percentile(filtered_r, 80)
        filtered_r = np.clip(filtered_r, minimum_value, maximum_value)
        filtered_r = ((filtered_r - minimum_value) / (maximum_value - minimum_value)) * 255

        rgb_img = cv2.merge((filtered_b, filtered_g, filtered_r))
        rgb_img = cv2.imwrite(output_path, rgb_img)




if __name__ == '__main__':
    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)
    #PART1
    
    ######################################################################
    
    fourier_transform(INPUT_PATH + "1.png", OUTPUT_PATH + "F1") #fourier 1
    
    
    ######################################################################

    hadamard_transform(INPUT_PATH + "1.png", OUTPUT_PATH + "H1") #hadamard 1

    ######################################################################

    cosine_transform(INPUT_PATH + "1.png", OUTPUT_PATH + "C1") #cosine 1
    
    ######################################################################

    fourier_transform(INPUT_PATH + "2.png", OUTPUT_PATH + "F2") #fourier 2
    
    
    ######################################################################
    
    hadamard_transform(INPUT_PATH + "2.png", OUTPUT_PATH + "H2") #hadamard 2

    ######################################################################

    cosine_transform(INPUT_PATH + "2.png", OUTPUT_PATH + "C2") #cosine 2
    
    ###################################################################### IDEAL LOW PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "ILP_r1.png", 25, 0, "ideal")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "ILP_r2.png", 50, 0, "ideal")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "ILP_r3.png", 100, 0, "ideal")
    
    ###################################################################### GAUSSIAN LOW PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GLP_r1.png", 25, 0, "gaussian")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GLP_r2.png", 50, 0, "gaussian")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GLP_r3.png", 100, 0, "gaussian")
    
    
    ###################################################################### BUTTERWORTH LOW PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BLP_r1.png", 25, 0, "butterworth")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BLP_r2.png", 50, 0, "butterworth")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BLP_r3.png", 100, 0, "butterworth")
    
    
    ###################################################################### IDEAL HIGH PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "IHP_r1.png", 25, 1, "ideal")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "IHP_r2.png", 50, 1, "ideal")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "IHP_r3.png", 100, 1, "ideal")
    
    ###################################################################### GAUSSIAN HIGH PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GHP_r1.png", 25, 1, "gaussian")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GHP_r2.png", 50, 1, "gaussian")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "GHP_r3.png", 100, 1, "gaussian")
    
    ###################################################################### BUTTERWORTH HIGH PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BHP_r1.png", 25, 1, "butterworth")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BHP_r2.png", 50, 1, "butterworth")
    ideal_filter_merger(INPUT_PATH + "3.png", OUTPUT_PATH + "BHP_r3.png", 100, 1, "butterworth")
    
    ###################################################################### BAND REJECT FILTERING
    
    ideal_filter_merger(INPUT_PATH + "4.png", OUTPUT_PATH + "BR1.png", 2000, 1, "band")
    ideal_filter_merger(INPUT_PATH + "5.png", OUTPUT_PATH + "BR2.png", 2000, 1, "band")
    
    ###################################################################### BAND PASS FILTERING
    
    ideal_filter_merger(INPUT_PATH + "4.png", OUTPUT_PATH + "BP1.png", 2000, 0, "band")
    ideal_filter_merger(INPUT_PATH + "5.png", OUTPUT_PATH + "BP2.png", 2000, 0, "band")

    space_domain_filtering(INPUT_PATH + "6.png", OUTPUT_PATH + "Space6.png")
    space_domain_filtering(INPUT_PATH + "7.png", OUTPUT_PATH + "Space7.png")
    
    ###################################################################### TEST SITE
    
    #out_radius = 2000
    #in_radius = 30
    #
    #for i in range(40):
    #    band_reject_test(INPUT_PATH + "4.png", OUTPUT_PATH, out_radius, in_radius)
    #    in_radius += 1
    
    