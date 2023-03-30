import cv2


def calculate_patch_hist(frame, bb):
    patch = frame[bb.y1:bb.y2, bb.x1:bb.x2, :]
    
    # The histogram consists of 512 bins (quantize the RGB space into cubes by grid 8*8*8)
    hist = cv2.calcHist([patch], [0, 1, 2], None, [8, 8, 8], [0, 256, 0, 256, 0, 256])
    return hist.flatten() / hist.sum()


def bhattacharyya_dist(hist1, hist2):
    return cv2.compareHist(hist1, hist2, cv2.HISTCMP_BHATTACHARYYA)
