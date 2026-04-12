import numpy as np
from PIL import Image, ImageDraw
import argparse
import os

CROSS_SIDE = 24
MAX_IMG_COUNT = 15

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
    
    # Ignore some images to avoid memory issues
    if len(images) > MAX_IMG_COUNT:
        images = images[:MAX_IMG_COUNT]
        # step = len(images) // MAX_IMG_COUNT
        # images = images[::step][:MAX_IMG_COUNT]
        # print(f"Too many images, only keeping {len(images)} frames for processing")
    
    # Crop all images to the same size and store them in a np array
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
        bottom_col = bitmap[16:24, 15-i]
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

def bitmap2txt(bitmaps: np.ndarray, out_path: str='./Software/animation_library/new_animation.txt', name: str="name") -> None:
    '''
    Export list of bitmaps to .txt file
    '''
    # Create a big string with the correct format for C++ array initialization
    text_output = '{\t//Animation X: ' + f'{name}\n'
    for i, bitmap in enumerate(bitmaps):
        byte_dict = bitmapper(bitmap)
        text_output += '\t{\t// Frame ' + f'{i}\n'
        for key in ["TR", "BL", "C"]:
            text_output += '\t\t{'
            text_output += ', '.join(f"0x{byte:04x}" for byte in byte_dict[key])
            text_output += '},\n'
        text_output += '\t},\n'
    text_output += '}'
    
    # Create txt file and print the big string in it, save and exit
    with open(out_path, "w") as f:
        f.write(text_output)


def bitmap2image(bitmaps: np.ndarray, loop_period_ms: float=1000.0, out_path: str='./Software/animation_library/new_animation.gif') -> None:
    '''
    Create a gif rendering of the image sequence
    '''
    # Check if bitmaps have the correct shape
    if bitmaps.shape[1:] != (CROSS_SIDE, CROSS_SIDE):
        raise ValueError(f"Bitmaps must be of shape (N, {CROSS_SIDE}, {CROSS_SIDE}), but got {bitmaps.shape}")
    
    frames = []
    frame0 = Image.new("RGB", (10*CROSS_SIDE, 10*CROSS_SIDE), color="gray")

    for i, bitmap in enumerate(bitmaps):
        img = Image.new("RGB", (10*CROSS_SIDE, 10*CROSS_SIDE), color="gray")

        # Draw cross background
        draw = ImageDraw.Draw(img)
        draw.rectangle([(0, 10*CROSS_SIDE/3), (10*CROSS_SIDE, 10*CROSS_SIDE*2/3)], outline="black", fill="black") # Black for inactive pixels
        draw.rectangle([(10*CROSS_SIDE/3, 0), (10*CROSS_SIDE*2/3, 10*CROSS_SIDE)], outline="black", fill="black") # Black for inactive pixels
        
        # Draw active pixels in green
        for j in range(CROSS_SIDE):
            for k in range(CROSS_SIDE):
                out_of_bounds = (j <= 7 and k <=7) or (j >= 16 and k >= 16) or (j <= 7 and k >= 16) or (j >= 16 and k <= 7)
                if out_of_bounds: continue
                if bitmap[j, k]: draw.ellipse([(10*k+2, 10*j+2), (10*k+8, 10*j+8)], outline=None, fill="green")
        if i > 0: frames.append(img)
        else: frame0 = img  # Use the first frame as the base for the gif

    # Export list of images as a gif
    frame0.save(out_path, save_all=True, append_images=frames, duration=loop_period_ms/bitmaps.shape[0], loop=0)

if __name__ == "__main__":
    tester = False  # Set to True to run tests, False to use CLI
    if tester == True:
        # Test the functions
        for source in ["sample.gif"]:#, "samples/"]:
            print(f"Testing open_images with {source} ...")
            imgs = open_images(f"./media/{source}")
            print(f"Number of frames: {imgs.shape[0]}, Image shape: {imgs.shape[1:]}")
            
            print("Testing image2bitmap ...")
            btmps = image2bitmap(imgs, threshold=0.2)
            print(f"Bitmap shape: {btmps.shape}, Bitmap dtype: {btmps.dtype}")
            
            print("Testing bitmap2txt ...")
            bitmap2txt(btmps, out_path=f"./Software/animation_library/{source.split('.')[0]}_animation.txt")
            print(f"Bitmap exported to .txt file successfully at ./Software/animation_library/{source.split('.')[0]}_animation.txt\n")

            print("Testing bitmap2image ...")
            bitmap2image(btmps, loop_period_ms=1000.0, out_path=f"./Software/animation_library/{source.split('.')[0]}_animation.gif")
            print(f"Bitmap exported to .gif file successfully at ./Software/animation_library/{source.split('.')[0]}_animation.gif\n")
    else:
        # Create args for CLI (path, threshold, loop_period_ms)
        parser = argparse.ArgumentParser(description="Create an animation from a gif or a sequence of images")
        parser.add_argument("path", type=str, help="Path to the source gif or image folder")
        parser.add_argument("--threshold", type=float, default=0.5, help="Threshold for converting images to bitmap (between 0 and 1)")
        parser.add_argument("--loop_period_ms", type=float, default=1000.0, help="Loop period of the output animation in milliseconds")
        args = parser.parse_args()
        
        # Define input and output paths
        in_path = str(args.path)
        name = in_path.split('/')[-1].split('.')[0]
        out_path_txt = f"./Software/animation_library/{name}_animation.txt"
        out_path_gif = f"./Software/animation_library/{name}_animation.gif"

        # Process images
        print("Opening source images ...")
        imgs = open_images(in_path)

        print("Processing images ...")
        btmps = image2bitmap(imgs, args.threshold)

        # Export result
        print("Exporting bitmaps ...")
        bitmap2txt(btmps, out_path=out_path_txt, name=name)
        bitmap2image(btmps, args.loop_period_ms, out_path=out_path_gif)
        print(f"Animation exported successfully to {out_path_txt} and {out_path_gif}")