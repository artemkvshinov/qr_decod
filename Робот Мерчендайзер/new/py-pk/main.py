import cv2                              # библиотека opencv (получение и обработка изображения)
import mediapipe as mp                  # библиотека mediapipe (распознавание рук)


camera = cv2.VideoCapture(0)            # получаем изображение с камеры (0 - порядковый номер камеры в системе)
mpHands = mp.solutions.hands            # подключаем раздел распознавания рук
hands = mpHands.Hands()                 # создаем объект класса "руки"
mpDraw = mp.solutions.drawing_utils     # подключаем инструменты для рисования

face_cascade_db = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")


p = [0 for i in range(21)]              # создаем массив из 21 ячейки для хранения высоты каждой точки
finger = [0 for i in range(5)]          # создаем массив из 5 ячеек для хранения положения каждого пальца

# функция, возвращающая расстояние по модулю (без знака)
def distance(point1, point2):
    return abs(point1 - point2)


while True:
    good, img = camera.read()                                   # получаем один кадр из видеопотока
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)               # преобразуем кадр в RGB


    results = hands.process(imgRGB)                             # получаем результат распознавания
    if results.multi_hand_landmarks:                            # если обнаружили точки руки
        for handLms in results.multi_hand_landmarks:            # получаем координаты каждой точки

            # при помощи инструмента рисования проводим линии между точками
            mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)

            # работаем с каждой точкой по отдельности
            # создаем список от 0 до 21 с координатами точек
            for id, point in enumerate(handLms.landmark):
                # получаем размеры изображения с камеры и масштабируем
                width, height, color = img.shape
                width, height = int(point.x * height), int(point.y * width)

                p[id] = height           # заполняем массив высотой каждой точки

            # получаем расстояние, с которым будем сравнивать каждый палец
            distanceGood = distance(p[0], p[5]) + (distance(p[0], p[5]) / 2)
            # заполняем массив 1 (палец поднят) или 0 (палец сжат)
            finger[1] = 1 if distance(p[0], p[8]) > distanceGood else 0
            finger[2] = 1 if distance(p[0], p[12]) > distanceGood else 0
            finger[3] = 1 if distance(p[0], p[16]) > distanceGood else 0
            finger[4] = 1 if distance(p[0], p[20]) > distanceGood else 0
            finger[0] = 1 if distance(p[4], p[17]) > distanceGood else 0

    # img = cv2.imread("IMG_20191012_145410_3.jpg")
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    faces = face_cascade_db.detectMultiScale(img_gray, 1.1, 19)
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # cv2.waitKey()

    cv2.imshow("Image", img)           # выводим окно с нашим изображением
    if cv2.waitKey(1) == ord('q'):     # ждем нажатия клавиши q в течение 1 мс
        break                          # если нажмут, всё закрываем