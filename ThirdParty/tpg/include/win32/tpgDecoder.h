/*********************************************************************************
*版权：			腾讯科技（深圳）有限公司
*文件名:			tpgDecoder.h
*作者:			王诗涛
*版本:			v1.0
*日期:			2017.11.08
*功能描述:		TPG解码
**********************************************************************************/

#ifndef __TPGDECODER_H__
#define __TPGDECODER_H__

#ifndef WIN32
#define DLLEX __attribute__ ((visibility ("default")))

#else
#define DLLEX __declspec(dllexport)
#endif

#define MAX_LAYER_NUM	3
#define MAX_WIDTH	16383
#define MAX_HEIGHT  16383

#ifndef TPG_RAW_DATA_FORMAT
#define TPG_RAW_DATA_FORMAT

#define IMAGE_SUPPLEMENT_DATA 0x000001BA
#define IMAGE_USER_DATA 0x000001BD


typedef enum tagRawDataFormat
{
	FORMAT_YUV = 0,
	FORMAT_RGB = 1,
	FORMAT_BGR = 2,
	FORMAT_RGBA = 3,
	FORMAT_BGRA = 4,
	FORMAT_NV21 = 5,
	FORMAT_RGBA_BLEND_ALPHA = 6, //RGB*alpha
}enRawDataFormat;

typedef enum TPGStatusCode {
    TPG_STATUS_OK = 0,
    TPG_STATUS_OUT_OF_MEMORY,
    TPG_STATUS_INVALID_PARAM,
    TPG_STATUS_BITSTREAM_ERROR,
    TPG_STATUS_UNSUPPORTED_FEATURE,
    TPG_STATUS_SUSPENDED,
    TPG_STATUS_USER_ABORT,
    TPG_STATUS_NOT_ENOUGH_DATA,
    TPG_STATUS_INIT_ERROR
} TPGStatusCode;


typedef	enum enumImageMode
{
    emMode_Normal = 0,				//静态图片格式，输出为RGB数据；
    emMode_EncodeAlpha = 1,			//带Alpha通道静态图片格式，输出为RGBA数据；
    emMode_BlendAlpha = 2,			//有Alpha通道，但是Alpha通道直接与RGB数据合成了，最终输出RGB数据；
    emMode_Animation = 3,			//动态图片格式（不带Alpha）
	emMode_AnimationWithAlpha = 4,	//动态图片格式（带Alpha）
}enumImageMode;

#endif

typedef struct _TPGFeatures
{
	int header_size;			//TPG文件头长度；
    int width;					//TPG图片宽度
    int height;					//TPG图片高度
    enumImageMode image_mode;	//TPG文件类型
    int version;				//TPG文件格式版本
	int frame_count;			//TPG文件包含的图像帧数，静态图片为1
    int reserved[10];			//保留位
}TPGFeatures;

typedef struct _TPGOutFrame
{
	unsigned char* pOutBuf;		//输出RGB数据缓冲区地址 
	int bufsize;				// 缓冲区大小
    int dstWidth;				// 输出图片目的宽度
    int dstHeight;				// 输出图片目的高度
    enRawDataFormat fmt;		// 输出图片数据类型
    int delayTime;				// 当前图片显示的延迟时间（只对动态图片有效）
}TPGOutFrame;

typedef struct _TPGVersion {
	int num;
	char str[20];
}TPGVersion;

#ifdef __cplusplus
extern "C" {
#endif

	//==================================================================
	//函数名：	TPGDecCreate
	//功能：		创建TPG解码器
	//输入参数：	pData	需解码的TPG码流
	//			len[in]	TPG码流长度
	//返回值：	类型（void*)
	//          返回TPG解码器对象句柄，创建不成功返回NULL。
	//修改记录：
	//==================================================================
    DLLEX void* TPGDecCreate(const unsigned char* pData, int len);


	//==================================================================
	//函数名：	TPGDecDestroy
	//功能：		销毁TPG解码器
	//输入参数：
	//			h	TPG解码器句柄
	//返回值：	类型（void)
	//修改记录：
	//==================================================================
	DLLEX void TPGDecDestroy(void* h);

	//==================================================================
	//函数名：	TPGDecodeImage
	//功能：		解码一帧图像
	//输入参数：
	//			hDec	TPG解码器句柄
	//			pData	需解码的TPG码流
	//			len	    TPG码流长度
	//			index	解码帧索引（静态图片该值设为0，对于动图，表示解码第index帧图像）
	//输出参数：
	//			pDecFrame	解码输出帧，包括宽高等信息
	//返回值：	类型（TPGStatusCode)
	//          解码成功返回TPG_STATUS_OK，否则返回对应错误码。
	//修改记录：
	//==================================================================
	DLLEX TPGStatusCode TPGDecodeImage(void* hDec, const unsigned char* pData, int len, int index, TPGOutFrame *pDecFrame);

	//==================================================================
	//函数名：	TPGParseHeader
	//功能：		解析码流头文件
	//输入参数：
	//			pData	需解码的TPG码流
	//			len	    TPG码流长度
	//输出参数：
	//			pFeatures	输出TPG文件基本特征信息
	//返回值：	类型（TPGStatusCode)
	//          当前文件为TPG文件返回TPG_STATUS_OK，否则返回对应错误码。
	//修改记录：
	//==================================================================
	DLLEX TPGStatusCode TPGParseHeader(const unsigned char* pData, int len, TPGFeatures* pFeatures);

	//==================================================================
	//函数名：	TPGGetDelayTime
	//功能：		获取当前帧图像的显示延迟时间（该接口只有动态图片有效）
	//输入参数：
	//			hDec	TPG解码器句柄
	//			pData	需解码的TPG码流
	//			len	    TPG码流长度
	//			index	解码帧索引（静态图片该值设为0，对于动图，表示解码第index帧图像）
	//输出参数：
	//			pDelayTime	对应帧的显示延迟时间
	//返回值：	类型（TPGStatusCode)
	//          获取返回TPG_STATUS_OK，否则返回对应错误码。
	//修改记录：
	//==================================================================
	DLLEX TPGStatusCode TPGGetDelayTime(void* hDec, const unsigned char* pData, int len, int index, int* pDelayTime);

	//==================================================================
	//函数名：	TPGCanDecode
	//功能：		检测当前码流数据是否足够解码
	//输入参数：
	//			hDec	TPG解码器句柄
	//			pData	需解码的TPG码流
	//			len	    TPG码流长度
	//			index	解码帧索引（静态图片该值设为0，对于动图，表示解码第index帧图像）
	//返回值：	类型（TPGStatusCode)
	//			第index帧能解码返回TPG_STATUS_OK，否则返回对应错误码。
	//修改记录：
	//==================================================================
	DLLEX TPGStatusCode TPGCanDecode(void* hDec,  const unsigned char* pData, int len, int index);

	//==================================================================
	//函数名：	TPGGetAdditionalInfo
	//功能：		获取TPG码流中的用户辅助信息(比如EXIF等)
	//输入参数：
	//			hDec		TPG解码器句柄
	//			pInData		TPG码流
	//			nInDatalen	TPG码流长度
	//			nIdentity	辅助信息标识
	//输出参数：
	//			pOutData	辅助信息首地址（如果没有返回NULL），该内存不需要应用层分配
	//			pOutDataLen	对应帧的显示延迟时间（辅助信息长度）
	//返回值：	类型（TPGStatusCode)
	//          获取到对应辅助信息返回TPG_STATUS_OK，否则返回对应错误码。
	//修改记录：
	//==================================================================
	DLLEX TPGStatusCode TPGGetAdditionalInfo(void* hDec, const unsigned char* pInData, int nInDatalen, int nIdentity, const unsigned char** pOutData, int* pOutDataLen);

	//==================================================================
	//函数名：	TPGDecGetAverageQP
	//功能：		获取TPG解码QP
	//输入参数：	无
	//返回值：	类型（float)
	//			返回对应QP值。
	//修改记录：
	//==================================================================
	DLLEX float TPGDecGetAverageQP(void* hDec);

	//==================================================================
	//函数名：	TPGDecGetVersion
	//功能：		获取TPG解码器版本号
	//输入参数：	无
	//返回值：	类型（int，string)
	//			返回对应版本号。
	//修改记录：
	//==================================================================
	DLLEX TPGVersion TPGDecGetVersion();
#ifdef __cplusplus
};
#endif


#endif	// __TPGDEC_H__
