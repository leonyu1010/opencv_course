import cv2
import numpy as np
import math

class DocScanner:
    def __init__(self, file):
        self.file = file
        self.final_image = None

    def run(self):
        self.image = cv2.imread(file, cv2.IMREAD_COLOR)
        edges = self.detect_edges(self.image)
        doc_contour = self.find_contour_of_maxArea(edges)
        width, height, dst_corners = self.setup_dst_corners()
        output = self.warp_corners(doc_contour, dst_corners, width, height)


        cv2.imshow("output", output)
        cv2.waitKey(0)

    def setup_dst_corners(self):
        dst_width = 500
        dst_height = int(500* 1.414)
        dst_top_left = (0, 0)
        dst_top_right = (dst_width, 0)
        dst_bottom_left = (0,dst_height)
        dst_bottom_right = (dst_width, dst_height)

        dst_corners = np.array([dst_top_left, dst_bottom_left, dst_bottom_right, dst_top_right], dtype=int)
        return (dst_width, dst_height, dst_corners)

    def detect_edges(self, image):
        lowThreshold = 10
        highThreshold = 40
        apertureSize = 5
        blurAmount = 3

        blurredImg = cv2.GaussianBlur(image, (2 * blurAmount + 1, 2 * blurAmount + 1), 0)
        edges = cv2.Canny(blurredImg, lowThreshold, highThreshold, apertureSize)     
        cv2.imshow("edges", edges)
        cv2.waitKey(1)

        return edges

    def find_contour_of_maxArea(self, edges):
        contours0, hierarchy = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        maxArea = -1
        contourId = -1
        area = -1
  
        for id, cnt in enumerate(contours0) :
            area = cv2.contourArea(cnt)
            if area > maxArea :
                maxArea = area
                contourId = id
        
        doc_contour = cv2.approxPolyDP(contours0[contourId], 0.05*cv2.arcLength(contours0[contourId],True), True)   
        print(doc_contour)

        contours = [doc_contour]
        cv2.drawContours(self.image, contours, 0, (0,0,255), 3, cv2.LINE_AA, hierarchy, 0)
        cv2.imshow("input", self.image)
        cv2.waitKey(0)
        return doc_contour

    def calDistance(self, pt1, pt2):
        distance = math.sqrt(math.pow((pt1[0]-pt2[0]), 2) + math.pow((pt1[1]-pt2[1]), 2))
        return distance

    def warp_corners(self, src_corners, dst_corners, dst_width, dst_height):    
        pts = []
        for pt in src_corners:
            pts.append((pt[0][0], pt[0][1]))

        distance_12 = self.calDistance(pts[0], pts[1])
        distance_23 = self.calDistance(pts[1], pts[2]) 

        if distance_12 < distance_23 :
            h, status = cv2.findHomography(np.array([pts[1], pts[2], pts[3], pts[0]]), dst_corners, cv2.RANSAC, 3)
        else:
            h, status = cv2.findHomography(np.array([pts[0], pts[1], pts[2], pts[3]]), dst_corners, cv2.RANSAC, 3)

        self.final_image = cv2.warpPerspective(self.image, h, (dst_width, dst_height))
        return self.final_image
 

if __name__ == '__main__':
    import sys
    try: 
        file = sys.argv[1]
    except:
        file = "book3.jpg"
    for file in ["book1.jpg", "book3.jpg", "book2.jpg", "book4.jpg"]:
        try:
            DocScanner(file).run()
            cv2.destroyAllWindows()
        except:
            pass
            cv2.destroyAllWindows()
