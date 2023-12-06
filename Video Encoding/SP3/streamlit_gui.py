import streamlit as st
from main_sp3 import SP3

class VideoConverterStreamlit:
    def __init__(self):
        self.input_file = None
        self.output_video_paths = []

    def select_file(self):
        file_path = st.file_uploader("Select a Video File")
        if file_path:
            self.input_file = file_path.name
            st.success(f"Selected file: {self.input_file}")

    def convert_resolutions(self):
        if self.input_file:
            sp3 = SP3()
            resolutions = {'720p': '1280x720', '480p': '640x480', '240p': '360x240', '120p': '160x120'}
            for res in resolutions:
                output_file = f'{res}.mp4'
                sp3.convert_res(self.input_file, resolutions[res],output_file)
                self.output_video_paths.append(output_file)
            st.success("Video converted to different resolutions successfully.")
        else:
            st.error("Please select a video file first.")

    def convert_codecs(self):
        if self.input_file:
            sp3 = SP3()
            codecs = ['libvpx', 'libvpx-vp9', 'libx265', 'libsvtav1']
            for codec in codecs:
                if codec == 'libvpx' or codec == 'libvpx-vp9':
                    output_file = f'{codec}.webm'
                else:
                    output_file = f'{codec}.mp4'
                sp3.convert_codec(self.input_file,codec,output_file)
            st.success("Video converted to different codecs successfully.")
            self.output_video_paths.append(output_file)
        else:
            st.error("Please select a video file first.")

    def compare_videos(self):
        if self.input_file:
            sp3 = SP3()
            codecs = ['libx265', 'libsvtav1']

            for codec in codecs:
                output_file = f'{codec}.mp4'
                sp3.convert_codec(self.input_file,codec,output_file)

            # Choose two videos for comparison
            file1 = f'{codecs[0]}.mp4'
            file2 = f'{codecs[1]}.mp4'
            output_file = output_file = 'comparison.mp4'
            sp3.compare_ex2(file1, file2,output_file)
            self.output_video_paths.append(output_file)

        else:
            st.error("Please select a video file first.")

    def extract_yuv_histogram(self):
        if self.input_file:
            sp3 = SP3()
            output_file = 'yuv_histogram.mp4'
            sp3.extract_yuv_histogram(self.input_file,output_file)
            st.success("YUV Histogram extracted successfully.")
            self.output_video_paths.append(output_file)
        else:
            st.error("Please upload a video file first.")

    def modify_chroma_subsampling(self):
        if self.input_file:
            sp3 = SP3()
            output_file = 'chroma_subsampled.mp4'
            chroma_subsampling = st.selectbox("Chroma Subsampling", ["4:4:4", "4:2:2", "4:2:0"])
            sp3.modify_chroma_subsampling(self.input_file, chroma_subsampling,output_file)
            self.output_video_paths.append(output_file)
            st.success("Chroma subsampling modified successfully.")
        else:
            st.error("Please upload a video file first.")
    
    def trim_video_file(self):
        if self.input_file:
            output_path = 'trimmed_video.mp4'
            sp3 = SP3()
            sp3.trim_video(self.input_file,output_file = output_path)
            st.success("Video trimmed successfully.")
            self.output_video_paths.append(output_file)

    def display_videos(self):
        if self.output_video_paths:
            st.subheader("Uploaded Video:")
            st.video(self.input_file)

            st.subheader("Converted Videos:")
            for path in self.output_video_paths:
                st.video(path)
        else:
            st.warning("No videos to display. Please perform actions to generate videos.")

    def run(self):
        st.title("Video Encoding Systems SP3 GUI")
        self.select_file()

        convert_resolutions_button = st.button("Convert to Different Resolutions")
        if convert_resolutions_button:
            self.convert_resolutions()
            self.display_videos()
            self.output_video_paths = []

        convert_codecs_button = st.button("Convert to Different Codecs")
        if convert_codecs_button:
            self.convert_codecs()
            self.display_videos()
            self.output_video_paths = []

        compare_videos_button = st.button("Compare Two Videos")
        if compare_videos_button:
            self.compare_videos()
            self.display_videos()
            self.output_video_paths = []
        
        extract_yuv_histogram_button = st.button("Extract YUV Histogram")
        if extract_yuv_histogram_button:
            self.extract_yuv_histogram()
            self.display_videos()
            self.output_video_paths = []

        modify_chroma_subsampling_button = st.button("Modify Chroma Subsampling")
        if modify_chroma_subsampling_button:
            self.modify_chroma_subsampling()
            self.display_videos()
            self.output_video_paths = []
        
        st.subheader("Developed by Pau de Batlle for Video Encoding Systems SP3")


if __name__ == "__main__":
    video_converter = VideoConverterStreamlit()
    video_converter.run()
