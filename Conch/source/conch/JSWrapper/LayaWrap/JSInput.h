#ifndef __LAYA_JSINPUT_H__
#define __LAYA_JSINPUT_H__

#include <stdio.h>
#include "../JSInterface/JSInterface.h"
#include "../../JCScrpitRuntime.h"
#include <mutex>

namespace laya
{
    struct TouchEventInfo
    {
        int nType;
        int nID;
        int x;
        int y;
    };
    class IConchThreadCmdMgr;
	class JSInput:public JsObjBase 
	{
	public:
		static JsObjClassInfo JSCLSINFO;
		static void exportJS();
		static JSInput* getInstance();
		static void Reset();			//临时
	public:
		JSInput();
		~JSInput();
		void onInputCallJSFunction(inputEvent e);
        void onDeviceOrientationCallJSFunction(DeviceOrientationEvent e);
        void onDeviceMotionCallJSFunction(DeviceMotionEvent e);
		bool activeCall( inputEvent param );
        bool activeCall(DeviceOrientationEvent param);
        bool activeCall(DeviceMotionEvent param);
        void captureScreenCallBack(char *p_pBuffer,int p_nLen, int p_nW, int p_nH);
        void onCaptureScreenCallJSFunction(char *p_pBuffer, int p_nLen, int p_nW, int p_nH);
        void swapCurrentTouchEvent();
        void setTouchMode( bool bMode );
        bool getTouchMode();

	public:
		static JSInput*                     m_pInstance;
        IConchThreadCmdMgr*                 m_pCmdPoster;
        bool                                m_bTouchMode;   ///<true为查询模式  false为回调模式
        std::vector<TouchEventInfo>         m_vInputEvents;
        std::vector<TouchEventInfo>         m_vInputEventsJS;
        std::recursive_mutex                m_Lock;
        int                                 m_nTouchFrame;
	};
}


#endif // __LAYA_JSINPUT_H__
