import os
import shutil
import zipfile
from datetime import datetime
from pathlib import Path
from zipfile import ZipFile
from base64 import b64encode
import numpy as np
from tracking import BoundingBox


def create_results(tracking_results, data_dir: Path):
    data_dir.mkdir(parents=True, exist_ok=True)
    for videoname, detections in tracking_results.items():
        video_results = data_dir.joinpath(f'{videoname}.txt')
        with video_results.open(mode='w') as fid:
            for frame_id, detection_boxes in detections.items():
                for d in detection_boxes:
                    fid.write(f'{frame_id + 1},{d.id_},{d.left},{d.top},{d.width()},{d.height()},1,-1,-1,-1\n')


def zipdir(path, ziph):
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file),
                       os.path.relpath(os.path.join(root, file),
                                       os.path.join(path, '..')))


def create_submission(tracking_results, tracker_name, results_dir='results', benchmark='upf23', split='all'):
    if isinstance(results_dir, str):
        results_dir = Path(results_dir)

    if results_dir.exists():
        shutil.rmtree(results_dir)

    benchmark_dir = results_dir.joinpath(f'{benchmark}-{split}')

    data_dir = benchmark_dir.joinpath(tracker_name, 'data')
    create_results(tracking_results, data_dir)
    
    submission_fpath = datetime.now().strftime(f'submission_{benchmark}-{split}_%Y-%m-%d_%H-%M-%S.zip')
    submission_fpath = results_dir.parent.absolute().joinpath(submission_fpath)    
    zipf = ZipFile(str(submission_fpath), 'w', zipfile.ZIP_DEFLATED)
    zipdir(benchmark_dir, zipf)
    zipf.close() 


def display_video(video_path, width=480, height=360):
    from IPython.display import HTML
    video_file = open(video_path, "r+b").read()
    video_url = f"data:video/mp4;base64,{b64encode(video_file).decode()}"
    return HTML(f"""<video width={width} height={height} controls><source src="{video_url}"></video>""")


def load_semantic_segmentation(semantic_seg_path, score_threshold):
    semantic_seg = np.load(semantic_seg_path, allow_pickle=True)

    bboxes, masks = [], []
    for i in range(len(semantic_seg)):
        frame_bboxes, frame_masks = [], []
        for bb, mask, score in zip(semantic_seg[i]['boxes'],
                                   semantic_seg[i]['masks'],
                                   semantic_seg[i]['scores']):
            if score > score_threshold:
                frame_bboxes.append(BoundingBox(bb))
                frame_masks.append(mask)

        bboxes.append(frame_bboxes)
        masks.append(frame_masks)
    return bboxes, masks


def plot_similarity_matrix(matrix, figsize=(7,7)):
    import matplotlib.pyplot as plt
    
    fig, ax = plt.subplots(figsize=(7,7))
    cax = ax.matshow(matrix, cmap=plt.cm.Reds)
    fig.colorbar(cax)
    for element in range(matrix.size):
        i, j = np.unravel_index(element, matrix.shape)
        ax.text(j, i, f'{matrix[i,j]:.1f}', va='center', ha='center')
    plt.xlabel('Next Bounding Boxes', fontsize=16)
    plt.ylabel('Previous Detection Boxes', fontsize=16)
    ax.xaxis.tick_top()
    ax.xaxis.set_label_position('top')
    plt.show()
