import PIL
import os
import cv2
from matplotlib import pyplot as plt

INPUT_PATH = "./THE1_Images/"
OUTPUT_PATH = "./Outputs/"

def read_rgb_image(img_path, rgb = True):
    img = Image.open(img_path).convert("RGB")
    return img

def read_grayscale_image(img_path, rgb = False):
    img = Image.open(img_path).convert("L")
    return img

def read_image_opencv(img_path):
    img = cv2.imread(img_path,0)
    return img

def write_image(img, output_path, rgb = True):
    img = img.save(output_path)

def write_image_opencv(img, output_path):
    img = cv2.imwrite(output_path, img)

def extract_save_histogram(img, path):
    plt.hist(img.ravel(),256,[0,256])
    plt.savefig(path)
    plt.clf()


def rotate_image(img,  degree = 0, interpolation_type = "linear"):
    #interpolation type: "linear" or "cubic"
    #degree: 45 or 90
    if interpolation_type == "linear":
        rotated = img.rotate(degree, PIL.Image.Resampling.BILINEAR)
    elif interpolation_type == "cubic":
        rotated = img.rotate(degree, PIL.Image.Resampling.BICUBIC)
    return rotated

def histogram_equalization(img):
    equalizedImage = cv2.equalizeHist(img)
    return equalizedImage

if __name__ == '__main__':
    if not os.path.exists(OUTPUT_PATH):
        os.makedirs(OUTPUT_PATH)
    #PART1
    img = read_rgb_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 45, "linear")
    write_image(output, OUTPUT_PATH + "a1_45_linear.png")

    img = read_rgb_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 45, "cubic")
    write_image(output, OUTPUT_PATH + "a1_45_cubic.png")

    img = read_rgb_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 90, "linear")
    write_image(output, OUTPUT_PATH + "a1_90_linear.png")

    img = read_rgb_image(INPUT_PATH + "a1.png")
    output = rotate_image(img, 90, "cubic")
    write_image(output, OUTPUT_PATH + "a1_90_cubic.png")

    img = read_rgb_image(INPUT_PATH + "a2.png")
    output = rotate_image(img, 45, "linear")
    write_image(output, OUTPUT_PATH + "a2_45_linear.png")

    img = read_rgb_image(INPUT_PATH + "a2.png")
    output = rotate_image(img, 45, "cubic")
    write_image(output, OUTPUT_PATH + "a2_45_cubic.png")

    #PART2
    img = read_image_opencv(INPUT_PATH + "b1_gray.png")
    extract_save_histogram(img, OUTPUT_PATH + "original_histogram.png")
    equalized = histogram_equalization(img)
    extract_save_histogram(equalized, OUTPUT_PATH + "equalized_histogram.png")
    output = equalized
    write_image_opencv(output, OUTPUT_PATH + "enhanced_image.png")

    # BONUS
    # Define the following function
    # equalized = adaptive_histogram_equalization(img)
    # extract_save_histogram(equalized, OUTPUT_PATH + "adaptive_equalized_histogram.png")
    # write_image(output, OUTPUT_PATH + "adaptive_enhanced_image.png")


