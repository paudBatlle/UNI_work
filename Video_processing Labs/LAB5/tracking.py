import cv2
import numpy as np


class BoundingBox:
    def __init__(self, bbox, bb_format='xy'):
        if bb_format == 'xy':
            self.bbox = np.asarray(bbox)
        else:
            self.bbox = np.asarray([bbox[1], bbox[0], bbox[3], bbox[2]])

    @property
    def x1(self):
        return self.bbox[0]

    @property
    def y1(self):
        return self.bbox[1]

    @property
    def x2(self):
        return self.bbox[2]

    @property
    def y2(self):
        return self.bbox[3]

    @property
    def left(self):
        return self.bbox[0]

    @property
    def top(self):
        return self.bbox[1]

    def height(self):
        return self.y2 - self.y1

    def width(self):
        return self.x2 - self.x1

    def size(self):
        return self.width(), self.height()

    def centroid(self):
        return np.asarray([(self.x1 + self.x2) // 2,
                           (self.y1 + self.y2) // 2])

    def __str__(self):
        return f'BoundingBox({self.bbox})'


class DetectionBox(BoundingBox):
    def __init__(self, id_, bbox, bb_format='xy'):
        if hasattr(bbox, 'bbox'):
            bbox = np.copy(bbox.bbox)
            bb_format = 'xy'
        super().__init__(bbox, bb_format)
        self.id_ = id_

    def __str__(self):
        return f'DetectionBox({self.id_}, {self.bbox})'


def assign_colors(ids, colormap=None):
    num_ids = len(ids)
    if num_ids == 1 and not colormap:
        return {ids[0]: (255, 0, 0)}

    if not colormap:
        import matplotlib.cm as cm
        colormap = cm.rainbow

    colors = 255 * colormap(np.linspace(0, 1, num_ids))[:, :3]
    colors = np.clip(colors.astype(np.int32), 0, 255)
    np.random.shuffle(ids)

    return {ids[i]: tuple([c.item() for c in color]) for i, color in enumerate(colors)}


def draw_boxes(frame, bboxes, color=None, thickness=2, show_centroid=False, radius=5, copy=True):
    if copy:
        frame = np.copy(frame)

    if not isinstance(bboxes, list):
        bboxes = [bboxes]

    if not color:
        color = (255, 0, 0)

    for bb in bboxes:
        x1, y1, x2, y2 = map(int, bb.bbox)
        cv2.rectangle(frame, (x1, y1), (x2, y2), color, thickness)

        if show_centroid:
            centroid = bb.centroid().astype(np.int32)
            cv2.circle(frame, centroid, radius, color, thickness)
    return frame


def draw_detections(frame, detection_boxes, frame_id=None, colors=None, thickness=2, show_id=True, font_scale=1,
                    show_centroid=False, radius=5, colormap=None, frame_id_color=(255, 255, 0), copy=True):
    if copy:
        frame = np.copy(frame)

    if frame_id is not None:
        cv2.putText(frame, f'{frame_id:04}', (50, 50), cv2.FONT_HERSHEY_SCRIPT_SIMPLEX, 1.5, frame_id_color, 3)

    if not detection_boxes:
        return frame

    if not isinstance(detection_boxes, list):
        detection_boxes = [detection_boxes]

    if not colors:
        colors = assign_colors([bb.id_ for bb in detection_boxes], colormap)

    for bb in detection_boxes:
        x1, y1, x2, y2 = map(int, bb.bbox)
        color = colors[bb.id_]
        cv2.rectangle(frame, (x1, y1), (x2, y2), color, thickness)

        if show_centroid:
            centroid = bb.centroid().astype(np.int32)
            cv2.circle(frame, centroid, radius, color, thickness)

        if show_id:
            cv2.putText(frame, f'{bb.id_}', (x1, y1 - 5), cv2.FONT_HERSHEY_SCRIPT_SIMPLEX, font_scale, color, thickness)

    return frame


def draw_tracks(frames, detections, colors=None, thickness=2, show_id=True, font_scale=1, show_centroid=False, radius=5,
                colormap=None, show_frame_id=True, frame_id_color=(255, 255, 0), copy=True):
    if not colors:
        ids = set()
        for frame_id, bboxes in detections.items():
            for bb in bboxes:
                ids.add(bb.id_)
        ids = list(ids)

        colors = assign_colors(ids, colormap)

    draw_frames = []
    frame_id = None
    for i in range(len(frames)):
        bboxes = detections.get(i, None)

        if show_frame_id:
            frame_id = i

        frame = draw_detections(frames[i], bboxes, frame_id, colors, thickness, show_id, font_scale, show_centroid,
                                radius, colormap, frame_id_color, copy)
        draw_frames.append(frame)
    return draw_frames
