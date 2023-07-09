#ifndef _MYLICENSEPLATE_H_
#define _MYLICENSEPLATE_H_

#include <stdio.h>
#include <unistd.h>

//json头文件
#include "json/json.h"
//curl头文件
#include "curl/curl.h"


typedef enum {
	SINGLE_PLATE = 0,		//单张车牌
	MULTI_PLATE				//多张车牌
} PLATE_E; 




//void myLicensePlate(const char *pathName ,PLATE_E plate);
Json::Value myLicensePlate(const char *pathName ,PLATE_E plate);

#endif