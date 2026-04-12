import numpy as np
import PIL.Image as Image
import argparse
import os

CROSS_SIDE = 24

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
    bitmaps = np.full((images.shape[0], CROSS_SIDE, CROSS_SIDE), False, dtype=bool)

    for i, img in enumerate(images):
        # Resize image to final height width
        resized_img = Image.fromarray(img).resize((CROSS_SIDE, CROSS_SIDE), Image.ANTIALIAS)

        # Apply thresholding on image to convert to bitmap
        bitmaps[i] = np.array(resized_img) > (threshold * 255)
    
    return bitmaps

def bitmapper(bitmap: np.ndarray) -> np.ndarray:
    '''
    Convert bitmap to bytes using custom mapping
    '''
    if bitmap.shape != (CROSS_SIDE, CROSS_SIDE):
        raise ValueError(f"Bitmap must be of shape {(CROSS_SIDE, CROSS_SIDE)}, but got {bitmap.shape}")
    
    byte_dict = {"TR": [], "BL": [], "C": []}

    for i in range(8):
        # Construct Top Right
        top_col = bitmap[0:8, 15-i]
        right_col = bitmap[8:16, 23-i]
        TR_cols = np.concatenate((top_col, right_col))
        TR_bin_value = np.sum([bit << i for i, bit in enumerate(TR_cols)])
        byte_dict["TR"].append(TR_bin_value)

        # Construct Bottom Left
        bottom_col = bitmap[16:23, 15-i]
        left_col = bitmap[8:16, 7-i]
        BL_cols = np.concatenate((left_col, bottom_col))
        BL_bin_value = np.sum([bit << i for i, bit in enumerate(BL_cols)])
        byte_dict["BL"].append(BL_bin_value)

        # Construct Center
        center_col = bitmap[8:16, 15-i]
        C_cols = np.concatenate((center_col, center_col))  # Center is duplicated to fill 16 bits
        C_bin_value = np.sum([bit << i for i, bit in enumerate(C_cols)])
        byte_dict["C"].append(C_bin_value)
    
    return byte_dict

def bitmap2txt(bitmaps: np.ndarray, out_path: str='./animation_library/new_animation.txt') -> None:
    '''
    Export list of bitmaps to .txt file
    '''
    # Create a big string with the correct format for C++ array initialization
    text_output = '{\t//Animation X: name\n'
    for i, bitmap in enumerate(bitmaps):
        byte_dict = bitmapper(bitmap)
        print(f"Bitmap {i} byte mapping: {byte_dict}")
        text_output += '\t{\t// Frame ' + f'{i}\n'
        for key in ["TR", "BL", "C"]:
            text_output += '\t\t{'
            text_output += ', '.join(f"0x{byte:04x}" for byte in byte_dict[key])
            text_output += '},\n'
        text_output += '\t},\n'
    text_output += '}'
    # print("Final text output:\n", text_output)
    
    # Create txt file and print the big string in it, save and exit
    with open(out_path, "w") as f:
        f.write(text_output)


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
        for source in ["sample.gif"]:#, "samples/"]:
            print(f"Testing open_images with {source} ...")
            imgs = open_images(f"./media/{source}")
            print(f"Number of frames: {imgs.shape[0]}, Image shape: {imgs.shape[1:]}")
            
            print("Testing image2bitmap ...")
            btmps = image2bitmap(imgs, threshold=0.5)
            print(f"Bitmap shape: {btmps.shape}, Bitmap dtype: {btmps.dtype}")
            # print(f"Sample bitmap (first frame):\n{btmps[0].astype(int)}")

            bitmap2txt(btmps)

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