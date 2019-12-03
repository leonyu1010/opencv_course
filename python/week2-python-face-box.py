import cv2

faceRoi = {}

def HandleMouse(action, x, y, flags, userData):
    if(action == cv2.EVENT_LBUTTONDOWN):
        faceRoi["TopLeft"] = (x, y)
    elif(action == cv2.EVENT_LBUTTONUP):
        faceRoi["BottomRight"] = (x, y)


def ProcessFace(img):
    if("TopLeft" not in faceRoi):
        return

    cv2.circle(img, faceRoi["TopLeft"], 1, (255, 255, 0))

    if("BottomRight" not in faceRoi):
        return

    cv2.rectangle(img, faceRoi["TopLeft"], faceRoi["BottomRight"], (255, 255, 0), 2)
    SaveFaceImage(img)


def SaveFaceImage(img):
    rowStart = faceRoi["TopLeft"][1]
    rowEnd = faceRoi["BottomRight"][1] + 1
    colStart = faceRoi["TopLeft"][0]
    colEnd = faceRoi["BottomRight"][0] + 1
    cv2.imwrite("./face_result.png", img[rowStart:rowEnd, colStart:colEnd])
    faceRoi.clear()


def main():
    windowName = "Sample"
    cv2.namedWindow(windowName)
    cv2.setMouseCallback(windowName, HandleMouse )

    img = cv2.imread("./sample.jpg", 1)
    cv2.putText(img, "Choose top-left corner and drag to select face.",
                (10, img.shape[0] - 20),
                cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 255), 1, cv2.LINE_AA)

    while(True):
        cv2.imshow(windowName, img)
        key = cv2.waitKey(500) & 0xFF
        if(key == 27):
            break;
        ProcessFace(img)

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()