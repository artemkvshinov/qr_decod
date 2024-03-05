from prettytable import PrettyTable
import os
import datetime
import cv2
import urllib.request
import numpy as np
import time

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



table = PrettyTable()
table.field_names = ["Продукт", 
                    "количество в одной упаковке", 
                    "единица измерения",
                    "Дата изготовления",
                    "сегодняшняя дата", 
                    "срок годности в днях",
                    "годен ли срок годности?",
                    "Дата окончания скорка годности",
                    "количество упоковок"]
Data = []

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
            current_date = datetime.date.today()
            IN = data.split("_")
            IN[8] = int(IN[8])
            print('run')
            flag = True
            for i in range(len(Data)):
                print('run check')
                data_change = Data[i].copy()
                if data_change[0] == IN[0] and data_change[3] == IN[3] and data_change[5] == IN[5]:
                    flag = False
                    data_change[8] = data_change[8] + IN[8]
                    Data[i] = data_change
                    table.del_row(i)
                    table.add_row([data_change[0], data_change[1], data_change[2], data_change[3], data_change[4], data_change[5], data_change[6], data_change[7], data_change[8]])
                    print('create new')
            if flag:
                print('edit old')
                data_time_final = 0
                data_time_start = IN[3].split("-")
                for i in range(3): data_time_start[i] = int(data_time_start[i])
                data_time_start = datetime.date(data_time_start[2], data_time_start[1], data_time_start[0])
                if IN[5] == "INF":
                    IN[6] = "Да"
                    IN[7] = "-"
                else :
                    data_time_final = data_time_start + datetime.timedelta(days = int(IN[5]))
                    if current_date >  data_time_final:
                        IN[6] = "НЕТ"
                    if current_date ==  data_time_final:
                        IN[6] = "Да*"
                    if current_date <  data_time_final:
                        IN[6] = "Да"
                    IN[7] = str(data_time_final.strftime('%d-%m-%Y'))
                Data.append(IN)
                IN[4] = str(current_date.strftime('%d-%m-%Y'))
                

                R = "\033[0;31;40m" #RED
                G = "\033[0;32;40m" # GREEN
                Y = "\033[0;33;40m" # Yellow
                B = "\033[0;34;40m" # Blue
                N = "\033[0m" # Reset

                color = ["\033[0;31;40m", "\033[0;32;40m", "\033[0;33;40m", "\033[0;34;40m", "\033[0m"]


                table.add_row([IN[0], IN[1], IN[2], IN[3], IN[4], IN[5], IN[6], IN[7], IN[8]])
            os.system("cls")
            print(table)
        cv2.imshow("Results", img)
        
        #cv2.waitKey(0)
        #cv2.destroyAllWindows()

        if cv2.waitKey(1) == 27:
            exit(0)
    
