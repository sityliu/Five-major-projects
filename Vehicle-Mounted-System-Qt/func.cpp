#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myslider.h"
#include <QDebug>

extern QTimer *get_pos_timer;


int r = 1;
void MainWindow::random_num(int i)
{
    r = i;
}



/*初始化播放器*/
void MainWindow::PlayerInit()
{
    file_path = "../Vehicle-Mounted-System/videos/";                                                    /*本地视频文件路径*/
    icon_path = "../Vehicle-Mounted-System/材料/";                                                       /*图标文件路径*/
    connect(pro, SIGNAL(readyReadStandardOutput()), this, SLOT(recv_process_data()));                   /*关联播放器的数据与解析数据的函数*/
    connect(ui->Slider_progress, SIGNAL(sliderReleasedAt(int)), this, SLOT(recv_slider_changed(int)));  /*关联滑动块数值改变信号*/

    ui->Slider_progress->setEnabled(false);                             /*设置进度条不可选*/

    //      信号发出者           发出的信号名  信号的接收者      负责接收信号的槽函数
    connect(get_pos_timer, SIGNAL(timeout()), this, SLOT(recv_timeout()));              /*关联超时信号与定时器*/

}

/*添加视频文件*/
void MainWindow::AddVideos(QString filename)
{
    play_list << file_path + filename;                                  /*往播放列表导入视频文件名*/
    new QListWidgetItem(filename.remove(".mp4"), ui->video_list);       /*生成窗口中的播放列表*/
}


/*显示播放按钮图标*/
void MainWindow::SetButtonStyle(QPushButton *button, QString pic)
{
    QIcon myicon;                                                       /*生成图标对象*/
    QString path = icon_path + pic;                                     /*图标路径 + 图标文件名*/
    myicon.addFile(tr(path.toStdString().c_str()));                     /*把图标完整路径类型转换成char*型*/
    button->setFlat(true);                                              /*去除按钮边框*/
    button->setIcon(myicon);                                            /*设置按钮图标*/
    button->setIconSize(QSize(50, 50));                                 /*设置按钮尺寸大小*/
}

/*移动进度条*/
void MainWindow::recv_slider_changed(int position)
{
    pro->write(QString("seek " + QString::number(position) + " 2\n").toUtf8());     /*快进，此处为固定格式*/

    play_flag = 1;                                                                  /*更新播放状态为播放中*/

    SetButtonStyle(ui->Play,"pause.png");                                           /*显示暂停图标*/
}

/*定时获取播放进度*/
void MainWindow::recv_timeout()
{
    pro->write("get_time_pos\n");
}

/*解析播放器输出信息*/
void MainWindow::recv_process_data()
{
    static int time_length = 0;
    int time_pos = 0;
    static int length_min = 0;
    static int length_sec = 0;
    int pos_min = 0;
    int pos_sec = 0;

    QString pro_data = pro->readAll();                          /*获取进程输出的内容，并备份用于字符串的处理*/

     qDebug() << pro_data;

    QString pro_data_length = pro_data;
    QString pro_data_pos = pro_data;

    if(pro_data_length.contains("ANS_LENGTH"))                  /*获取歌曲长度*/
    {
        bool ok;
        /*查找关键词所在位置，并裁剪掉多余的字符串*/
        int ret = pro_data_length.indexOf("ANS_LENGTH");        // 查找 “ANS_LENGTH” 所在的下标，并且返回下标
        pro_data_length.remove(0, ret);                         // 裁剪 “ANS_LENGTH” 之前的内容
        ret = pro_data_length.indexOf("=");                     // 查找字符 “=” 的内容
        pro_data_length.remove(0, ret+1);                       // 裁剪 “ANS_LENGTH=” 的内容
        ret = pro_data_length.indexOf(".");                     // 查找字符 " . " 所在下标，并且返回下标
        pro_data_length.remove(ret, pro_data_length.length());  // 裁剪 “.” 至末尾的全部内容

        time_length = pro_data_length.toInt(&ok, 10);           /*把结果的类型转换成整型数字*/
        ui->Slider_progress->setRange(0, time_length);          /*设置进度条总长度*/

        length_min = time_length / 60;                          /*数值转换成分秒形式*/
        length_sec = time_length % 60;

    }

    else if(pro_data_pos.contains("ANS_TIME_POSITION"))         /*获取进度*/
    {
        bool ok;
        /*查找关键词所在位置，并裁剪掉多余的字符串*/
        int ret = pro_data_pos.indexOf("ANS_TIME_POSITION");
        pro_data_pos.remove(0, ret);
        ret = pro_data_pos.indexOf("=");
        pro_data_pos.remove(0, ret+1);
        ret = pro_data_pos.indexOf(".");
        pro_data_pos.remove(ret, pro_data_pos.length());


        time_pos = pro_data_pos.toInt(&ok, 10);                 /*把结果转成整型*/
        ui->Slider_progress->setValue(time_pos);                /*设置进度条当前数值，跟随播放进度移动*/

        pos_min = time_pos / 60;                                /*数值转换成分秒形式*/
        pos_sec = time_pos % 60;

    }


    // 自动播放下一首
    else if(pro_data_pos.contains("End"))
    {
        qDebug() << r ;
        if(r == 1)
        {
            circulate = 1;
            on_Next_clicked();
        }
        else if (r == 2)
        {
            circulate = 2;
            random_func();
            pro->close();
            Player_Play();
        }
        else if (r == 3)
        {
            circulate = 3;
            pro->close();
            Player_Play();
        }
    }




    /*设置显示的数值规范*/
    ui->label->setText(QString("%1:%2/%3:%4").arg(pos_min, 2, 10, QLatin1Char('0')).arg(pos_sec, 2, 10, QLatin1Char('0')).arg(length_min, 2, 10, QLatin1Char('0')).arg(length_sec, 2, 10, QLatin1Char('0')));
    //ui->label->setText(QString("%1:%2/%3:%4").arg(pos_min).arg(pos_sec).arg(length_min).arg(length_sec));
}


/*双击列表*/
void MainWindow::on_video_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(play_flag == 1 || play_flag == 2)                        /*判断当前播放状态*/
        pro->close();                                           /*关掉当前的播放*/

    /*在播放列表中找到双击的这首视频*/
    for(int j = 0; j < play_list.length(); j++)
    {
        if(play_list[j] == file_path + item->text() + ".mp4")
        {
            pro->start(QString("mplayer -slave -quiet -x 600 -y 360 -zoom  -wid %1  %2").arg(ui->widget->winId()).arg(play_list[j]));

            ui->video_list->setCurrentItem(ui->video_list->item(j));

            ui->Slider_progress->setEnabled(true);
            pro->write("get_time_length\n");
            pro->write("get_time_pos\n");

            play_flag = 1;
            get_pos_timer->start(500);                          /*0.5秒更新一次进度条位置*/

            SetButtonStyle(ui->Play,"pause.png");               /*显示暂停图标*/

            i = j;
        }
    }
}







/*改写进度条类的构造函数*/
MySlider::MySlider(QWidget *parent):QSlider(parent){
    m_value=0;
    m_mousePress = false;
    m_isMoving=false;
}

/*改写进度条类的析构函数*/
MySlider::~MySlider()
{
}

/*改写鼠标点下事件*/
void MySlider::mousePressEvent(QMouseEvent *event)
{
    get_pos_timer->stop();
    //    this.x:控件原点到界面边缘的x轴距离；
    //    globalPos.x：鼠标点击位置到屏幕边缘的x轴距离；
    //    pos.x：鼠标点击位置到本控件边缘的距离；
    //    this.width:本控件的宽度;
    //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
    QSlider::mousePressEvent(event);
    m_isMoving = false;
    m_mousePress = true;
    //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();
    //value + 0.5 四舍五入
    if(value>maximum()){
        value=maximum();
    }
    if(value<minimum()){
        value=minimum();
    }
    m_value=value+0.5;
    setValue(m_value);
    //emit sliderMoved( m_value );
    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));

    QCoreApplication::sendEvent(parentWidget(), &evEvent);
}

/*改写鼠标移动事件*/
void MySlider::mouseMoveEvent(QMouseEvent *event)
{
    QSlider::mouseMoveEvent(event);
    //qDebug()<<"mouseMoveEvent";
    double pos = event->pos().x() / (double)width();
    double value = pos * (maximum() - minimum()) + minimum();
    if(value>maximum()){
       value=maximum();
    }
    if(value<minimum()){
       value=minimum();
    }
    //value + 0.5 四舍五入
    if(m_mousePress){
       m_value=value + 0.5;
       m_isMoving=true;
       //emit sliderMoved(m_value);
    }
    setValue(value + 0.5);
    //向父窗口发送自定义事件event type，这样就可以在父窗口中捕获这个事件进行处理
    QEvent evEvent(static_cast<QEvent::Type>(QEvent::User + 1));

}

/*改写鼠标点下后松开事件*/
void MySlider::mouseReleaseEvent(QMouseEvent *event)
{
    QSlider::mouseReleaseEvent(event);
    m_mousePress = false;
    m_isMoving=false;
    emit sliderReleasedAt(m_value);//抛出有用信号
    get_pos_timer->start(500);
}

