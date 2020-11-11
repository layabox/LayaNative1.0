package laya.game.config;

import java.io.InputStream;
import laya.game.utility.IniReader;
import android.util.Log;

public class config 
{
	private static config ms_config = null;
	public boolean 	m_bIsHandleUpdateAPK=false;
	public String 	m_sApkUpdateUrl=null;
	public String 	m_sUpdateDownloadPath="mnt/sdcard";
	public String 	m_sUpdateAPKFileName="utoupdate.apk";
	public String 	m_sAppVersion=null;
	public boolean 	m_bCheckNetwork=false;
	
	public boolean 	m_bShowLoadingView = false;
	public int		m_nLoadingViewBgColor = 0;
	
	//------------------------------------------------------------------------------
	//市场相关的
	public String 	m_sMarketPlatformClassname=null;			//市场名字
	public int 		m_nMarketWaitScreenBKColor=0xffffffff;		//市场等待的背景颜色
	public int	 	m_nMarketEnterPlatformType=0;				//进入平台的类型
	public String 	m_sMarketExitShowWebUrl=null;				//退出后显示网页
	public String 	m_sMarketServerName="";						//市场的服务器名字
	public int 		m_nMarketPayType=0;							//充值类型 0 为充值  1为购买道具
	public int		m_nMarketLoginType=0;						//登录类型 0 为没有  1为登录 2为授权
	public int 		m_nMarketChargeType=0;						//是元宝还是Q币
	
	
	public static config GetInstance()
	{
		if( ms_config == null )
		{
			ms_config = new config();
		}
		return ms_config;
	}
	public config()
	{
		
	}
	public boolean init( InputStream pInputStream )
	{
		try
		{
			IniReader pIniReader = new IniReader( pInputStream );
			m_bIsHandleUpdateAPK=( Integer.parseInt( pIniReader.getValue(null, "IsHandleUpdateAPK") ) != 0 );
			m_sApkUpdateUrl=pIniReader.getValue(null, "ApkUpdateUrl");
			m_sUpdateDownloadPath=pIniReader.getValue(null, "UpdateDownloadPath");
			m_sUpdateAPKFileName=pIniReader.getValue(null, "UpdateAPKFileName");
			m_sAppVersion=pIniReader.getValue(null, "AppVersion");
			m_bCheckNetwork=( Integer.parseInt( pIniReader.getValue(null, "CheckNetwork") ) != 0 );
			m_bShowLoadingView = Integer.parseInt( pIniReader.getValue(null, "loadingViewShow") ) !=  0;
			String sColor1 = pIniReader.getValue(null, "loadingViewBgColor");
			boolean bAddAlpha1=sColor1.length()<=6;
			int nColor1 = (int)(Long.parseLong(sColor1, 16));
			if( bAddAlpha1 )
			{
				nColor1|=0xff000000;
			}
			m_nLoadingViewBgColor=nColor1;

			m_sMarketPlatformClassname=pIniReader.getValue(null, "MarketPlatformClassname");
			String sColor = pIniReader.getValue(null, "MarketWaitScreenBKColor");
			boolean bAddAlpha=sColor.length()<=6;
			int nColor = (int)(Long.parseLong(sColor, 16));
			if( bAddAlpha )
			{
				nColor|=0xff000000;
			}
			m_nMarketWaitScreenBKColor=nColor;
			m_nMarketEnterPlatformType=Integer.parseInt( pIniReader.getValue(null, "MarketEnterPlatformType") );
			m_sMarketExitShowWebUrl=pIniReader.getValue(null, "MarketExitShowWebUrl");
			m_sMarketServerName=pIniReader.getValue(null, "MarketServerName");
			m_nMarketPayType=Integer.parseInt( pIniReader.getValue(null, "MarketPayType") );
			m_nMarketChargeType = Integer.parseInt(pIniReader.getValue(null, "MarketChargeType"));
			m_nMarketLoginType= Integer.parseInt( pIniReader.getValue(null, "MarketLoginType"));
			

			Log.i("0", "====config.init m_bIsHandleUpdateAPK=" + m_bIsHandleUpdateAPK );
			Log.i("0", "====config.init m_sApkUpdateUrl=" + m_sApkUpdateUrl );
			Log.i("0", "====config.init m_sUpdateDownloadPath=" + m_sUpdateDownloadPath );
			Log.i("0", "====config.init m_sUpdateAPKFileName=" + m_sUpdateAPKFileName );
			Log.i("0", "====config.init m_sAppVersion=" + m_sAppVersion );
			Log.i("0", "====config.init m_bCheckNetwork=" + m_bCheckNetwork );
			Log.i("0", "====config.init m_sPlatformClassname=" + m_sMarketPlatformClassname );
			Log.i("0", "====config.init m_nWaitScreenBKColor=" + m_nMarketWaitScreenBKColor );
			Log.i("0", "====config.init m_nEnterPlatformType=" + m_nMarketEnterPlatformType );
			Log.i("0", "====config.init m_sExitShowWebUrl=" + m_sMarketExitShowWebUrl );
			Log.i("0", "====config.init m_sMarketServerName=" + m_sMarketServerName );
			Log.i("0", "====config.init m_nMarketPayType=" + m_nMarketPayType );
			Log.i("0", "====config.init m_bMarketHasLogin=" + m_nMarketLoginType );
			Log.i("0", "====config.init m_bShowLoadingView=" + m_bShowLoadingView );
			Log.i("0", "====config.init m_nLoadingViewBgColor=" + m_nLoadingViewBgColor );
		}
		catch(Exception e)
		{
			return false;
		}
		return true;
	}
}
