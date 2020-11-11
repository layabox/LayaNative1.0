
#ifndef _LAYA_CONFIG_H__
#define _LAYA_CONFIG_H__

#include <string>

namespace laya
{
    enum FRAME_TYPE
    {
        FT_SLOW = 0,
        FT_FAST,
        FT_MOUSE,
    };
	class JCSystemConfig 
	{
	public:
		JCSystemConfig();
		void reset();
	public:
		int m_nMaxTextureMemSize;
		int m_nAtlasWidth;
		int m_nAtlasHeight;
		int m_nAtlasGridSize;
		int m_nAtlasNum;
		std::string m_strStartURL;
		bool	m_bPerfStat;			//进行效率统计
        FRAME_TYPE  m_nFrameType;
        double m_nFrameThreshold;
        int     m_nSleepTime;
		int		m_nPerf_UpdateNum;
		std::string	m_strPerfOut;
		std::string m_jsonparamExt;//额外参数
        int m_nMemoryCanvasWidth;   //内存canvas的宽
        int m_nMemoryCanvasHeight;  //内存canvas的高
		static bool s_bIsPlug;      //因为初始化太早了，做成static
        static bool s_bLocalizable;//设置是否是本地包
        bool m_bShowInternalPerBar = false;//是否显示js ondraw部分和gl的柱状性能图。
        bool m_bUseChoreographer = false; //在android下启用choreographer
        bool m_bUseAndroidSystemFont = false;
	};
	extern JCSystemConfig g_kSystemConfig;
}

#endif