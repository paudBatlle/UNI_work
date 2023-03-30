import numpy as np
import cv2


def compute_color_flow_map(flow):
    UNKNOWN_FLOW_THRESH = 1e9  # this is how an unknown optical flow is marked in the ground truth
    idx = np.abs(flow) > UNKNOWN_FLOW_THRESH

    flow = np.copy(flow)    
    flow[idx] = 0

    mag, ang = cv2.cartToPolar(flow[..., 0], flow[..., 1])
    hsv = np.zeros(flow.shape[:2] + (3,), dtype=np.uint8)
    hsv[..., 0] = 90 * ang / np.pi
    hsv[..., 1] = cv2.normalize(mag, None, 0, 255, cv2.NORM_MINMAX)
    hsv[..., 2] = 255

    return cv2.cvtColor(hsv, cv2.COLOR_HSV2RGB)


# The following two functions are from:
# https://www.programcreek.com/python/?project_name=ignacio-rocco%2Fweakalign#

def read_flo_file(filename, verbose=False):
    """
        Read from .flo optical flow file (Middlebury format)
        :param flow_file: name of the flow file
        :return: optical flow data in matrix

        adapted from https://github.com/liruoteng/OpticalFlowToolkit/

        """
    with open(filename, 'rb') as f:
        magic = np.fromfile(f, np.float32, count=1)

        if magic != 202021.25:
            raise TypeError('Magic number incorrect. Invalid .flo file')

        w = np.fromfile(f, np.int32, count=1)
        h = np.fromfile(f, np.int32, count=1)
        if verbose:
            print("Reading %d x %d flow file in .flo format" % (h, w))
        data2d = np.fromfile(f, np.float32, count=int(2 * w * h))
        # reshape data into 3D array (columns, rows, channels)
        data2d = np.resize(data2d, (h[0], w[0], 2))
        return data2d


def write_flo_file(flow, filename):
    """
        Write optical flow in Middlebury .flo format

        :param flow: optical flow map
        :param filename: optical flow file path to be saved
        :return: None

        from https://github.com/liruoteng/OpticalFlowToolkit/

        """
    # forcing conversion to float32 precision
    flow = flow.astype(np.float32)
    with open(filename, 'wb') as f:
        magic = np.array([202021.25], dtype=np.float32)
        (height, width) = flow.shape[0:2]
        w = np.array([width], dtype=np.int32)
        h = np.array([height], dtype=np.int32)
        magic.tofile(f)
        w.tofile(f)
        h.tofile(f)
        flow.tofile(f)
