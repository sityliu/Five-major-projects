import cv2
import mediapipe as mp
import numpy as np
#计算手部坐标角度
def get_angle(v1,v2):
    #计算余弦距离
    angle = np.dot(v1,v2)/(np.sqrt(np.sum(v1*v1))*np.sqrt(np.sum(v2*v2)))
    #通过余弦距离计算余弦角度
    angle = np.arccos(angle)/3.14*180
    return angle

def get_str_guester(up_fingers,list_lms):
    #当只有一指出现且是食指关键点8
    if len(up_fingers)==1 and up_fingers[0]==8:
        #用v1存取节点6到节点7之间的矢量
        v1 = list_lms[6]-list_lms[7]
        #用v2存取节点8到节点7之间的矢量
        v2 = list_lms[8]-list_lms[7]
        #计算v1、v2之间的夹角
        angle = get_angle(v1,v2)
       
        if angle<160:
            str_guester = "9"
        else:
            str_guester = "1"
    #当只有一指且是大拇指关键点4，显示'Good'
    elif len(up_fingers)==1 and up_fingers[0]==4:
        str_guester = "Good"
    #当只有一指且是小拇指关键点20，显示'Bad'
    elif len(up_fingers)==1 and up_fingers[0]==20:
        str_guester = "Bad"
    #当只有一指且是中指关键点12，显示国际友好手势
    elif len(up_fingers)==1 and up_fingers[0]==12:
        str_guester = "FXXX"
    #当出现两指，且是食指关键点8和中指关键点12,显示2
    elif len(up_fingers)==2 and up_fingers[0]==8 and up_fingers[1]==12:
        str_guester = "2"
    # #当出现两指，是大拇指关键点4和食指关键点8，显示'love'
    # elif len(up_fingers)==2 and up_fingers[0]==4 and  up_fingers[1]==8 :
    #     str_guester = "love"

    #当出现两指，是小拇指关键点20和大拇指关键点4,显示6
    elif len(up_fingers)==2 and up_fingers[0]==4 and up_fingers[1]==20:
        str_guester = "6"
    #当出现两指，是食指关键点8和大拇指4,显示8
    elif len(up_fingers)==2 and up_fingers[0]==4 and up_fingers[1]==8:
        str_guester = "8"
    #当出现三指，是食指关键点8、中指关键点12和无名指关键点16，显示3
    elif len(up_fingers)==3 and up_fingers[0]==8 and up_fingers[1]==12 and up_fingers[2]==16:
        str_guester = "3"
    #当出现三指，是大拇指关键点4、食指关键点8和中指关键点12，进入显示判断
    elif len(up_fingers)==3 and up_fingers[0]==4 and up_fingers[1]==8 and up_fingers[2]==12:
        #计算手指尖的距离
        dis_8_12 = list_lms[8,:] - list_lms[12,:]
        dis_8_12 = np.sqrt(np.dot(dis_8_12,dis_8_12))
        
        dis_4_12 = list_lms[4,:] - list_lms[12,:]
        dis_4_12 = np.sqrt(np.dot(dis_4_12,dis_4_12))
        #判断手指尖的距离
        if dis_4_12/(dis_8_12+1) <3:
            str_guester = "7"
        
        elif dis_4_12/(dis_8_12+1) >5:
            str_guester = "Gun"
        else:
            str_guester = "7"
    #当出现三指，是大拇指关键点4、食指关键点8和小拇指关键点20，显示'ROCK'
    elif len(up_fingers)==3 and up_fingers[0]==4 and up_fingers[1]==8 and up_fingers[2]==20:
        str_guester = "ROCK"
    #当出现四指,是食指关键点8、中指关键点12、无名指关键点16、小拇指关键点20，显示4
    elif len(up_fingers)==4 and up_fingers[0]==8 and up_fingers[1]==12 and up_fingers[2]==16 and up_fingers[3]==20:
        str_guester = "4"
    #当出现五指，
    elif len(up_fingers)==5 :
        str_guester = "5"
    #当没有手指出现，显示10
    elif len(up_fingers)==0:
        str_guester = "10"
    #否则不显示
    else:
        str_guester = " "
    #返回这个字符串
    return str_guester

#main函数进入口
if __name__ == "__main__":
    #打开摄像头
    cap = cv2.VideoCapture(0)
    #设定手势识别函数
    mpHands =  mp.solutions.hands
    #设定手势识别参数(这里按默认设置)
    hands = mpHands.Hands(static_image_mode=False,
               max_num_hands=2,
               model_complexity=1,
               min_detection_confidence=0.5,
               min_tracking_confidence=0.5)
    #定义绘图函数
    mydrawing =  mp.solutions.drawing_utils
    while True:
        #读取摄像头图像
        ret, img = cap.read()
        if not ret:
            continue
        image_hight,image_width,_ = np.shape(img)
        #将BGR转换为RGB格式
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        #镜面翻转
        img = cv2.flip(img,1)
        #读取手部检测结果
        results = hands.process(img)
        #将RGB转回BGR
        img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)

        if results.multi_hand_landmarks:
            hand = results.multi_hand_landmarks[0]

            mydrawing.draw_landmarks(img,hand,mpHands.HAND_CONNECTIONS)
            #采集所有关键点的坐标
            list_lms = []
            for i in range(21):
                #归一化x轴坐标
                pos_x = hand.landmark[i].x*image_width
                #归一化y轴坐标
                pos_y = hand.landmark[i].y*image_hight
                #强制类型转换为整型变量，便于数据处理
                list_lms.append([int(pos_x),int(pos_y)])

            #构造凸包点
            list_lms = np.array(list_lms,np.int32)
            hull_index = [0,1,2,3,6,10,14,19,18,17,10]
            hull = cv2.convexHull(list_lms[hull_index,:])

            #绘制凸包
            cv2.polylines(img,[hull],True,(0,255,0),2)

            #查找外部的点数
            n_fig = -1
            ll = [4,8,12,16,20]
            up_fingers = []
            #收集凸包外指尖的点
            for i in ll:
                pt = (int(list_lms[i][0]),int(list_lms[i][1]))
                dist = cv2.pointPolygonTest(hull,pt,True)
                if dist<0:
                    up_fingers.append(i)
            #函数get_str_guester:定义规则,up_fingers:凸包外的点(手指数量),list_lms:关键点的坐标
            str_guester = get_str_guester(up_fingers,list_lms)
            #显示字符串str_guester(手势名称)的值
            cv2.putText(img,' %s'%(str_guester),(90,90),cv2.FONT_HERSHEY_SIMPLEX,3,(255,255,0),cv2.LINE_AA)

            for i in ll:
                pos_x = hand.landmark[i].x*image_width
                pos_y = hand.landmark[i].y*image_hight
                #画点
                cv2.circle(img,(int(pos_x),int(pos_y)),3,(0,255,0),-1)
        #显示摄像头图像
        cv2.imshow('lwt',img)
        if cv2.waitKey(1) & 0xFF == 27:
            break
#释放摄像头资源
cap.release()
