import cv2
import numpy as np
import sys
import time

#funcsion detected QRcode
def display(im, bbox):
    n = len(bbox)
    for j in range(n):
        cv2.line(im, tuple(bbox[j][0]), tuple(bbox[ (j+1) % n][0]), (255,0,0), 3)

    # Display results
    cv2.imshow("Results", im)

cam = cv2.VideoCapture(0)

cv2.namedWindow("test")

img_counter = 0


while True:
    
    ret, frame = cam.read()          #init camera
    
#---------if camera not init----------
    if not ret:
        print("failed to grab frame")
        break
#-------------------------------------
    
    cv2.imshow("test", frame)         #init window

#----------button commands-----------
    k = cv2.waitKey(1)                #chek butto is presing
    
    if k%256 == 27:                   #if ESC pressed window exit
        print("Escape hit, closing...")
        break
    
    elif k%256==13:                   #if enter detecter QRcode
        print("QRcde detecting")
        inputImage=frame
        qrDecoder = cv2.QRCodeDetector()
        data,bbox,rectifiedImage = qrDecoder.detectAndDecode(inputImage)
        if len(data)>0:
            print("Decoded Data : {}".format(data))
#            display(inputImage, bbox)
            rectifiedImage = np.uint8(rectifiedImage);
            cv2.imshow("Rectified QRCode", rectifiedImage);
        else:
            print("QR Code not detected")
#            cv2.imshow("Results", inputImage)
            
    elif k%256 == 32:                   #if spase create photo
        img_name = "opencv_frame_{}.png".format(img_counter)
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))
        img_counter += 1

cam.release()

cv2.destroyAllWindows()