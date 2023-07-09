#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myslider.h"
#include <QFileDialog>
#include <QDebug>
#include <cstdlib>
/****************************************************************************
    本程序为车载影音，功能组成为：播放视频、切换视频
    知识点包含：mplayer播放器、进程管理、定时器、字符串处理、信号与槽
    可拓展功能：音量加减、快进快退按钮
****************************************************************************/

QTimer *get_pos_timer;                                                                  /*创建一个定时器，用于定时获取播放进度*/

/*构造函数：程序运行时自动调用，一般用于初始化*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);                                                                  /*界面初始化*/

    i = 0;                                                                              /*视频序号默认为0开始*/
    play_flag = 0;                                                                      /*当前播放状态默认为：无视频播放*/
    pro = new QProcess(this);                                                           /*为播放器进程请求资源*/
    get_pos_timer = new QTimer(this);                                                   /*为定时器请求资源*/
    PlayerInit();                                                                       /*播放器初始配置*/

    SetButtonStyle(ui->Play, "play.png");                                               /*显示各按钮图标*/
    SetButtonStyle(ui->Next, "next.png");
    SetButtonStyle(ui->Last, "last.png");
    SetButtonStyle(ui->get_file_path, "get_filepath.png");
    SetButtonStyle(ui->fast_forward, "fast_forward.png");
    SetButtonStyle(ui->rewind_quickiy, "rewind_quickiy.png");
    SetButtonStyle(ui->volume_add, "volume_add.png");
    SetButtonStyle(ui->volume_low, "volume_low.png");
    SetButtonStyle(ui->close_video, "close.png");
    SetButtonStyle(ui->pushButton_5,"顺序播放.png");



    AddVideos("James_Blunt-You're_Beautiful(Live).mp4");                                /*添加视频文件*/



    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(savestabuyEditinfo()));
    ui->lineEdit->setPlaceholderText("请输入需要搜索的视频");

}




/*析构函数：程序结束后自动调用，一般用于释放内存*/
MainWindow::~MainWindow()
{
    delete ui;
}



QString cmd_str;
/*播放视频*/
void MainWindow::Player_Play()
{
    // 1.判断播放列表是否为空，如果播放列表为空则不进行操作，不为空的话才会执行播放功能
    if(play_list.length() == 0)
            return;

    // 2.开始播放视频
    cmd_str = QString("mplayer -slave -quiet -wid %1 %2").arg(ui->widget->winId()).arg(play_list[i]);
    pro->start(cmd_str);

    // 3.1让进度条启用；3.2让播放列表中的视频名高亮
    ui->Slider_progress->setEnabled(true);
    ui->video_list->setCurrentItem(ui->video_list->item(i));   //item代表列表中的选项

    // 4.1获视频总时间；2.获取视频播放进度
    pro->write("get_time_length\n");
    pro->write("get_time_pos\n");

    // 5.每隔0.5秒发送一次信号，用于获取当前播放进度
    get_pos_timer->start(500);

    // 6.设置暂停图标
    SetButtonStyle(ui->Play,"pause.png");

    // 7.更新播放状态
    play_flag = 1;
}


/*暂停播放*/
void MainWindow::Player_Pause()
{
    // 1.暂停播放时，先停止定时器
    get_pos_timer->stop();

    // 2.暂停视频
    pro->write("pause\n");

    // 3.更新播放图标
    SetButtonStyle(ui->Play,"play.png");

    // 4.更新播放状态
    play_flag = 2;
}

/*练习：继续播放*/
void MainWindow::Player_Continue()
{
    // 1.继续播放视频
    pro->write("pause\n");

    // 2.重新开启定时器
    get_pos_timer->start(500);

    // 3.更新播放图标
    SetButtonStyle(ui->Play,"pause.png");

    // 4.更新播放状态
    play_flag = 1;
}

/*练习：播放或暂停视频*/
void MainWindow::on_Play_clicked()
{
    // 1.如果当前没有视频播放，点击之后开始播放
    if(play_flag == 0)
    {  
      Player_Play();
    }
    // 2.如果当前正在播放视频，点击之后暂停
    else if(play_flag == 1)
    {
      Player_Pause();
    }
    // 3.如果当前处于暂停状态，点击后继续播放
    else if(play_flag == 2)
    {
      Player_Continue();
    }
}

/*上一首*/
void MainWindow::on_Last_clicked()
{
    if(play_flag == 1 || play_flag == 2)
        pro->close();
    else
        return;

    if(i <= 0)
        i = play_list.length() - 1;
    else
        i--;

    Player_Play();



}

/*练习：下一首*/
void MainWindow::on_Next_clicked()
{
    if(play_flag == 1 || play_flag == 2)
        pro->close();
    else {
        return;
    }

    if(i >= play_list.length() - 1)
        i = 0;
    else
        i++;
    Player_Play();
}


// 获取文件列表
QStringList files;
QStringList videos_list;
int jj = 1;
void MainWindow::on_get_file_path_clicked()
{
    files = QFileDialog::getOpenFileNames(
                                  this,
                                  "获取视频",
                                  "/home/work/Vehicle-Mounted-System/videos",
                                  "视频 (*)");

    if(jj == 1)
    {
        ui->video_list->clear();
        play_list.clear();
        ++jj;
    }

    qDebug() << jj;
    for(int i = 0; i < files.length(); i++)
    {
       if( !videos_list.contains(files[i]) )
       {
           videos_list.append(files[i]);
           QString str = QString(videos_list[i]).remove("/home/work/Vehicle-Mounted-System/videos/");
           AddVideos(str);
       }
    }



}

// 音量加
void MainWindow::on_volume_add_clicked()
{
    pro->write("volume 1000\n");
}

// 音量减
void MainWindow::on_volume_low_clicked()
{
    pro->write("volume -1000\n");
}

// 快进
void MainWindow::on_fast_forward_clicked()
{
     pro->write("seek 10\n");
}

// 快退
void MainWindow::on_rewind_quickiy_clicked()
{
    pro->write("seek -10\n");
}




















void MainWindow::on_pushButton_clicked()
{
    if(play_flag == 1 || play_flag == 2)
    {
        Player_Pause();
        pro->close();
    }
    ui->video_list->clear();
    videos_list.clear();
    play_list.clear();
    play_flag = 0;
    ui->widget->repaint();
    ui->label->setText(QString("00:00/00:00"));
    ui->Slider_progress->setEnabled(false);
}

void MainWindow::random_func()
{
    random_num(2);
    if(videos_list.length() > 2)
        i = rand() % videos_list.length();
    qDebug() << i ;
}

void MainWindow::on_close_video_clicked()
{
    if(play_flag == 1 || play_flag == 2){
        pro->close();
        ui->widget->repaint();
    }
    else
        return;
}

void MainWindow::on_pushButton_5_clicked()
{
    if(circulate == 1)
    {
      random_num(2);
      SetButtonStyle(ui->pushButton_5,"随机播放.png");
      circulate = 2;
    }
    else if(circulate == 2)
    {
      random_num(3);
      SetButtonStyle(ui->pushButton_5,"单曲循环.png");
      circulate = 3;
    }
    else if(circulate == 3)
    {
      random_num(1);
      SetButtonStyle(ui->pushButton_5,"顺序播放.png");
      circulate = 1;
    }
}

QString str_search;
void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->video_list->clear();
    for(int i = 0; i < files.length(); i++)
    {
          QString str = QString(videos_list[i]).remove("/home/work/Vehicle-Mounted-System/videos/");
          AddVideos(str);
    }
}

void MainWindow::savestabuyEditinfo()
{
    str_search = ui->lineEdit->text();
    qDebug() << str_search;
    int clear_i = 1;
    for(int i = 0; i < videos_list.length(); i++)
    {
        int ret = videos_list[i].indexOf(str_search);
        qDebug() << ret;    
        if(clear_i == 1)
        {
            ui->video_list->clear();
            clear_i = 2;
        }
        if(ret != -1)
        {
            QString str = QString(videos_list[i]).remove("/home/work/Vehicle-Mounted-System/videos/");
            AddVideos(str);
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{

}
