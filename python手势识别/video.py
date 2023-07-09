import cv2
import mediapipe as mp
import time

pTime = 0
cTime = 0

# 设定绘图函数：绘制手部图形
mp_drawing = mp.solutions.drawing_utils

# 设定手势识别函数：识别手部
mp_hands = mp.solutions.hands

# 确定手势识别参数
hands = mp_hands.Hands(
        static_image_mode=False,
        max_num_hands=2,
        min_detection_confidence=0.8,
        min_tracking_confidence=0.8)

# 读取摄像头
cap = cv2.VideoCapture(0)

# 通过循环将每一帧图片读取出来，并显示在屏幕
while True:
    # read方法返回两个参数
    # ret 判断摄像头是否打开成功，frame 为读取的每一帧的图像对象
    ret,frame = cap.read()
    if not ret:
        print('摄像头打开失败')
        break

    frame= cv2.flip(frame,1)   # 水平镜像翻转， 0为开启上下镜像 1为开启左右镜像 -1为左右并上下镜像
    
    results = hands.process(frame) # 获取手势识别结果

    handLmsStyle = mp_drawing.DrawingSpec(color=(255, 255, 255), thickness=10)	# 点的样式
    handConStyle = mp_drawing.DrawingSpec(color=(255, 0, 0), thickness=8)	# 线的样式
    
    if results.multi_hand_landmarks:
        # 通过for循环遍历出所有手的全部信息点，然后就可以对信息点进行绘制
        for hand_landmarks in results.multi_hand_landmarks:
            print('hand_landmarks:',hand_landmarks) #显示每个关键点的坐标
            #绘制关键点
            mp_drawing.draw_landmarks(
                frame, hand_landmarks, mp_hands.HAND_CONNECTIONS, handLmsStyle, handConStyle)

            pt_list = []
            # 获取21个手部关键点坐标
            for lm in hand_landmarks.landmark:
                h,w,c = frame.shape	                # 获取图形的高、宽、颜色
                pt = (int(lm.x*w),int(lm.y*h))	    # 以元组的形式保存每一个坐标
                pt_list.append(pt)	                # 把坐标保存到列表中，方便绘图
                print(pt[0],pt[1])
                
            # 绘制关键点并标号   
            for i in range(21):
                cv2.circle(frame,pt_list[i],5,(0,255,255),-1)
                cv2.putText(frame,str(i),pt_list[i],cv2.FONT_HERSHEY_SIMPLEX,0.9,(0,255,0),3)
        
    else:
        print("没有检测到手...")
    
    
    # 显示FPS
    cTime = time.time()		# 获取当前时间戳
    fps = 1/(cTime-pTime)	# 用1/（当前时间戳 - 前一刻时间戳） = 帧数（频率）
    pTime = cTime			# 保存当前时间戳作为前一刻时间戳，方便下一次循环使用
    cv2.putText(frame, f"FPS : {int(fps)}", (50, 100), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 3)
    
    # imshow方法展示窗口，第一个参数为窗口的名字，第二个参数为帧数据
    cv2.imshow('MediaPipe Hands', frame)
    
    if cv2.waitKey(1) & 0xFF == 27:
        break

cap.release()



    
    
     