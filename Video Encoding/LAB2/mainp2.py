import os
import subprocess
import json
from mainp1 import P1
import cv2
from PIL import Image
import numpy as np

class P2:
    #EX1
    def to_mp2(self, input_file,output_file):
         # Get the current working directory
        current_directory = os.getcwd()

        # Construct the full path for the output image
        output_path = os.path.join(current_directory, output_file)
        info_path = os.path.join(current_directory,'info.mp4')
        # Conversion command using FFmpeg
        cmd = [
            'ffmpeg',
            '-i', input_file,  # Input video file
            '-c:v', 'mpeg2video',  # Video codec for MP2
            '-q:v', '2',  # Set video quality
            '-c:a', 'mp2',  # Audio codec for MP2
            output_path  # Output video file
        ]
        subprocess.run(cmd)
        # Get video info using FFmpeg
        output_path = os.path.join(current_directory, 'info.json')
        info_cmd = [
            'ffmpeg',
            '-i', input_file,  # Input video file
            output_path     
        ]
        
    #EX2
    def mod_resolution(self,input_file, output_file, width, height):
        # Get the current working directory
        current_directory = os.getcwd()

        # Construct the full path for the output video
        output_path = os.path.join(current_directory, output_file)

        # FFmpeg command to get video information
        info_cmd = [
            'ffprobe',
            '-v', 'error',
            '-select_streams', 'v:0',
            '-show_entries', 'stream=width,height',
            '-of', 'csv=s=x:p=0',
            input_file
        ]
    
        info_result = subprocess.run(info_cmd, capture_output=True, text=True)
        old_resolution = info_result.stdout.strip()

        # FFmpeg command to modify the resolution
        cmd = [
            'ffmpeg',
            '-i', input_file,  # Input video file
            '-vf', f'scale={width}:{height}',  # Resize the video to the desired resolution
            output_path  # Output video file
        ]

        subprocess.run(cmd)

        # Print old and new resolutions
        print("Old Resolution:", old_resolution)
        print("New Resolution:", f"{width}x{height}")
        print('Ex2 success')
    
    #EX3
    def modify_chroma_subsampling(self,input_file, output_file, chroma_subsampling):
        # Get the current working directory
        current_directory = os.getcwd()

        # Construct the full path for the output video
        output_path = os.path.join(current_directory, output_file)
        # Get the current working directory

        

        # FFprobe command to get the chroma subsampling format
        info_cmd = [
            'ffprobe',
            '-v', 'error',
            '-select_streams', 'v:0',
            '-show_entries', 'stream=pix_fmt',
            '-of', 'default=nokey=1:noprint_wrappers=1',
            input_file
        ]
        old_subsampling = subprocess.run(info_cmd, capture_output=True, text=True).stdout.strip()

        # FFmpeg command to modify chroma subsampling
        cmd = [
            'ffmpeg',
            '-i', input_file,  # Input video file
            '-vf', f'format={chroma_subsampling}',  # Change chroma subsampling
            '-c:v', 'libx264',  # Set the video codec (change as needed)
            '-q:v', '2',  # Set video quality (change as needed)
            output_path  # Output video file
        ]

        subprocess.run(cmd)

        print("Old Chroma Subsampling:", old_subsampling)
        print("New Chroma Subsampling:", chroma_subsampling)
        print('Ex3 success')

#EX4
    def extract_video_data(self,input_file):
        info_cmd = [
        'ffprobe',
        '-v', 'error',
        '-select_streams', 'v:0',
        '-show_entries', 'stream=width,height,duration,bit_rate,codec_name,chroma_location',
        '-of', 'json',
        input_file,  # Input video file
        ]
        result = subprocess.run(info_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        if result.returncode == 0:
            info = json.loads(result.stdout)
        else:
            print("Error while running ffprobe:", result.stderr)
            return None

        video_info =  info['streams'][0]
        
        print("Video Information:")
        print("Width:", video_info['width'])
        print("Height:", video_info['height'])
        print("Duration:", video_info['duration'], "seconds")
        print("Bit Rate:", video_info['bit_rate'], "bits/s")
        print("Codec Name:", video_info['codec_name'])
        print("Chroma Location:", video_info['chroma_location'])
        print('Ex4 success')
        return video_info

    def bw_video(self,input_file,output_file):
        p1 = P1()
        cap = cv2.VideoCapture(input_file)
        current_directory = os.getcwd()
        # Construct the full path for the output video
        saved_frame_path = os.path.join(current_directory, 'bbb_frames')
        bw_frames_path = os.path.join(current_directory, 'bw_frames')
        output_path = os.path.join(current_directory, output_file)


        if not cap.isOpened():
            print("Error: Could not open video file.")
            return False

        frame_count = 0
        modified_frames = []

        while True:
            ret, frame = cap.read()
            if not ret:
                break
            if frame_count == 10:
                break

            modified_frames.append(frame)

            #save frame
            frame = Image.fromarray(modified_frames[frame_count])
            frame.save(f"{saved_frame_path}/frame_{frame_count}.jpg")

            #modify frame
            input_image = f"{saved_frame_path}/frame_{frame_count}.jpg"
            output_image = f'{bw_frames_path}/frame_{frame_count}.jpg'
            # Modify the frame to grayscale using FFmpeg
            p1.transform_to_bw(input_image, output_image)
            frame_count += 1

        cap.release()

        #CREATE THE VIDEO
        #Set the path to the folder containing your JPEG frames
        folder_path = bw_frames_path

        # Create a list of frame filenames
        frame_filenames = [os.path.join(folder_path, filename) for filename in os.listdir(folder_path)]

        # Sort the filenames to ensure the frames are in the correct order
        frame_filenames.sort()

        # Set the output video filename and codec (e.g., MP4 with H.264 codec)
        output_video = "BW_video.mp4"
        fourcc = cv2.VideoWriter_fourcc(*"mp4v")

        # Get the dimensions of the first frame to determine the video size
        frame = cv2.imread(frame_filenames[0])
        print(frame)
        height, width, _ = frame.shape

        # Create a VideoWriter object
        out = cv2.VideoWriter(output_video, fourcc, 30, (width, height))

        # Read each frame, write it to the video, and display progress
        for frame_filename in frame_filenames:
            frame = cv2.imread(frame_filename)
            out.write(frame)

        # Release the VideoWriter
        out.release()

        print('Ex5 success')
        return True

            


p2 = P2()
input_file = "BigBuckBunny.mp4"
print('please, decompress the BBB.mp4 file before running the program')
'''
#Ex 1
print('----------------- Ex 1 --------------------')
output_file = "BigBuckBunny.mp2"
video_info = p2.to_mp2(input_file,output_file)
print(video_info)

#Ex2
print('----------------- Ex 2 --------------------')
width = 420
height = 360
output_file = "BigBuckBunny_modRes.mp4"
p2.mod_resolution(input_file, output_file, width, height)

#Ex3
print('----------------- Ex 3 --------------------')
chroma_subsampling = 'yuv444p'
output_file = "BigBuckBunny_chrSub.mp4"
p2.modify_chroma_subsampling(input_file, output_file, chroma_subsampling)


#Ex4
print('----------------- Ex 4 --------------------')
data = p2.extract_video_data(input_file)

'''
#Ex5
print('----------------- Ex 5 --------------------')
input_file = "BigBuckBunny.mp4"
output_file = "BigBuckBunny_bw.mp4"
video = p2.bw_video(input_file, output_file)