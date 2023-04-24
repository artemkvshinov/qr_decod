import os
import cv2
import urllib.request
import numpy as np
import time
from tabulate import tabulate
table_name = []
table_num = []
return_data =[
    table_name,
    table_num
]
def LinearSearch(lys, element):
    for i in range (len(lys)):
        if lys[i] == element:
            return i
    return -1

stream = urllib.request.urlopen(input("введите ip адрес камеры "))
bytes = bytes()
last_time = 0

def display(im, bbox):
    n = len(bbox)
    for j in range(n):
        cv2.line(im, tuple(bbox[j][0]), tuple(bbox[ (j+1) % n][0]), (255,0,0), 3)
 
    # Display results
    cv2.imshow("Results", im)


while True:
    bytes += stream.read(1024)
    a = bytes.find(b'\xff\xd8')
    b = bytes.find(b'\xff\xd9')
    if a != -1 and b != -1:
        jpg = bytes[a:b+2]
        bytes = bytes[b+2:]
        img = cv2.imdecode(np.fromstring(jpg, dtype=np.uint8), cv2.IMREAD_COLOR)

        qrDecoder = cv2.QRCodeDetector()
        
        # Detect and decode the qrcode
        data,bbox,rectifiedImage = qrDecoder.detectAndDecode(img)
        if len(data)>0 and time.time() - last_time >= 2:
            os.system("cls")
            last_time = time.time()
            #print("Decoded Data : {}".format(data))
            #display(img, bbox)
            rectifiedImage = np.uint8(rectifiedImage)
            #cv2.imshow("Rectified QRCode", rectifiedImage);
            if data.startswith("product_") :
                if data.lstrip('product_')  in table_name:
                    table_num[LinearSearch(table_name,data.lstrip('product_'))]+=1
                    #print("OK")

                else:
                    table_name.append(data.lstrip('product_'))
                    table_num.append(1)
            #print(table_name,"\n",table_num)
            print(tabulate(return_data,  tablefmt="grid"))
        #else:
            #print("QR Code not detected")
        cv2.imshow("Results", img)
        
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()

        if cv2.waitKey(1) == 27:
            exit(0)