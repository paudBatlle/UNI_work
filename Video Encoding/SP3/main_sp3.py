
from moviepy.video.io.VideoFileClip import VideoFileClip
import subprocess
import json


class SP3():

    def trim_video(self,input_file, output_file):
        # Load the video clip
        video_clip = VideoFileClip(input_file)

        # Trim the video to the specified time range
        start_time, end_time = 55, 75
        trimmed_clip = video_clip.subclip(start_time, end_time)

        # Write the trimmed video to a new file
        trimmed_clip.write_videofile(output_file, codec="libx264", audio_codec="aac")

        # Close the video clip objects
        video_clip.close()
        trimmed_clip.close()

    def convert_res(self,input_file,resolution, output_file):

        command_res = [
            'ffmpeg',
            '-i', input_file,
            '-vf', f'scale={resolution}',
            '-c:a', 'copy',
            output_file, 
        ]

        subprocess.run(command_res)


    def convert_codec(self,input_file,codec,output_file):

        command_cod = [
            'ffmpeg',
            '-i', input_file,
            '-c:v', codec,
            '-c:a', 'copy',
            output_file, 
        ]

        subprocess.run(command_cod)

     #Ex1: Convert to different resolutions and codecs
    def convert_all_ex1(self,input_file = 'BigBuckBunny.mp4'):
    
        resolutions = { '720p' :'1280x720', '480p' : '640x480','240p' : '360x240', '120p' : '160x120'}
        codecs = ['libvpx', 'libvpx-vp9', 'libx265', 'libsvtav1']

        #Trim original video to a 20 sec fragment to speed up operations
        
        output_file = 'video_trimmed.mp4'
        self.trim_video(input_file,output_file)

        #convert to different resolutions
        for res in resolutions:
            input_file = 'BigBuckBunny_trimmed.mp4'
            self.convert_res(input_file,output_file,resolutions[res])
            #convert to different codecs
            for codec in codecs:
                input_file = f'{resolutions[res]}.mp4'
                self.convert_codec(input_file,output=input_file ,codec=codec,res=res)
    
    def compare_ex2(self,input_file1, input_file2,output_file):
        
        # Define the ffmpeg command
        ffmpeg_command = [
            'ffmpeg',
            '-i', input_file1,
            '-i', input_file2,
            '-filter_complex', 'hstack',
            output_file
        ]
        # Run the ffmpeg command
        subprocess.run(ffmpeg_command)

        def extract_yuv_histogram(self, input_file,output_file):

            create_video_with_histogram_command = [
            'ffmpeg',
            '-i', input_file,
            '-vf', 'split=2[a][b],[b]histogram,format=yuva444p[hh],[a][hh]overlay',
            output_file
            ]

            # Step 3: Remove temporary image files
            subprocess.run(create_video_with_histogram_command)
        
        def modify_chroma_subsampling(self,input_file, chroma_subsampling,output_file):
            
            # FFmpeg command to modify chroma subsampling
            chromasub_cmd = [
                'ffmpeg',
                '-i', input_file,  # Input video file
                '-vf', f'format={chroma_subsampling}',  # Change chroma subsampling
                '-c:v', 'libx264',  # Set the video codec (change as needed)
                '-q:v', '2',  # Set video quality (change as needed)
                output_path  # Output video file
            ]

            subprocess.run(chromasub_cmd)

        
