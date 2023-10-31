import subprocess
import urllib.request 
from PIL import Image 
import os
import io
import scipy.fft as sdct
import scipy.fft as sidct

#from skimage import metrics


class P1:
    def rgb_yuv(self, a, b, c,rgb_to_yuv = True):
        '''
        Convert RGB values to YUV color space or visa versa.

        Args:
            rgb to yuv:
            a (float R): Red channel value (0-255).
            b (float G): Green channel value (0-255).
            c (float B): Blue channel value (0-255).

            yuv to rgb:
            a (float Y)
            b (float U) 
            c (float V)


        Returns:
            YUV or RGB values depending on the conversion type.
        '''
        if rgb_to_yuv == True:
            y = round(0.299 * a + 0.587 * b + 0.114 * c)
            u = round((c - y) * 0.492)
            v = round((a - y) * 0.877)
            return y, u, v
        else:
            r = round(a + 1.13983 * c)
            g = round(a - 0.39465 * b - 0.58060 * c)
            b = round(a + 2.03211 * b)
            print('success')
        return r, g, b


    def resize_and_reduce_quality(self,input_file, output_file, width, height, quality):
        '''
        Resize an image and reduce its quality using FFmpeg.

        Args:
            input_file (str): The path to the input image file.
            output_file (str): The path to the output image file.
            width (int): The new width for the resized image.
            height (int): The new height for the resized image.
            quality (int): The quality setting for the output image (0-100).

        Returns:
            tuple: A tuple containing the dimensions (width, height) of the original and modified images.
            Saves output image to directory specified.
        '''
        # Get the current working directory
        current_directory = os.getcwd()

        # Construct the full path for the output image
        output_path = os.path.join(current_directory, output_file)
        cmd = [
            'ffmpeg',
            '-i', input_file,  # Input image file
            '-vf', f'scale={width}:{height}',  # Resize the image
            #'-c:v', 'libx264',  # Codec, No need to specify for image (JPEG)
            '-q:v', str(quality),  # Set quality (lower value for higher compression)
            output_path  # Output image file
        ]
    
        subprocess.run(cmd)

         # Open the original and modified images
        original_image = Image.open(input_file)
        modified_image = Image.open(output_path)

        original_width, original_height = original_image.size
        modified_width, modified_height = modified_image.size

        # Compute the SSIM quality metric
        #ssim_quality = metrics.structural_similarity(original_image, modified_image)
        print('success')

        return (original_width, original_height), (modified_width, modified_height)

    def serpentine(self,matrix):
            '''
            Convert a matrix to a serpentine array .

            Args:
                matrix (matrix): input of a mxm matrix.

            Returns:
                array: The serpentine read of the matrix.
            '''
            serpentine_data = []
            rows, cols = len(matrix), len(matrix[0])

            for d in range(rows + cols - 1):
                if d % 2 == 0:
                    # Traverse downward along the diagonal
                    for i in range(max(0, d - rows + 1), min(d, cols - 1) + 1):
                        row, col = d - i, i
                        serpentine_data.append(matrix[row][col])
                else:
                    
                    # Traverse upward along the diagonal
                    for i in range(max(0, d - cols + 1), min(d, rows - 1) + 1):
                        row, col = i, d - i
                        serpentine_data.append(matrix[row][col])
            print('success')
            return serpentine_data

    def transform_to_bw(self,input_image, output_image):
        # Get the current working directory
        current_directory = os.getcwd()

        # Construct the full path for the output image
        output_path = os.path.join(current_directory, output_image)
        cmd = [
            'ffmpeg',
            '-i', input_image,   # Input image file
            '-vf', 'format=gray',  # Convert to grayscale (B/W)
            '-q:v', '0',           # Set video quality to the lowest value for maximum compression
            output_path           # Output image file
        ]

        subprocess.run(cmd)
        print('success')

    def run_length_encode(self,data):
        encoded_data = []
        count = 1

        for i in range(1, len(data)):
            if data[i] == data[i - 1]:
                count += 1
            else:
                encoded_data.extend([count, data[i - 1]])
                count = 1

        encoded_data.extend([count, data[-1]])
        print('success')
        return encoded_data

class DCTConverter:
    def dct(self, data):
        dct_data = sdct.dct(data, type=2, norm='ortho')
        print('success')
        return dct_data

    def idct(self, dct_data):
        idct_data = sidct.idct(dct_data, type=2, norm='ortho')
        print('success')
        return idct_data
    
        
            

p1 = P1()

### Exercise 1 ###
print('------------------ EXERCISE 1 ----------------')
# Convert RGB to YUV
r = 100
g = 255
b = 100
y, u, v = p1.rgb_yuv(r, g, b, True)
print(f"RGB to YUV: R={r}, G={g}, B={b} -> Y={y}, U={u}, V={v}")

# Convert YUV to RGB
y = 128
u = -37
v = 94
r, g, b = p1.rgb_yuv(y, u, v,False)
print(f"YUV to RGB: Y={y}, U={u}, V={v} -> R={r}, G={g}, B={b}")

### Exercise 2 ###
print('------------------ EXERCISE 2 ----------------')

image_url = 'https://img.freepik.com/free-photo/cute-domestic-kitten-sits-window-staring-outside-generative-ai_188544-12519.jpg?size=626&ext=jpg&ga=GA1.1.386372595.1697673600&semt=ais'
urllib.request.urlretrieve(image_url,'gatito.jpg')

input_image = 'gatito.jpg'
output_image = 'gatito_reduced.jpg'
new_width = 300
new_height = 150
image_quality = 1  # Adjust the quality as needed

original_size, modified_size = p1.resize_and_reduce_quality(input_image, output_image, new_width, new_height, image_quality)    

print(f"Original image size: {original_size[0]}x{original_size[1]} pixels")
print(f"Modified image size: {modified_size[0]}x{modified_size[1]} pixels")

### Exercise 3 ###
print('------------------ EXERCISE 3 ----------------')

matrix = [
    [1, 2, 3, 4, 5],
    [6, 7, 8, 9, 10],
    [11, 12, 13, 14, 15],
    [16, 17, 18, 19, 20],
    [21, 22, 23, 24, 25]
]
serpentine_data = p1.serpentine(matrix)
print(f'serpentine data: {serpentine_data}')

### Exercise 4 ###
print('------------------ EXERCISE 4 ----------------')

input_image = 'gatito.jpg'
output_image = 'gatito_bw.jpg'
p1.transform_to_bw(input_image, output_image)


print('------------------ EXERCISE 5 ----------------')
data = [0,0,0,1,1,2,3,3,3,4,5,5,5,5,5]
encoded_data = p1.run_length_encode(data)
print(f'original data: {data}, encoded_data: {encoded_data}')

print('------------------ EXERCISE 6 ----------------')
dct_converter = DCTConverter()
data = [255,180,0,255,200]
dct_data = dct_converter.dct(data)
idct_data = dct_converter.idct(dct_data)

print(f'original: {data}, dct: {dct_data}, idct: {idct_data}')