#include "mylicenseplate.h"

/********************************************** 以下头文件不要移动 **********************************************/
//API接口头文件
#include "api/ocr.h"
#include "api/base/base.h"
#include "api/base/base64.h"
#include "api/base/http.h"
#include "api/base/utils.h"

/********************************************** 以下 ID 和 KEY 需要根据自己创建的应用进行修改 **********************************************/
// #define APP_ID  	"17840970"
// #define API_KEY 	"sFrHiHw4A4ZnNKPS436dAlCU" 			
// #define SECRET_KEY 	"Lvpvsf1VfXdrDoR3ovgCOoj6Is8G944Q" 
// #define APP_ID  	"24794480"
// #define API_KEY 	"8HWwoqmNAUa3FaIGOp99xDGR" 			
// #define SECRET_KEY 	"f58Sft0NRbPkoGGZNEnrvF6PRHdWhRHK" 	
#define APP_ID  	"32502558"
#define API_KEY 	"uDzDiInp5MmGwVTtORzisx0M" 			
#define SECRET_KEY 	"7jsRzrghgs1ez5FAkTI9DzloCrCHaQMQ" 		



/*
 * 功能：实现车牌识别
 * 参数：
 * 		pathName	---》图片路径名
 * 		plate 		---》实现多张车牌(MULTI_PLATE)还是单张车牌(SINGLE_PLATE)
 * 返回值：返回接收接收到的 JSON 对象
 */
Json::Value myLicensePlate(const char *pathName ,PLATE_E plate)
{
	// 设置APPID/AK/SK
	std::string app_id = APP_ID;				//"你的 App ID";
	std::string api_key = API_KEY;				//"你的 Api key";
	std::string secret_key = SECRET_KEY;		//"你的 Secret Key";
	aip::Ocr client(app_id, api_key, secret_key);
	//client.setDebug(true);

	Json::Value result;							//定义JSON类返回值变量
	std::string image;							//定义string类变量
	aip::get_file_content(pathName, &image);	//获取图片文件信息存入image (成功返回0，失败返回-1 ==> api/base/utils.h)
	//printf("image.size():%lu\n" ,image.size());  //55835

	// 如果有可选参数
	std::map<std::string, std::string> options;

	if (plate == MULTI_PLATE)
	{
		options["multi_detect"] = "true";   //是否检测多张车牌，默认为false，当置为true的时候可以对一张图片内的多张车牌进行识别
	}else{
		options["multi_detect"] = "false";
	}


	// 带参数调用车牌识别(识别车牌信息 api/ocr.h)
	result = client.license_plate(image, options);

	//此处是将得到的 json 格式的 result 转成字符串（调试使用）
	//Json::FastWriter writer;
	//printf("%s\n", writer.write(result).c_str() );

	return result;
}

