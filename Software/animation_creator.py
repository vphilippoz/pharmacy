import numpy as np
import PIL.Image as Image
import argparse
import os


def open_images(path: str) -> np.ndarray:
    '''
    Open gif image or sequence of images
    '''
    images = []

    if path.endswith(".gif"):
        # Open gif and extract all frames
        img = Image.open(path)
        try:
            while True:
                images.append(np.array(img.copy()))
                img.seek(img.tell() + 1)
        except EOFError:
            pass
    else:
        # Read all images from folder
        for filename in sorted(os.listdir(path)):
            if filename.endswith((".png", ".jpg", ".jpeg")):
                img_path = os.path.join(path, filename)
                img = Image.open(img_path)
                images.append(np.array(img))
    
    img_side = min(min(img.shape[0] for img in images), min(img.shape[1] for img in images))
    np_images = np.zeros((len(images), img_side, img_side))
    for i, img in enumerate(images):
        # Crop to square
        height, width = img.shape
        left = (width - img_side) // 2
        top = (height - img_side) // 2
        right = left + img_side
        bottom = top + img_side
        img_cropped = img[top:bottom, left:right]
        
        # Add image in the np array
        np_images[i] = np.array(img_cropped)

    return np_images

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

if __name__ == "__main__":
    tester = True
    if tester == True:
        # Test the functions
        for source in ["sample.gif", "samples/"]:
            print(f"Testing open_images with {source} ...")
            imgs = open_images(f"./media/{source}")
            print(f"Number of frames: {imgs.shape[0]}, Image shape: {imgs.shape[1:]}")

    else:
        # Create args for CLI (path, threshold, loop_period_ms)
        parser = argparse.ArgumentParser(description="Create an animation from a gif or a sequence of images")
        parser.add_argument("path", type=str, help="Path to the source gif or image folder")
        parser.add_argument("--threshold", type=float, default=0.5, help="Threshold for converting images to bitmap (between 0 and 1)")
        parser.add_argument("--loop_period_ms", type=float, default=1000.0, help="Loop period of the output animation in milliseconds")
        args = parser.parse_args()
        
        path = args.path
        thrsh = args.threshold
        period = args.loop_period_ms
        print(f"Process starts with Path={path}, Threshold={thrsh}, Loop period= {period} ms\n")

        # Process images
        print("Opening source images ...")
        imgs = open_images(path)

        print("Processing images ...")
        btmps = image2bitmap(imgs, thrsh)

        # Export result
        print("Exporting bitmaps ...")
        bitmap2txt(btmps)
        bitmap2image(btmps, period)