


# TODO:
# 1. Open gif image or sequence of images
# 2. Extract frames from the gif or read the sequence of images
# 3. Convert each frame to a bitmap
# 4. Export bitmaps in a single JSON file
# 5. Create a render of the animation using the bitmaps and export it as a gif

import numpy as np


def open_images(path: str) -> np.ndarray:
    '''
    Open gif image or sequence of images
    '''

    if path.endswith(".gif"):
        # Process gif
        pass
    else:
        # Process image folder
        pass
    
    # For each image:
    #   Convert to black and white
    #   Crop to a square format, centered on the image

    # Return all images in a single ndarray with shape (nb_images, height, width)
    return

def image2bitmap(images: np.ndarray, threshold: float) -> np.ndarray:
    '''
    Convert images to bitmap representation
    '''

    # Reduce image resolution to final height width

    # Apply thresholding on image to convert to bitmap
    
    # Return bitmap of image in a ndarray
    return
    
def bitmap2txt(bitmaps: np.ndarray) -> None:
    '''
    Export list of bitmaps to .txt file
    '''
    
    # For each bitmap:
    #   Convert bitmap to bytes using custom mapping
    #   Create string from bytes

    # Concatenate all strings in a JSON format
    
    # Create txt file and print the big string in it, save and exit

    return None


def bitmap2image(bitmaps: np.ndarray, loop_period_ms: float=1000.0) -> None:
    '''
    Create a gif rendering os the image sequence
    '''

    # For each bitmap:
    #   Create a colored image from bitmap

    # Export list of images as a gif


    return None

if __name__ == "main":
    # Get args (path, threshold, loop_period_ms)

    # Process images
    print("Opening source images ...")
    imgs = open_images(path)

    print("Processing images ...")
    btmps = image2bitmap(imgs, thrsh)

    # Export result
    print("Exporting bitmaps ...")
    bitmap2txt(btmps)
    bitmap2image(btmps, period)