from skvideo import io as video_io
import numpy as np
import cv2
from base64 import b64encode

# this function reads a video and outputs the list of frames in grayscale
def read_gray_video(filepath, num_frames=None):
    cap = cv2.VideoCapture(filepath)
    
    total_num_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    if num_frames:
        num_frames = min(num_frames, total_num_frames)
    else:
        num_frames = total_num_frames
    
    frames, i = [], 0
    while cap.isOpened() and i < num_frames:
        ret, frame = cap.read()
        if ret:
            gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            frames.append(gray_frame)
        else:
            break
        i += 1
    cap.release()
    return frames

# this function receives a list of frames and adds a white Gaussian noise with a given level (standard deviation)
def add_noise(gray_frames, noise_level=8):
    noisy_frames = []
    for f in gray_frames:
        noisy_frame = f.astype(np.float64) + noise_level*np.random.randn(*gray_frames[1].shape)
        noisy_frame = np.uint8(np.clip(noisy_frame, 0, 255))
        noisy_frames.append(noisy_frame)
    return noisy_frames

def write_gray_video(filename, video, fps):
    with video_io.FFmpegWriter(filename, outputdict={'-pix_fmt': 'yuv420p', '-r': f'{fps}'}) as writer:
        for frame in video:
            writer.writeFrame(cv2.cvtColor(frame,cv2.COLOR_GRAY2RGB))

def write_gray_video2(filename, video, fps):
    height, width = video[0].shape
    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    out = cv2.VideoWriter(filename, fourcc, fps, (width, height))
    
    for frame in video:
        out.write(cv2.cvtColor(frame,cv2.COLOR_GRAY2BGR))

    out.release()
    
def display_video(video_path, width=480, height=360):
    from IPython.display import HTML
    video_file = open(video_path, "r+b").read()
    video_url = f"data:video/mp4;base64,{b64encode(video_file).decode()}"
    return HTML(f"""<video width={width} height={height} controls><source src="{video_url}"></video>""")
