#include <iostream>
#include <fstream>
#include <time.h>
#include "mylicenseplate.h"
#include "./tts/TTS.h"
#include "WayFinding.h"
using namespace std;

// 计费时间
//定义两个时间结构体存放入场和出场的时间
struct tm in_time;
struct tm out_time;

bool car_flag = 0;//假设flag = 0，0表示已出场，1表示已入场

//停车位选择，横坐标与纵坐标☆☆☆☆☆☆
int track_space_x;
int track_space_y;

// 1.创建 JSON 对象用于接收车牌识别的结果
Json::Value result;
// 封装车牌识别函数接口
/*
**参数：
	pic:图片的路径
**返回值：
    车牌识别之后返回车牌识别结果
*/
// void LicensePlateRecognition(string pic)
string LicensePlateRecognition(string pic)
{
    // 1.创建 JSON 对象用于接收车牌识别的结果
    // Json::Value result; // 转为全局变量
    // 2.调用函数接口实现车牌识别，并且使用 result 接收 Json 返回值
    //string变量.c_str()：把 string 类型的数据转换成 char * 类型的数据
    result = myLicensePlate(pic.c_str(),SINGLE_PLATE);
    // 输出数据
    // cout << result << endl;
    // cout << result["words_result"]["color"] ;
    // cout << result["words_result"]["number"] << endl;
    string str_result = result["words_result"]["color"].asString() + " " + 
                        result["words_result"]["number"].asString();

    // 返回车牌识别的结果
    return str_result;
}

// 6.封装一个函数接口，用户获取当前时间，并把时间的字符串返回
string GetTime()
{
    // 6.1 创建一个time系列相关的结构体
    time_t curtime;
    // 6.2 使用time函数获取当前系统时间，并把内容保存到时间结构体中
    time(&curtime);
    // 6.3 使用ctime函数把时间转换成日常时间的格式 
    // cout << ctime(&curtime) << endl; 
    //6.4.准备一个时间的结构体类型的指针变量 
    struct tm *now; 
 
    //6.5.使用localtime函数把时间结构转换成struct tm的结构体数据类型 
    now = localtime(&curtime); 
 
    //6.6.使用now来表示时间当中的年(1900年之后)、月(0~11)、日、时、分、秒、周几 
    //xxxx年x月x日 时：分：秒 
    //now->xxx：直取时间结构体中的年、月、日...，单独取出使用 
    int my_year = now->tm_year + 1900; 
    int my_mon = now->tm_mon + 1; 
 
    //5.7.把int类型的日期转成string类型的日期 
    string date_str = to_string(my_year) + "年" + to_string(my_mon) + "月" + to_string(now->tm_mday) + "日"; 
 
    //5.8.把int类型的时间转成string类型的时间 
    string time_str = to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec); 
 
    if(car_flag == 1){
        in_time = *now;
    }else{
        out_time = *now;
    }

    //5.9.把日期和时间的字符串返回到调用的地方 
    return date_str + " " + time_str;//日期 + 空格 + 时间 


}


// 封装车牌号码合成语音
void MakeAudio(string str_audio)
{
    // 6.3 登录科大讯飞接口
    int ret = MSPLogin(NULL, NULL, "appid = 5e07b8d3,work_dir = .");
    if(ret){
        cout << "登录失败" << endl;
    }else{
        cout << "登录成功" << endl;
    }

    // 7. 合成音频
    // 准备音频起始参数
    string session_begin_params = "voice_name = xiaofeng, text_encoding = utf8, sample_rate = 16000, speed = 50, volume = 50, pitch = 50, rdn = 2";
    // 
    ret = text_to_speech(str_audio.c_str(), "car_in.wav", session_begin_params.c_str());

    if(ret){
        cout << "合成失败" << endl;
        return;
    }
    //退出科大讯飞服务器成功
	ret = MSPLogout();
    if(ret == 0)
    {
    	cout << "退出登录成功!" << endl;;
    }

    // 播报语音
    // 使用 system 函数接口实现在程序中调用 shell 命令
    system("mplayer car_in.wav");
}

int File_exist(string pic_path)
{
    // 1.创建流对象
    ifstream ifs;
    // 2.打开文件
    ifs.open(pic_path,ios::in);
    // 3.判断文件是否存在
    if(ifs){
        cout << "图片存在" << endl;
        return 1;
    }else{
        cout << "图片不存在" << endl;
        return -1;
    }
    // 4.关闭文件
    ifs.close();
    return 0;
}

void Sava_file(string str_result,string cat_sava_file,string car_access,string car_time)
{
    // 5.1 创建流对象
    ofstream ofs;
    // 5.2 打开文件
    ofs.open(cat_sava_file,ios::app);
    // 5.3 判断文件是否打开成功
    if(!ofs){
        cout << "文件打开失败" << endl;
        return;
    }

    // 获取当前时间
    string get_time = GetTime();
    cout << get_time << endl;

    // 5.4 数据写入文件
    ofs << car_access << str_result << "  " <<car_time << get_time <<endl;
    // 5.5 关闭文件
    ofs.close();
}

// 封装入场识别函数接口
void Car_in()
{
    // 入场等于 1
    car_flag = 1;

    // 输入图片路径
    cout << "请输入需要识别的车牌图片：" ;
    string pic_path;
    cin >> pic_path;

    // 验证图片文件是否存在
    int ret = File_exist(pic_path);
    if(ret == -1){
        return;
    }else if (ret == 1)
    {
        // 调用车牌识别函数
        string str_result = LicensePlateRecognition(pic_path);
        cout << str_result << endl;

        // 5.把识别的结果保存在文件中
        Sava_file(str_result,"./car_in.txt","入场车辆: ","入场时间: ");

        // 6.车牌号码合成语音前准备
        string str_audio = "欢迎" + result["words_result"]["number"].asString() + "入场" ;
        // 6.1 调用车牌号码合成函数
        MakeAudio(str_audio);

        // 显示停车位
        Show_map();

        //输入停车位☆☆☆☆☆☆
        cout << "请选择停车位:" ;
        cin >> track_space_y >> track_space_x;
        // if(track_space_y >9 && track_space_x >9)
        // {
        //     cout << "所停放的车位不在停车场范围内，" <<endl;
        //     return;
        // }
        // void parking_lot();
        
        // 7.调用寻路代码
        WayFinding(0,0, track_space_y,track_space_x);

        cout << endl << "停车完毕~" << endl;


    }
      
    return;

}


// 封装离场识别函数接口
void Car_out()
{
    // 出场等于 0
    car_flag = 0;

    // 输入图片路径
    cout << "请输入需要识别的车牌图片：" ;
    string pic_path;
    cin >> pic_path;

    // 验证文件是否存在
    int ret = File_exist(pic_path);
    if(ret == -1){
        return;
    }else if (ret == 1)
    {
        // 调用车牌识别函数
        string str_result = LicensePlateRecognition(pic_path);
        cout << str_result << endl;

        // 5.把识别的结果保存在文件中
        Sava_file(str_result,"./car_out.txt","离场车辆: ","离场时间: ");

        // 6.车牌号码合成语音前准备
        string str_audio = "祝" + result["words_result"]["number"].asString() + "一路顺风" ;
        
        
        // 7.调用寻路代码
        WayFinding(track_space_y,track_space_x,  9,0);

        cout << endl << "离场完毕~" << endl;

        // 6.1 调用车牌号码合成函数
        MakeAudio(str_audio);

        // 收费
        out_time.tm_hour - in_time.tm_hour;
        cout << "共计收费：" << (out_time.tm_hour - in_time.tm_hour) * 3600 * 10+
                                (out_time.tm_min - in_time.tm_min) * 60 * 10+
                                (out_time.tm_sec - in_time.tm_sec) * 10 
                            << endl;

    }
    return;
}


int main()
{
    while(1)
    {
        //1、实现输入
        int cmd;
        cout << "请输入操作方式：" << endl;
        cout << "       0: 退出程序" << endl;
        cout << "       1: 入场识别" << endl;
        cout << "       2: 离场识别" << endl;
        cin >> cmd;

        //1.1、判断输入
        if (cmd == 0){
            // 等于 0 程序结束
            return 1;
        }else if (cmd == 1){
            // 等于 1 入场程序
            cout << "入场识别中...." << endl;
            Car_in();
        }else if (cmd == 2 && car_flag == 1 ){
            // 等于 2 出场程序
            cout << "离场识别中...." << endl;
            Car_out();
        }else{
            cout << "输入无效" << endl;
        }
    }
    return 0;
}