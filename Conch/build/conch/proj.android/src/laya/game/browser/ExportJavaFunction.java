package laya.game.browser;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import laya.game.Netword.NetworkReceiver;
import laya.game.Notifycation.LayaNotifyManager;
import laya.game.PlatformInterface.LayaPlatformGlue;
import laya.game.conch.LayaConch3;
import laya.game.device.DevID;
import laya.game.floatmenu.MyWindowManager;
import laya.game.plugin.webview.LayaWebViewManager;
import laya.game.utility.LayaAudioMusic;
import laya.game.utility.ProcessInfo;
import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetricsInt;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.os.Handler;
import android.os.Vibrator;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;


public class ExportJavaFunction 
{
	private static	ExportJavaFunction		ms_pExportJavaFunction = null;
	private 		Paint 					m_Paint = new Paint();
	//private			LayaExternalWebView		m_pLayaExternalWebView = null;
	private			String					m_sExternalUrl = "";
	//private			LayaEditBox				m_pEditBox = null;
	private 		String					m_sAlertMsg = "";
	private			int						m_nAlertCallbackType = 0;
	private			boolean					m_bShowLoading = false;
	private			boolean					m_bScreenWakeLock = false;
	private			boolean					m_bShowWating = false;
	public 			Handler					m_Handler = new Handler();
	public 			LayaConch3				m_pEngine= null;
   static public 	String					m_sHref="";
   static public	int 					m_nState=0;		//是否是第一次创建。0 还没创建 ， 1 已经创建， 2被删除了
	
	//------------------------------------------------------------------------------
	public static ExportJavaFunction GetInstance()
	{
		if(ms_pExportJavaFunction==null && m_nState!=2){//如果已经被删除了，则不允许再创建。
			ms_pExportJavaFunction = new ExportJavaFunction();
			m_nState=1;
		}
		
		return ms_pExportJavaFunction;
	}
	public static void DelInstance(){
		Log.e("","DELETE ExportJavaFunction instance!");
		m_nState=2;
		if( ms_pExportJavaFunction !=null ){
			ms_pExportJavaFunction = null;
		}
	}
	
	public void distory()
	{
		m_pEngine=null;
		m_Paint=null;
	}
	//------------------------------------------------------------------------------
	public ExportJavaFunction()
	{
	}
	//------------------------------------------------------------------------------
	public void Init( Context p_pContext )
	{
		/*
		if( m_pLayaExternalWebView == null )
		{
			//kuo remove code TODO 
			//m_pLayaExternalWebView = new LayaExternalWebView( p_pContext );
		}
		if( m_pEditBox == null )
		{
			//kuo remove code
			//m_pEditBox = new LayaEditBox( MainActivity.getLayaApplicationActivity().getApplication() );
			m_pEditBox = new LayaEditBox( p_pContext );

			m_pEditBox.CreateEditBox();
			//Log.d("Laya_plugin","MainActivity.m_pAbsEditLayout="+MainActivity.m_pAbsEditLayout+",m_pEditBox="+m_pEditBox+",m_pEditBox.m_pEditBox="+m_pEditBox.m_pEditBox);
			MainActivity.m_pAbsEditLayout.addView( m_pEditBox.m_pEditBox );
			
		}
		*/
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getTextBuffer", 0, "(Ljava/lang/String;Ljava/lang/String;IIIZII)[I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getTextWidth", 1, "(Ljava/lang/String;Ljava/lang/String;IIZ)I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setExternalLink", 2, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "closeExternalLink", 3, "()V");
        
        //EditBox
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxPosX", 4, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxPosY", 5, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxWidth", 6, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxHeight", 7, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxStyle", 8, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxFocus", 9, "()V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxBlur", 10, "()V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxVisible", 11, "(Z)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxValue", 12, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getEditBoxValue", 13, "()Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxColor", 14, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxFontSize", 15, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxPos", 16, "(II)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxSize", 17, "(II)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxCursorPosition", 18, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxMaxLength", 19, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxPassword", 20, "(Z)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxRegular", 21, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "alert", 22, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "alertCallback", 23, "(Ljava/lang/String;)V");
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "alertExit", 24, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "showLoadingView", 25, "(Z)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getWifiMac", 26, "()Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(ProcessInfo.class.getName().replaceAll("\\.", "/"), "getProcessCpuRate", 27, "()F");
        GL2JNILib.exportStaticMethodToC(ProcessInfo.class.getName().replaceAll("\\.", "/"), "getTotalMem", 28, "()F");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "reloadApp", 29, "()V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setHrefToJava", 30, "(Ljava/lang/String;)V");
        //屏幕一直亮着锁定
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setScreenWakeLock", 31, "(Z)V");
        // 推送
        GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "setRepeatingNotify", 32, "(IJILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "setRepeatWithHourMinute", 33, "(IIIILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "setOnceNotify", 34, "(IJLjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "removeNotify", 35, "(I)V");
        GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "removeAllNotify", 36, "()V");
        GL2JNILib.exportStaticMethodToC(LayaNotifyManager.class.getName().replaceAll("\\.", "/"), "deleteAlarmAndNotify", 37, "(I)V");
        
        //获得屏幕的英寸
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getScreenInch", 38, "()F");
        //设置touchMove的偏移量
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setTouchMoveRange", 39, "(F)V");
        //设置横竖屏
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setScreenOrientation", 40, "(I)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getScreenOrientation", 41, "()I");
        
        //-----------------------------------------------------------------------------------------
        //market相关的

        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getMarketName", 42, "()Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getPayType", 43, "()I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getLoginType", 44, "()I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getChargeType", 45, "()I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "login", 46, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "logout", 47, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "switchUser", 48, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterPlatform", 49, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterBBS", 50, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterFeedback", 51, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "recharge", 52, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "buyProps", 53, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getEnterPlatformType", 54, "()I");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setRechargeInfo", 55, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterAccountMgr", 56, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterShareAndFeed", 57, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "enterInvite", 58, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "authorize", 59, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "refreshToken", 60, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getGameFriends", 61, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "sendToDesktop", 62, "(Ljava/lang/String;)V");
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "sendMessageToPlatform", 63, "(Ljava/lang/String;)V");
        

        //-----------------------------------------------------------------------------------------
        //wating条
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "showWaiting", 64, "(Z)V");
        
        //editBox只能输入数字
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setEditBoxNumberOnly", 65, "(Z)V");
		
		//背景声音
		GL2JNILib.exportStaticMethodToC(LayaAudioMusic.class.getName().replaceAll("\\.", "/"), "playBackgroundMusic", 66, "(Ljava/lang/String;I)V");
		GL2JNILib.exportStaticMethodToC(LayaAudioMusic.class.getName().replaceAll("\\.", "/"), "stopBackgroundMusic", 67, "()V");
		GL2JNILib.exportStaticMethodToC(LayaAudioMusic.class.getName().replaceAll("\\.", "/"), "pauseBackgroundMusic", 68, "()V");
		GL2JNILib.exportStaticMethodToC(LayaAudioMusic.class.getName().replaceAll("\\.", "/"), "resumeBackgroundMusic", 69, "()V");
		GL2JNILib.exportStaticMethodToC(LayaAudioMusic.class.getName().replaceAll("\\.", "/"), "setBackgroundMusicVolume", 70, "(F)V");
		
		//发往主线程的事件
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "postCmdToMain", 71, "(III)V");
		GL2JNILib.exportStaticMethodToC(ProcessInfo.class.getName().replaceAll("\\.", "/"), "getUsedMem", 72, "()F");
		GL2JNILib.exportStaticMethodToC(ProcessInfo.class.getName().replaceAll("\\.", "/"), "getAvalidMem", 73, "()F");
		
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "RuntimeCallJava", 74, "(Ljava/lang/String;Ljava/lang/String;)V");
		
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "ShowMessage", 75, "(Ljava/lang/String;)V");
		
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getContextedType", 76, "()F");
		
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getAndroidModel", 77, "()Ljava/lang/String;");
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "canSendToDesktop", 78, "(Ljava/lang/String;)I");
		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "openTopicCircle", 79, "(Ljava/lang/String;)V");
		//
 		GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "vibrate", 80, "()V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "GetDeviceInfo", 81, "()Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "JSToJavaBridge", 82, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "RuntimeCallJavaPArray", 83, "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "showAssistantTouch", 84, "(Z)V");
        
        //Market相关
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getUserInfo", 85, "(Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getAvailableLoginType", 86, "(Ljava/lang/String;)V");
        
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "getMarketValue", 87, "(Ljava/lang/String;)Ljava/lang/String;");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setMarketValue", 88, "(Ljava/lang/String;Ljava/lang/String;)V");
        GL2JNILib.exportStaticMethodToC(this.getClass().getName().replaceAll("\\.", "/"), "setMultiAble", 89, "(Z)V");
	}
	//------------------------------------------------------------------------------
	public static int [] getTextBuffer(String p_sChar, String p_sFamily, int p_nSize, int p_nTextColor, int p_nWeight,boolean p_bItalic,int p_nBorderSize,int p_nBorderColor )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			return exjf.getTextPixelBuffer(p_sChar, p_sFamily, p_nSize, p_nTextColor, p_nWeight, p_bItalic,p_nBorderSize,p_nBorderColor );
		return null;
	}
	//------------------------------------------------------------------------------	
    public Bitmap drawTextToBitmap(  String p_sChar, int p_nFontSize, int p_nTexColor, Point pt,int p_nBorderSize,int p_nBorderColor )
    {
    	//目前描边，只支持一个像素的
    	int nBorderSize = 0;
    	if( p_nBorderSize > 0 )
    	{
    		nBorderSize = 1;
    	}
    	int nBmpSize = (p_nFontSize) + nBorderSize * 2;
		m_Paint.setStyle(Style.FILL);
		m_Paint.setTextSize( p_nFontSize );
		m_Paint.setAntiAlias(true);
    	int w = (int) m_Paint.measureText(p_sChar ) +  nBorderSize * 2;
		Bitmap mybitmap = Bitmap.createBitmap( w, nBmpSize, Bitmap.Config.ARGB_8888 );
		Canvas canvas = new Canvas( mybitmap );       
		canvas.drawColor( 0 );
		
		FontMetricsInt fontMetrics = m_Paint.getFontMetricsInt();  // 转载请注明出处：http://blog.csdn.net/hursing  
	    int baseline = 0 + (p_nFontSize - fontMetrics.bottom + fontMetrics.top) / 2 - fontMetrics.top;  
		
		//如果有描边，多花4遍
		if( nBorderSize > 0 )
		{
			m_Paint.setColor(p_nBorderColor);
			canvas.drawText( p_sChar, nBorderSize-nBorderSize,baseline-nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize+nBorderSize,baseline-nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize-nBorderSize,baseline+nBorderSize, m_Paint );
			canvas.drawText( p_sChar, nBorderSize+nBorderSize,baseline+nBorderSize, m_Paint );
		}
		m_Paint.setColor(p_nTexColor);
	    canvas.drawText(p_sChar, nBorderSize, baseline, m_Paint);  
		//canvas.drawText( p_sChar, nBorderSize,p_nFontSize-2, m_Paint );
		pt.x = w;
		pt.y = nBmpSize;
		return mybitmap;
    }
    //------------------------------------------------------------------------------
    public int[] getTextPixelBuffer( String p_sWord, String p_sFamily, int p_nSize, int p_nTextColor,int p_nWeight,boolean p_bItalic,int p_nBorderSize,int p_nBorderColor )
    {
    	int nSize = p_nSize;
    	Point pt=new Point();
 		Bitmap pBitmap = this.drawTextToBitmap(p_sWord, nSize, p_nTextColor, pt,p_nBorderSize,p_nBorderColor );
 		int len = pt.x*pt.y+2;//2 width height
		int[] pDataBuffer = new int[ len ];	
		int nNum = 0;
		
		for( int i = 0; i < pt.y; i++ )
		{
			for( int j = 0; j < pt.x; j++ )
			{
				pDataBuffer[ nNum ] = pBitmap.getPixel(j, i);
				nNum++;
			}
		}
		pDataBuffer[len-2]=pDataBuffer[0];
		pDataBuffer[len-1]=pDataBuffer[1];
		pDataBuffer[0]=pt.x;
		pDataBuffer[1]=pt.y;
		return pDataBuffer;
    }
    //------------------------------------------------------------------------------
    public static int getTextWidth(String p_sChar, String p_sFamily, int p_nSize, int p_nWeight,boolean p_bItalic )
    {
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			return exjf._getTextWidth(p_sChar, p_sFamily, p_nSize, p_nWeight, p_bItalic );
		return 1;
    }
    //------------------------------------------------------------------------------
	public int _getTextWidth(String p_sChar, String p_sFamily, int p_nSize, int p_nWeight,boolean p_bItalic )
	{
		m_Paint.setStyle(Style.FILL);
		m_Paint.setTextSize(p_nSize);
		m_Paint.setAntiAlias(true);
		int w = (int) m_Paint.measureText(p_sChar );
		return w;
	}
	//------------------------------------------------------------------------------
	public static void setExternalLink( String p_sUrl )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.showExternalWeb( p_sUrl );
	}
	//------------------------------------------------------------------------------
	public static void closeExternalLink()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null )
			exjf.closeWebView();
	}
	//------------------------------------------------------------------------------
	private void showExternalWeb( String p_sUrl )
	{
		m_sExternalUrl = p_sUrl;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					m_pEngine.getExternalWebView().m_pReadyData.m_sUrl = m_sExternalUrl;
					m_pEngine.getExternalWebView().m_pReadyData.m_nX = 0;
					m_pEngine.getExternalWebView().m_pReadyData.m_nY = 0;
					WindowManager wm = (WindowManager)(m_pEngine.getGameContext().getSystemService(Context.WINDOW_SERVICE));
					DisplayMetrics dm = new DisplayMetrics();
					wm.getDefaultDisplay().getMetrics(dm);
					//TODO 插件中怎么获得宽高
					m_pEngine.getExternalWebView().m_pReadyData.m_nWidth = dm.widthPixels;//MainActivity.GetScreenWidth();
					m_pEngine.getExternalWebView().m_pReadyData.m_nHeight = dm.heightPixels;//MainActivity.GetScreenHeight();
					m_pEngine.getExternalWebView().showWebView();
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	private void closeWebView()
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					m_pEngine.getExternalWebView().closeWebView();
				}
			}
		);
	}
	public boolean m_bShowAssistantTouch = false;
	public static void showAssistantTouch( boolean p_nShow )
	{
	    ExportJavaFunction pExportJavaFun = ExportJavaFunction.GetInstance();
        if( pExportJavaFun!=null )
        {
            pExportJavaFun._showAssistantTouch( p_nShow );
        }
	}
	public void _showAssistantTouch( boolean p_bShow )
	{
	    m_bShowAssistantTouch = p_bShow;
	    m_Handler.post
        (
            new Runnable() 
            {
                public void run() 
                {
                    if( m_bShowAssistantTouch == true )
                    {
                        MyWindowManager.setVisibleForsmall( View.VISIBLE );
                    }
                    else
                    {
                        MyWindowManager.setVisibleForsmall( View.INVISIBLE );
                    }
                }
            }
        );
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPosX( int p_nLeft )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPosX( p_nLeft );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPosY( int p_nTop )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPosY( p_nTop );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxWidth( int p_nWidth )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setWidth( p_nWidth );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxHeight( int p_nHeight )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setHeight( p_nHeight );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxStyle( String p_sStyle )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setStyle( p_sStyle );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxFocus()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				//pEditBox.setFocus( true );
				int len=pEditBox.m_pEditBox.getText().length();
				pEditBox.setCursorPosition( len );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxBlur()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				//这个地方必须要隐藏Editbox否则恢复的时候会弹出软件盘
				pEditBox.m_bFocus = false;
				pEditBox.setVisible( false );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxVisible( boolean p_bVisible )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setVisible( p_bVisible );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxValue( String p_sValue )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){		
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setValue( p_sValue );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static String getEditBoxValue()
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				return pEditBox.getValue();
			}
		}
		return "";
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxColor( int p_nColor )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setTextColor( p_nColor );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxFontSize( int p_nFontSize )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setTextSize( p_nFontSize );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPos( int p_nX,int p_nY )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPos( p_nX,p_nY );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxSize( int p_nW,int p_nH )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setSize( p_nW,p_nH );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxCursorPosition( int p_nPos )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setCursorPosition( p_nPos );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxMaxLength( int p_nMaxLength )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setMaxLength( p_nMaxLength );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxPassword( boolean p_bPassword )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.setPassWord( p_bPassword );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void setEditBoxNumberOnly( boolean p_bNumberOnly )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
		if( pEditBox != null )
		{
			pEditBox.setNumberOnly( p_bNumberOnly );
		}
		}
	}
	
	public static void setMultiAble( boolean p_bMultiAble )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
		LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
		if( pEditBox != null )
		{
			pEditBox.setMultiAble(p_bMultiAble);
		}
		}
	}
	
	//------------------------------------------------------------------------------
	public static void setEditBoxRegular( String p_sRegular )
	{
		ExportJavaFunction exjf = ExportJavaFunction.GetInstance();
		if( exjf!=null ){
			LayaEditBox pEditBox = exjf.m_pEngine.getEditBox();
			if( pEditBox != null )
			{
				pEditBox.SetRegular( p_sRegular );
			}
		}
	}
	//------------------------------------------------------------------------------
	public static void alert( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_sAlertMsg = msg;
			pExport.m_nAlertCallbackType = 0;
			pExport.JSAlert();
		}
	}
	//------------------------------------------------------------------------------
	public static void alertCallback( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_sAlertMsg = msg;
			pExport.m_nAlertCallbackType = 1;
			pExport.JSAlert();
		}
	}
	//------------------------------------------------------------------------------
	public static void alertExit( String msg )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_sAlertMsg = msg;
			pExport.m_nAlertCallbackType = 2;
			pExport.JSAlert();
		}
	}
	//------------------------------------------------------------------------------
	public static void setHrefToJava(String href)
	{
		m_sHref = href;
	}
	//------------------------------------------------------------------------------
	public void JSAlert()
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
//					if( expjava.m_pEngine.getIsPlug() )
					if(expjava!=null)
						LayaWrapper.MyJSAlert("提示", m_sAlertMsg,m_nAlertCallbackType , expjava.m_pEngine);
//					else
//						LayaWrapper.MyJSAlert("提示", m_sAlertMsg,m_nAlertCallbackType, null);
				}
			}
		);
	}
	//屏幕一直亮着的 设定
	//------------------------------------------------------------------------------
	public static void setScreenWakeLock( boolean p_bWakeLock )
	{
		LayaConch3.GetInstance().setRuntimeExt();//set runtime ext
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_bScreenWakeLock = p_bWakeLock;
			pExport.setScreenWakeLock();
		}
	}
	//------------------------------------------------------------------------------
	public void setScreenWakeLock()
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					if( m_pEngine.getIsPlug() )
					{
						LayaConch3.GetInstance().setScreenWakeLock( m_bScreenWakeLock );
					}
					else
					{
						LayaWrapper.setScreenWakeLock( m_bScreenWakeLock );
					}
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	public static void showLoadingView( boolean p_bShow )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null){
			pExport.m_bShowLoading = p_bShow;
			pExport.showLoadingView();
		}
	}
	//------------------------------------------------------------------------------
	public void showLoadingView()
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					//TODO
					//kuo comment code here
					if(m_pEngine != null)
						m_pEngine.setLoadingViewVisible(m_bShowLoading);
						//m_pEngine.getLoadingView().show( m_bShowLoading );
				}
			}
		);
	}
	
	public static void reloadApp(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._reloadApp();
	}
	public void _reloadApp(){
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					GL2JNILib.reloadJS();
				}
			}
		);
	}
	
	public static void postCmdToMain(int nCmd, int nParam1, int nParam2){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._postCmdToMain(nCmd, nParam1, nParam2);
	}
	
	public void _postCmdToMain(int nCmd, int nParam1, int nParam2){
		class _Runnable implements Runnable{ 
			int m_nCmd = 0;
			int m_nP1 = 0;
			int m_nP2 = 0;

			public _Runnable(int nCmd, int nParam1, int nParam2){
				m_nCmd = nCmd; m_nP1 = nParam1; m_nP2 = nParam2;
			}
			public void run(){
				GL2JNILib.onRunCmd(m_nCmd, m_nP1, m_nP2);
			}
		};
		
		m_Handler.post(new _Runnable(nCmd, nParam1, nParam2));
	}
	//------------------------------------------------------------------------------
	public static String getWifiMac()
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID != null )
		{
			String wifi=pDevID.GetWifiMac();
			return (wifi==null)?"":wifi;
		}
		return "";
	}
	//------------------------------------------------------------------------------
	public static String getAndroidModel()
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID != null )
		{
			return pDevID.GetPhoneModelAndSDK();
		}
		return "";
	}
	//------------------------------------------------------------------------------
	public static float getScreenInch()
	{
		return 4;
		//TODO return MainActivity.GetScreenInch();
	}
	//------------------------------------------------------------------------------
	public static void setTouchMoveRange( float p_fMM )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp!=null)
			exp.m_pEngine.getCanvas().m_TouchFilter.setMoveRangeMM( p_fMM );
	}
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	public static void setScreenOrientation( int p_nOrientation  )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null) return;
		if( !exp.m_pEngine.getIsPlug() )
		{
//			LayaWrapper.GetInstance().m_LayaEngineContext.setRequestedOrientation(p_nOrientation);
			((Activity)exp.m_pEngine.mCtx).setRequestedOrientation(p_nOrientation);
		}
		Log.i("0", ">>>>>>ExportJavaFunction setScreenOrientation=" + p_nOrientation );
		boolean bLandscape = false;
		switch( p_nOrientation )
		{
		case ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE:
			bLandscape= true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_USER:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_BEHIND:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_NOSENSOR:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE:
			bLandscape = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE:
			bLandscape = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT:
			bLandscape = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR:
			bLandscape = false;
			break;
		}
		exp.m_pEngine.m_bHorizontalScreen = bLandscape;
	}
	public static int getScreenOrientation( )
	{
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null)
			return 0;
		if( !exp.m_pEngine.getIsPlug() ){
		    return ((Activity)exp.m_pEngine.mCtx).getRequestedOrientation();
			//return LayaWrapper.GetInstance().m_LayaEngineContext.getRequestedOrientation();
		}
		return 0;
	}
	// ------------------------------------------------------------------------------
	public static void ShowMessage(String pMessage){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._showMessage(pMessage);
	}
	public void _showMessage(final String pMessage)
	{
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
					if(expjava!=null)
						LayaWrapper.ShowMessage(pMessage,expjava.m_pEngine);
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	public static void showWaiting( boolean p_bShow )
	{
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport!=null)
			pExport._showWaiting( p_bShow );
	}
	//------------------------------------------------------------------------------
	public void _showWaiting( boolean p_bShow )
	{
		m_bShowWating = p_bShow;
		m_Handler.post
		(
			new Runnable() 
			{
				public void run() 
				{
					ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
					if(expjava!=null && expjava.m_pEngine != null)						
						LayaWrapper.showWaiting( m_bShowWating,expjava.m_pEngine );
				}
			}
		);
	}
	//------------------------------------------------------------------------------
	
	public static String getMarketName()
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.getMarketName();
		}
		return null;
	}
	//------------------------------------------------------------------------------
	public static int getPayType()
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.getPayType();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	public static int getLoginType()
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null)
		{
			return pLayaPlatform.getLoginType();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	public static int getChargeType()
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.getChargeType();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	public static void login(String JsonParam)
	{
		Log.e("",">>>>>>>>>>>>>>> login param = "+JsonParam);
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.login(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void switchUser(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.switchUser(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void logout(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.logout(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void enterPlatform(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.enterPlatform(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void enterBBS(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.enterBBS(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void enterFeedback(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.enterFeedback(JsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void buyProps( String jsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.buyProps( jsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static int getEnterPlatformType()
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.getEnterPlatformType();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	public static void setRechargeInfo( String jsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.setRechargeInfo( jsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void enterAccountMgr(String JsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.enterAccountMgr(JsonParam);
		}
	} 
	//------------------------------------------------------------------------------
	public static void enterShareAndFeed(String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.shareAndFeed(p_sJsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void enterInvite( String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.invite( p_sJsonParam);
		}
	}
	//------------------------------------------------------------------------------
	public static void  authorize( String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.authorize( p_sJsonParam );
		}
	}
	//------------------------------------------------------------------------------
	public static void  refreshToken( String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			Log.i("0",">>>>>>>refreshToken =" + p_sJsonParam );
			pLayaPlatform.refreshToken( p_sJsonParam );
		}
	}
	//------------------------------------------------------------------------------
	public static void recharge( String jsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.recharge( jsonParam );
		}
	}
	//------------------------------------------------------------------------------
	public static void getGameFriends( String p_sJsonParam )
	{
		Log.i("0",">>>>>>>getGameFriends =" + p_sJsonParam );
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.getGameFriends( p_sJsonParam);
		}
	}
	
	//------------------------------------------------------------------------------
	public static void sendToDesktop( String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.sendToDesktop( p_sJsonParam );
		}
	}
	//------------------------------------------------------------------------------

	public static int canSendToDesktop(String p_sJsonParam){
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.canSendToDesktop(p_sJsonParam);
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	public static void sendMessageToPlatform( String p_sParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.sendMessageToPlatform( p_sParam );
		}
	}
	public static void getUserInfo( String p_sJsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.getUserInfo( p_sJsonParam );
		}
	}
	public static void getAvailableLoginType(String p_sJsonParam)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.getAvailableLoginType( p_sJsonParam );
		}
	}
	public static String getMarketValue(String key)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			return pLayaPlatform.getMarketValue( key );
		}
		return "";
	}
	public static void setMarketValue(String key,String value)
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.setMarketValue(key,value);
		}
	}
	public static void openTopicCircle( String p_sJsonParam )
	{
		LayaPlatformGlue pLayaPlatform = LayaPlatformGlue.GetInstance();
		if(pLayaPlatform!=null){
			pLayaPlatform.openTopicCircle( p_sJsonParam );
		}
	}
	public static String GetDeviceInfo(){
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		DevID pDevID = exp.m_pEngine.getDevID();
		if( pDevID == null )
			return "";
		JSONObject obj = new JSONObject();
		JSONArray imeiArr = new JSONArray();
		imeiArr.put(pDevID.GetIMEI());
		JSONArray imsiArr = new JSONArray();
		imsiArr.put(pDevID.GetIMSI());
		try {
			int temp[]=pDevID.GetResolutionArray();
			obj.put("dpi", temp[2]);
			obj.put("resolution",temp[1]+"*"+temp[0]);
			obj.put("guid",pDevID.GetWifiMac());
			obj.put("imei", imeiArr);
			obj.put("imsi", imsiArr);
			obj.put("os", "android");
			obj.put("osversion", pDevID.GetOSVersion());
			obj.put("phonemodel", pDevID.GetPhoneModelAndSDK());
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return obj.toString();
	}
	public static String JSToJavaBridge(String jsName,String methodName,String jsonParam){
		ExportJavaFunction exp = ExportJavaFunction.GetInstance();
		if(exp==null )
			return "";
		return exp.m_pEngine.executeJavaCode(jsName, methodName, jsonParam);
	}
	//------------------------------------------------------------------------------
	public static void vibrate(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return;
		pExport._vibrate();
	}
	public void _vibrate(){
		Vibrator vibrator = (Vibrator)(m_pEngine.getGameContext().getSystemService(Context.VIBRATOR_SERVICE));
        /* 
         * 想设置震动大小可以通过改变pattern来设定，如果开启时间太短，震动效果可能感觉不到 
         * */  
      long [] pattern = {10,100,100,200};   // 停止 开启 停止 开启   
      vibrator.vibrate(pattern,-1);           //重复两次上面的pattern 如果只想震动一次，index设为-1
		//vibrator.vibrate(10000);
	}
	public static float getContextedType(){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return NetworkReceiver.NET_UNKNOWN;
		NetworkReceiver rec = pExport.m_pEngine.getNetworkReceiver();		
		return rec.getConnectedType(pExport.m_pEngine.getGameContext());
	}
	public static String RuntimeCallJavaPArray(String desc,String method, Object[] params ){
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return "";
		Log.e("", "RuntimeCallJavaPArray params.length = "+params.length+" desc="+desc+" method="+method);
		if(desc.equalsIgnoreCase("callExternalMethod")){
			return PluginNotify.reciveInfoFromJSEx(method,params);
		}
		return "";
	}
	public static void RuntimeCallJava(String method, final String params ){
		Log.e("","RuntimeCallJava:"+method+","+params);
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		if(pExport==null)
			return;
		if(method.compareTo("createWebview")==0){
			pExport.m_Handler.post
			(
				new Runnable() 
				{	
					public void run() 
					{
						LayaWebViewManager.getInstance().createWebView(params);
					}
				}
			);
		}else if(method.compareTo("callWebviewJsMethod")==0){
			pExport.m_Handler.post
			(
				new Runnable() 
				{
					public void run() 
					{
						LayaWebViewManager.getInstance().callWebviewJsMethod(params);
					}
				}
			);
		}else if(method.compareTo("callExternalMethod")==0){
			pExport.m_Handler.post
			(
				new Runnable() 
				{
					public void run() 
					{
						PluginNotify.reciveInfoFromJS(params);
					}
				}
			);
		}else if(method.compareTo("onLoadingEvent")==0){
			pExport.m_Handler.post
			(
				new Runnable() 
				{
					public void run() 
					{
						//LayaWebViewManager.getInstance().callWebviewJsMethod(params);
					}
				}
			);
		}
	}
	
}
