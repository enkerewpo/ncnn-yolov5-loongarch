import cv2
import os
import sys
from skimage.util import random_noise
from skimage.io import imsave
from skimage.transform import resize
import random
import numpy as np

# remove everything except the bear1.jpg and gen.py

excluded_files = ['bear1.jpg', 'gen.py']

for file in os.listdir():
    if file not in excluded_files:
        os.remove(file)

def add_gaussian_noise(image, mean=0, sigma=25):
    """
    Add Gaussian noise to the image.
    """
    h, w, c = image.shape
    noise = np.random.normal(mean, sigma, (h, w, c))
    noisy_image = np.clip(image + noise, 0, 255).astype(np.uint8)
    return noisy_image
        

def random_color_shift(image, color_shift_factor=0.1):
    """
    Randomly shift color channels to simulate color distortion.
    """
    # Generate random color shift factors for each channel
    color_factors = np.random.uniform(-color_shift_factor, color_shift_factor, size=3)
    
    # Apply color shift to the image
    shifted_image = image.copy().astype(np.float32)
    shifted_image[:, :, 0] += color_factors[0] * 255  # Red channel
    shifted_image[:, :, 1] += color_factors[1] * 255  # Green channel
    shifted_image[:, :, 2] += color_factors[2] * 255  # Blue channel
    
    # Clip pixel values to valid range [0, 255]
    shifted_image = np.clip(shifted_image, 0, 255).astype(np.uint8)

    return shifted_image

def random_brightness(image, brightness_range=(0.5, 1.5)):
    """
    Randomly adjust brightness of the image within the specified range.
    """
    # Generate a random brightness factor
    brightness_factor = np.random.uniform(*brightness_range)

    # Apply brightness adjustment
    adjusted_image = cv2.convertScaleAbs(image, alpha=brightness_factor)

    return adjusted_image

def apply_color_distortion(image, color_shift_factor=0.1, brightness_range=(0.5, 1.5)):
    """
    Apply random color distortion to the image with configurable parameters.
    """
    # Randomly shift color channels
    distorted_image = random_color_shift(image, color_shift_factor)

    # Randomly adjust brightness
    # distorted_image = random_brightness(distorted_image, brightness_range)

    return distorted_image

def apply_gaussian_blur(image, kernel_size=(5, 5), sigma=0):
    """
    Apply Gaussian blur to the image.
    """
    blurred_image = cv2.GaussianBlur(image, kernel_size, sigma)
    return blurred_image

input_img_bear1 = cv2.imread('bear1.jpg')

# resize to 1920x1080 and 640x480

output_img_bear1_1920x1080 = cv2.resize(input_img_bear1, (1920, 1080))
output_img_bear1_640x480 = cv2.resize(input_img_bear1, (640, 480))

cv2.imwrite('bear1_cam1.jpg', output_img_bear1_1920x1080)
cv2.imwrite('bear1_cam2.jpg', output_img_bear1_640x480)

# then generate images with noise level 0 - 100, simulate ISO noise in digital camera
# both for 1920x1080 and 640x480, cam1 and cam2

for noise_level in range(0, 101):
    noise_img_bear1_1920x1080 = add_gaussian_noise(output_img_bear1_1920x1080, sigma=noise_level)
    noise_img_bear1_640x480 = add_gaussian_noise(output_img_bear1_640x480, sigma=noise_level)
    
    # add color distortion to the image according to the noise level
    # more noise, more distortion
    # color_shift_factor = noise_level / 100 * 0.1
    # sigma = noise_level / 100 * 5
    csf = noise_level / 100 * 0.1
    sigma = noise_level / 100 * 5
    
    noise_img_bear1_1920x1080 = apply_color_distortion(noise_img_bear1_1920x1080, color_shift_factor=csf)
    noise_img_bear1_640x480 = apply_color_distortion(noise_img_bear1_640x480, color_shift_factor=csf)
    
    noise_img_bear1_1920x1080 = apply_gaussian_blur(noise_img_bear1_1920x1080, sigma=sigma)
    noise_img_bear1_640x480 = apply_gaussian_blur(noise_img_bear1_640x480, sigma=sigma)
    
    cv2.imwrite(f'bear1_cam1_noise_{noise_level}.jpg', noise_img_bear1_1920x1080)
    cv2.imwrite(f'bear1_cam2_noise_{noise_level}.jpg', noise_img_bear1_640x480)