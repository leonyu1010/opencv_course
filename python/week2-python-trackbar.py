import cv2

imageFile = "./truth.png"

ScaleValue = 0
ScaleType = 0

def HandleScaleValue(value):
    global ScaleValue
    ScaleValue = value


def HandleScaleType(value):
    global ScaleType
    ScaleType =value


def ProcessResize(value, up, img):
    if(value == 0 or (not up and (value == 100))):
        return img

    factor = 1.0
    if(up):
        factor += value / 100.0
    else:
        factor -= value / 100.0

    return cv2.resize(img, None, fx=factor, fy=factor)


def main():
    global ScaleValue, ScaleType

    windowName = "Truth"
    maxScaleValue = 100
    maxScaleType = 1
    ScaleValueBar = "Scale: "
    ScaleValueType = "Type: 0 Up; 1 Down "

    cv2.namedWindow(windowName, cv2.WINDOW_AUTOSIZE)

    cv2.createTrackbar(ScaleValueBar, windowName, ScaleValue, maxScaleValue, HandleScaleValue)
    cv2.createTrackbar(ScaleValueType, windowName, ScaleType, maxScaleType, HandleScaleType)

    img = cv2.imread(imageFile, 1)
    originalImg  = img.copy()

    while(True):
        cv2.imshow(windowName, img)

        key = cv2.waitKey(50)
        if(key == 27):
            break

        up = ScaleType == 0

        if( not up and ScaleValue == 100):
            cv2.setTrackbarPos(ScaleValueBar, windowName, 0)

        img = ProcessResize(ScaleValue, up , originalImg)

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()