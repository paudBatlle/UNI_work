import subprocess
class yuv():
    def extract_yuv_histogram(self, input_file, output_file):
        '''
        # Step 1: Extract YUV histogram as an image sequence
        extract_histogram_command = [
            'ffmpeg',
            '-i', input_file,
            '-vf', 'split=2[h1][h2];[h1]waveform=mode=column:components=3[hist];[h2][hist]hstack',
            '-vsync', '0',
            'histogram_%04d.png'
        ]

        subprocess.run(extract_histogram_command)

        # Step 2: Create a video from the image sequence with YUV histogram
        create_video_with_histogram_command = [
            'ffmpeg',
            '-framerate', '30',  # Adjust the framerate as needed
            '-pattern_type', 'glob',  # Use glob pattern type
            '-i', 'histogram_*.png',  # Use glob pattern to include all files
            '-i', input_file,
            '-filter_complex', '[1:v][0:v] overlay=W-w-10:H-h-10,setsar=1 [v]',
            '-c:a', 'copy',
            output_file
        ]

        subprocess.run(create_video_with_histogram_command)
        '''
        create_video_with_histogram_command = [
        'ffmpeg',
        '-i', input_file,
        '-vf', 'split=2[a][b],[b]histogram,format=yuva444p[hh],[a][hh]overlay',
        output_file
        ]

        # Step 3: Remove temporary image files
        subprocess.run(create_video_with_histogram_command)


yuv1 = yuv()

# Example usage: Extract YUV histogram and create a new video container
input_video_path = "BigBuckBunny_short.mp4"
output_video_path = "output_video_with_histogram.mp4"
yuv1.extract_yuv_histogram(input_video_path, output_video_path)
