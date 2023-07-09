#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QTimer>
#include <QListWidgetItem>
#include <QSlider>
#include <QMouseEvent>
#include <QIcon>
#include <QPainter>
#include <QPalette>
#include <QtGui>
#include <QStandardItemModel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*重写进度条类*/
class MySlider : public QSlider
{
    Q_OBJECT

public:
    MySlider(QWidget *parent = 0);
    ~MySlider();

Q_SIGNALS:
    void sliderReleasedAt(int);

public Q_SLOTS:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool m_isMoving;
    int m_value;
    bool m_mousePress;

};

/*主窗体类*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

/*封装函数的声明*/
    void PlayerInit();                                      /*设置按钮*/
    void AddVideos(QString );                               /*添加视频*/
    void SetButtonStyle(QPushButton *button,QString pic);   /*设置按钮图标*/
    void Player_Play();
    void Player_Pause();
    void Player_Continue();

    void random_num(int i);
    void random_func();



/*私有槽函数声明*/
private slots:
    void on_Play_clicked();
    void on_Next_clicked();
    void on_Last_clicked();
    void recv_timeout();
    void recv_process_data();
    void on_video_list_itemDoubleClicked(QListWidgetItem *item);
    void recv_slider_changed(int );

    void on_get_file_path_clicked();

    void on_volume_add_clicked();

    void on_volume_low_clicked();

    void on_fast_forward_clicked();

    void on_rewind_quickiy_clicked();


    void on_pushButton_clicked();


    void on_close_video_clicked();



    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void savestabuyEditinfo();


    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;             /*新建界面对象*/

    QProcess *pro ;                 /*新建一个播放器进程*/
    QStringList play_list;          /*新建一个播放列表*/
    int i ;                         /*播放列表中的标号*/
    int play_flag ;                 /*记录播放状态：0表示当前没有视频播放，1表示当前正在播放，2表示当前正在暂停*/
    int circulate = 1;
    QString file_path;              /*视频文件所在路径*/
    QString icon_path;              /*图标文件所在路径*/
};
#endif // MAINWINDOW_H
