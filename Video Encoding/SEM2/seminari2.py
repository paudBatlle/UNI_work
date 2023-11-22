from moviepy.video.io.VideoFileClip import VideoFileClip
import subprocess
import json
from yuv_hist import yuv
from subtitles import Subtitles

class Semi2():
    def trim_video(self,input_file, output_file):
        # Load the video clip
        video_clip = VideoFileClip(input_file)

        # Trim the video to the specified time range
        start_time, end_time = 60, 69
        trimmed_clip = video_clip.subclip(start_time, end_time)

        # Write the trimmed video to a new file
        trimmed_clip.write_videofile(output_file, codec="libx264", audio_codec="aac")

        # Close the video clip objects
        video_clip.close()
        trimmed_clip.close()

    def generate_motion_vector_video(self,input_file, output_file):
        # Define the ffmpeg command
        
        #ffplay -flags2 +export_mvs input.mp4 -vf codecview=mv_type=fp
        #ffmpeg -flags2 +export_mvs -i input.mp4 -vf codecview=mv=pf+bf+bb output.mp4
        '''
        ffplay_command = [
            'ffplay',
            '-flags2', '+export_mvs',
            input_file,
            '-vf', 'codecview=mv_type=fp',
        ]
        '''
        ffmpeg_command = [
            'ffmpeg',
            '-flags2', '+export_mvs',
            '-i', input_file,
            '-vf', 'codecview=mv_type=fp',
            output_file
        ]
        # Run the ffmpeg command
        subprocess.run(ffmpeg_command)

    def create_bbb_container(self,input_file, output_file):
        # Define the ffmpeg commands for each requirement
        cut_video_command = [
            'ffmpeg',
            '-i', input_file,
            '-t', '50',
            '-c:v', 'libx264',
            '-c:a', 'aac',
            '-strict', 'experimental',
            'temp_video.mp4'
        ]

        export_mono_audio_command = [
            'ffmpeg',
            '-i', 'temp_video.mp4',
            '-vn',
            '-ac', '1',
            'mono_audio.mp3'
        ]

        export_stereo_audio_low_bitrate_command = [
            'ffmpeg',
            '-i', 'temp_video.mp4',
            '-vn',
            '-ac', '2',
            '-b:a', '64k',
            'stereo_audio_low_bitrate.mp3'
        ]

        export_aac_audio_command = [
            'ffmpeg',
            '-i', 'temp_video.mp4',
            '-vn',
            '-c:a', 'aac',
            'audio.aac'
        ]
        '''
        merge_command = [
            'ffmpeg',
            '-i', 'temp_video.mp4',
            '-i', 'mono_audio.mp3',
            '-i', 'stereo_audio_low_bitrate.mp3',
            '-i', 'audio.aac',
            '-filter_complex', '[0:v][1:a][2:a][3:a]concat=n=4:v=1:a=1[v][aout]',
            '-map', '[v]',
            '-map', '[aout]',
            '-c:v', 'libx264',
            '-c:a', 'aac',
            output_file
        ]
        '''
        merge_command = [
            'ffmpeg',
            '-i', 'temp_video.mp4',
            '-i', 'mono_audio.mp3',
            '-i', 'stereo_audio_low_bitrate.mp3',
            '-i', 'audio.aac',
            '-filter_complex', 'amix=inputs=4:duration=longest',
            output_file
        ]

        try:
            # Cut video
            subprocess.run(cut_video_command)

            # Export mono audio
            subprocess.run(export_mono_audio_command)

            # Export stereo audio with lower bitrate
            subprocess.run(export_stereo_audio_low_bitrate_command)

            # Export audio with AAC codec
            subprocess.run(export_aac_audio_command)

            # Merge everything into a final video container
            subprocess.run(merge_command)
        finally:
            # Clean up temporary files
            subprocess.run(['rm', 'temp_video.mp4', 'mono_audio.mp3', 'stereo_audio_low_bitrate.mp3', 'audio.aac'])
    
    def get_track_count(self,mp4_file):
        # Run ffprobe to get information about the tracks
        ffprobe_command = [
            'ffprobe',
            '-v', 'error',
            '-show_entries', 'format=nb_streams',
            '-of', 'json',
            mp4_file
        ]

        try:
            # Run the ffprobe command and capture the output
            result = subprocess.run(ffprobe_command, capture_output=True, text=True)
            
            # Parse the JSON output
            ffprobe_data = json.loads(result.stdout)

            # Get the number of streams (tracks)
            track_count = ffprobe_data['format']['nb_streams']

            return track_count

        except Exception as e:
            print(f"Error: {e}")
            return None
            
S2 = Semi2()

#ex 1
#Trim video from second 5 to 14
input_video_path = "BigBuckBunny.mp4"
output_video_path = "BigBuckBunny_short.mp4"

S2.trim_video(input_video_path, output_video_path)

#generate motion vector video
input_video_path = "BigBuckBunny_short.mp4"
output_video_path = "BigBuckBunny_motion_vector.mp4"

S2.generate_motion_vector_video(input_video_path, output_video_path)


#ex 2
input_video_path = "BigBuckBunny.mp4"
output_bbb_container_path = "bbb_container.mp4"

S2.create_bbb_container(input_video_path, output_bbb_container_path)


#ex3
mp4_file_path = "BigBuckBunny_short.mp4"
tracks_count = S2.get_track_count(mp4_file_path)

if tracks_count is not None:
    print(f"The MP4 container contains {tracks_count} tracks.")
else:
    print("Failed to determine the number of tracks.")

#ex4,5

sub = Subtitles()

input_video_path = "BigBuckBunny.mp4"
output_video_path = "bbb_with_subtitles.mp4"

sub.getSubtitles(input_video_path, output_video_path)

#ex 6
yuv1 = yuv()
input_video_path = "BigBuckBunny_short.mp4"
output_video_path = "bbb_with_histogram.mp4"
yuv1.extract_yuv_histogram(input_video_path, output_video_path)
