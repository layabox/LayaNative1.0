package laya.game.conch;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.Vector;

import laya.game.Netword.NetworkReceiver;
import laya.game.PlatformInterface.LayaPlatformFactory;
import laya.game.PlatformInterface.LayaPlatformGlue;
import laya.game.PlatformInterface.LayaPlatformInterface;
import laya.game.browser.ExportJavaFunction;
import laya.game.browser.GL2JNILib;
import laya.game.browser.LayaEditBox;
import laya.game.browser.LayaExternalWebView;
import laya.game.browser.LayaWrapper;
import laya.game.browser.MainCanvas;
import laya.game.browser.PluginNotify;
import laya.game.config.config;
import laya.game.device.DevID;
import laya.game.plugin.webview.LayaWebViewManager;
import laya.game.utility.LayaAudioMusic;
import laya.game.utility.ProcessInfo;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.os.Build;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnGenericMotionListener;
import android.view.View.OnKeyListener;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.ProgressBar;
import android.widget.Toast;
import laya.game.browser.ExportJavaFunction;

public class LayaConch3 implements ILayaGameEgine {
	// view
	private static final String TAG = "LayaConch3";
	public AbsoluteLayout m_pAbsLayout = null;
	public LayaExternalWebView m_pLayaExternalWebView = null;
	public LayaEditBox m_pEditBox = null;
	public View m_pExternalView = null;
	private AbsoluteLayout m_pProgressBarLayout = null;
	private AbsoluteLayout m_pEditBoxLayout = null;
	private ProgressBar m_pProgressBar = null;
	public boolean m_bShowProgressBar = false;
	private LayaPlatformInterface m_pPlatform = null;
	public MainCanvas m_pCavans = null;
	public DevID m_pDevID = null;
	private ILayaEventListener m_layaEventListener = null;
	private boolean m_interceptKey = false;
	private JavaJSBridge mJavaJSBridge;

	public int m_nScreenOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
	private AssetManager m_AM = null;
	public Context mCtx = null;
	public boolean m_bIsPlug = true;
	public ExportJavaFunction m_pExpJava = null;
	public String m_strUrl = "";
	public String m_strExt = "";
	public boolean m_bShowLoadingView = false;
	public boolean m_bHorizontalScreen = true; // 是否横屏
	private NetworkReceiver m_pNetWorkReveiver = new NetworkReceiver();
	private long m_nBackPressTime = 0;
	protected String m_strCachePath = "";// ;//com.tencent.qb.plugin.egret/";
	static public String m_strSoPath = "";
	static public String m_strJarFile = "";
	static public String m_strSoFile = "/liblaya.so";

	static public final String MARKET_MARKETNAME = "marketName";
	static public final String MARKET_WAITSCREENBKCOLOR = "waitscreenbkcolor";
	static public final String MARKET_ENTERPLATFORMTYPE = "enterplatformtype";
	static public final String MARKET_EXITSHOWWEBURL = "exitshowweburle";
	static public final String MARKET_SERVERNAME = "servername";
	static public final String MARKET_PAYTYPE = "paytype";
	static public final String MARKET_LOGINTYPE = "logintype";
	static public final String MARKET_CHARGETYPE = "chargetype";
	

	
	public static LayaConch3 ms_layaConche = null;

	public static LayaConch3 GetInstance() {
		if (ms_layaConche == null) {
			ms_layaConche = new LayaConch3();
		}
		return ms_layaConche;
	}

	public LayaConch3() {
		ms_layaConche = this;
	}

	public LayaConch3(Context ctx) {
		setContext(ctx);
		ms_layaConche = this;
	}

	public void setContext(Context ctx) {
		if (ctx == null)
			return;
		mCtx = ctx;
		m_pDevID = new DevID(ctx);
		mJavaJSBridge = new JavaJSBridge();
		LayaWebViewManager.getInstance().setContext(ctx);
	}

	public void MarketOnCreate() {
		String _marketName = m_marketBundle.getString(MARKET_MARKETNAME);
		_marketName = _marketName == null ? "" :_marketName;
		m_pPlatform = LayaPlatformFactory.GetInstance().CreateInterface(
				"laya.game.Market."
						+ _marketName);
		Log.e("", "m_pPlatform = " + m_pPlatform);
		LayaPlatformGlue.GetInstance().Init(m_pPlatform);
		if (m_pPlatform != null) {
			m_pPlatform.LP_onCreate(mCtx);
		}
	}

	public void onCreate() {
		// 监听网络
		Log.e(TAG, ">>>>>>>conchjar android-2.0.8");
		IntentFilter pFilter = new IntentFilter();
		pFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
		try 
		{
			mCtx.registerReceiver(m_pNetWorkReveiver, pFilter);
		} 
		catch (Exception e) 
		{
			Log.i("0","registerReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		Log.e(TAG, "plugin-----------------onCreate() ");
		ActivityManager am = (ActivityManager) mCtx
				.getSystemService(Context.ACTIVITY_SERVICE);
		ProcessInfo.init(am);

		if (m_bIsPlug) {
			InputStream in = getClass().getResourceAsStream(
					"/assets/config.ini");
			config.GetInstance().init(in);
			MarketOnCreate();
		} else {
			m_bShowLoadingView = config.GetInstance().m_bShowLoadingView;
            MarketOnCreate();
		}
		/*
		 * kuo add code {{ begin
		 */
		boolean initedNative = false;
		if (m_bIsPlug) {
			String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
			initedNative = GL2JNILib.initNativeLibrary(pluginPath, true);
			if (!initedNative) {
				throw new RuntimeException(
						"Failed to load native runtime library");
			}
		} else {
			if (m_strSoPath.length() > 0) {
				String pluginPath = getSoPath() + m_strSoFile;// "libegret.so";
				initedNative = GL2JNILib.initNativeLibrary(pluginPath, true);
				if (!initedNative) {
					throw new RuntimeException(
							"Failed to load native runtime library");
				}
			} else {
				initedNative = GL2JNILib.initNativeLibrary("laya", false);
				if (!initedNative) {
					throw new RuntimeException(
							"Failed to load native runtime library");

				}
			}
		}
		// 先配置一下
		GL2JNILib.configSetIsPlug(m_bIsPlug);
		
		if (m_strUrl.length() > 0) {
			GL2JNILib.configSetURL(m_strUrl);
		}

		ExportJavaFunction.m_nState = 0;
		ExportJavaFunction expjava = ExportJavaFunction.GetInstance();
		expjava.m_pEngine = this;
		expjava.Init(mCtx);
		// need parent activity to involve
		// this.mCtx.setRequestedOrientation(m_nScreenOrientation);
		// config.GetInstance().init( this );
		String _marketName = m_marketBundle.getString(MARKET_MARKETNAME);
		if ( _marketName != null && _marketName.length() > 1 && m_pPlatform != null) {
			m_pPlatform.LP_Init(mCtx);
		} else {
			Log.e("0", "==============Java流程 没有第三方平台直接调用 PlatformInitOK");
			PlatformInitOK(0);
		}
	}

	public void PlatformInitOK(int p_nFlag) {
		Log.e("0", "==============Java流程 InitMainCanvas()");
		EngineStart();
	}

	//TEST
	/**
	 * 遍历缓存目录，获得所有的缓存
	 */
	static class CacheInfo{
		int id;
		String path;
		String url;
	}
	//cachePath 需要到 appCache 那一层。
	public static Vector<CacheInfo> getCachedApp(String cachePath){
		Vector<CacheInfo> vecFile = new Vector<CacheInfo>();
		File file = new File(cachePath);
		File[] subFile = file.listFiles();		
		
        for (int i = 0; i < subFile.length; i++) {
        	CacheInfo cinfo = new CacheInfo();
            // 判断是否为文件夹
            if (subFile[i].isDirectory()) {
                String filename = subFile[i].getName();
                //vecFile.add(filename);
                cinfo.path = cachePath+"/"+filename;
                if( filename.compareTo("sessionFiles")==0){
                	//vecFile.add("-1");//临时缓存
                	cinfo.id=-1;
                }
                else{
                	//找有没有id
					String appidfile = cachePath+"/"+filename+"/sourceid/appid";
                	File appidFile = new File(appidfile);
                	if(appidFile.exists()){
                		try{
                			BufferedReader br = new BufferedReader(new FileReader(appidfile));
                			String data = br.readLine();
                			//vecFile.add(data);
                			cinfo.id=Integer.parseInt(data);
                		}catch(Exception e){
                			//vecFile.add("-2");//无法获得
                			cinfo.id=-2;
                		}
                	}else{
                		//vecFile.add("-3");//无法获得
                		cinfo.id=-3;
                	}
                }
            }
            vecFile.add(cinfo);
        }
        return vecFile;		
	}
	/**
	 * 删除一个目录。
	 * @param file
	 */
	public static void deletePath(File file) {
		Log.e("2jni","cacheMgr delete dir: "+file.toString());
		if (file.isFile()) {
			file.delete();
			return;
		}

		if (file.isDirectory()) {
			File[] childFiles = file.listFiles();
			if (childFiles == null || childFiles.length == 0) {
				file.delete();
				return;
			}

			for (int i = 0; i < childFiles.length; i++) {
				deletePath(childFiles[i]);
			}
			file.delete();
		}
	}

	//TEST
	public void EngineStart() {
		String strLayaCache = getAppCacheDir() + "/LayaCache";
		//TEST
		//Vector<CacheInfo> rets = getCachedApp(strLayaCache+"/appCache");
		//for( int i=0; i<rets.size(); i++){
		//	CacheInfo ci = rets.get(i);
		//	Log.e("2jni","CCCCCCCCC:id="+ci.id+",url="+ci.url+",path="+ci.path);
		//}
		///deletePath(new File("/data/data/com.layabox.conch4/cache/LayaCache/appCache/00c4020b"));
		//TEST
		File cacheFolder = new File(strLayaCache);
		if (!cacheFolder.exists()) {
			cacheFolder.mkdir();
		}
		Log.e(TAG, "plugin-----------------EngineStart() = "
				+ (strLayaCache + "/localstorage"));
		// 准备localStorage目录
		File localStoragePath = new File(strLayaCache + "/localstorage");
		if (!localStoragePath.exists()) {
			if (!localStoragePath.mkdirs()) {
				Log.e("", "创建localStorage目录失败！");
				ExportJavaFunction.alert("创建游戏目录失败，请清理空间或重启应用再试");
				return;
			}
		}
		GL2JNILib.SetLocalStoragePath(strLayaCache + "/localstorage");

		m_pCavans = new MainCanvas(mCtx);

		InitView();

		String cachePath = getAppCacheDir() + "/LayaCache";
		if (m_AM != null) {
			GL2JNILib.InitDLib(m_AM, "cache", cachePath);
		} else {
			GL2JNILib.InitDLib(null, getJarFile(), cachePath);// "/data/data/com.tencent.mtt/files/plugins/com.tencent.qb.plugin.egret/egret.jar",
																// cachePath );
		}
	}

	public void setShowLoadingView(boolean _isShow) {
		m_bShowLoadingView = _isShow;
	}

	// 设备控制相关
	public void setScreenOrientation(int ori) {
		// IGameApp.setScreenOrientation(ori);
	}

	public void setScreenWakeLock( boolean p_bWakeLock ) 
	{
		try
		{
			Log.i("0",">>>>>>screenWakeLock wake=" + p_bWakeLock );
			Activity activity = (Activity)(mCtx);
			if(activity==null)
			{
				return;
			}
			if( p_bWakeLock )
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
			else
			{
				Log.i("0",">>>>>>screenWakeLock ok" + p_bWakeLock );
				activity.getWindow().clearFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
			}
		}
		catch( Exception e )
		{
			Log.i("0",">>>>>>screenWakeLock error" );
		}
	}

	@SuppressLint("NewApi") @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	void InitView() {
		if (m_pAbsLayout == null) {
			m_pAbsLayout = new AbsoluteLayout(this.mCtx);
			m_pAbsLayout.setBackgroundColor(0x00ffffff);
			if (true) {
				if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB){
				m_pAbsLayout.setOnGenericMotionListener(new OnGenericMotionListener() {
					@Override
					public boolean onGenericMotion(View arg0, MotionEvent event) {
						// TODO Auto-generated method stub
						//if( MotionEvent.TOOL_TYPE_UNKNOWN == event.getToolType(0))
						int eventSource = event.getSource();
						if( (eventSource & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK
		                && event.getAction() == MotionEvent.ACTION_MOVE)
						{
							/* 获取左摇杆值 */
							float leftX = event.getAxisValue(MotionEvent.AXIS_X);
							float leftY = event.getAxisValue(MotionEvent.AXIS_Y);
							/* 获取右摇杆值 */
							float z = event.getAxisValue(MotionEvent.AXIS_Z);
							float rz = event.getAxisValue(MotionEvent.AXIS_RZ);

							float lt = event.getAxisValue(MotionEvent.AXIS_LTRIGGER);
							float rt = event.getAxisValue(MotionEvent.AXIS_RTRIGGER);
							GL2JNILib.handleJoystickEvent(leftX,leftY,z,rz,lt,rt);
						}
						return false;
					}
				});
				}
				m_pAbsLayout.setOnKeyListener(new OnKeyListener() 
				{
					@Override
					public boolean onKey(View v, int keyCode, KeyEvent event)
					{
						Log.e("input", ">>>>>>>>>>>>>>>>"+keyCode);
						if(event.getAction() == KeyEvent.ACTION_DOWN)
						{
							GL2JNILib.handleKeyEvent( keyCode,KeyEvent.ACTION_DOWN);
							/*
							switch(keyCode)
							{
							case KeyEvent.KEYCODE_ENTER:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_ENTER,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_DPAD_CENTER:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_CENTER,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_DPAD_UP:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_UP,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_DPAD_RIGHT:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_RIGHT,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_DPAD_DOWN:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_DOWN,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_DPAD_LEFT:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_LEFT,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_BACK:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_BACK,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_MENU:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_MENU,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_VOLUME_UP:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_VOLUME_UP,KeyEvent.ACTION_DOWN);
								break;
							case KeyEvent.KEYCODE_VOLUME_DOWN:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_VOLUME_DOWN,KeyEvent.ACTION_DOWN);
								break;
							}
							*/
						}
						else if(event.getAction() == KeyEvent.ACTION_UP)
						{
							GL2JNILib.handleKeyEvent( keyCode,KeyEvent.ACTION_UP );
							/*
							switch(keyCode)
							{
							case KeyEvent.KEYCODE_ENTER:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_ENTER,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_DPAD_CENTER:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_CENTER,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_DPAD_UP:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_UP,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_DPAD_RIGHT:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_RIGHT,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_DPAD_DOWN:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_DOWN,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_DPAD_LEFT:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_DPAD_LEFT,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_BACK:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_BACK,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_MENU:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_MENU,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_VOLUME_UP:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_VOLUME_UP,KeyEvent.ACTION_UP);
								break;
							case KeyEvent.KEYCODE_VOLUME_DOWN:
								GL2JNILib.handleKeyEvent(KeyEvent.KEYCODE_VOLUME_DOWN,KeyEvent.ACTION_UP);
								break;
							}
							*/
						}
						if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN)
						{
							ExportJavaFunction exp = ExportJavaFunction.GetInstance();
							if(exp==null)
							{
								//return 0;
							}
							else if(exp.m_pEngine.getIsPlug() ){
								//此处方便js接管Back事件，鉴于qq浏览器在没有初始化完成的时候就调用了back造成非法现予以屏蔽
								//GL2JNILib.postMsgToRuntime("Back", "");
								return false;
							}
							Log.e("", "onKey = " + keyCode);
							long curtm = System.currentTimeMillis();
							int MaxDelay = 3500; // Toast.LENGTH_LONG 对应的时间
							if (m_nBackPressTime == 0 || (m_nBackPressTime > 0 && curtm - m_nBackPressTime > MaxDelay)) 
							{
								m_nBackPressTime = System.currentTimeMillis();
								Activity context=(Activity)mCtx;
								if(LayaWrapper.IsFinishing(context))return true;
								Toast.makeText(mCtx, "再次点击后退键退出游戏!", Toast.LENGTH_LONG).show();
							}
							else
							{
								// 退出
								game_plugin_exitGame();
							}
							return true;
						}
						if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_UP)
						{
							return true;
						}
						return false;
					}
				});
			}
		}
		int uiParm=0;
		/*if(Build.VERSION.SDK_INT>=19)
		{
			uiParm=View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION|0x1000;
		}
		else 
		if(Build.VERSION.SDK_INT==18)
		{
			//uiParm=View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
		}
		else
		*/ if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			uiParm=View.SYSTEM_UI_FLAG_FULLSCREEN;
		}
		if(uiParm!=0)m_pAbsLayout.setSystemUiVisibility(uiParm);

		m_pAbsLayout.addView(m_pCavans);

		if (m_pLayaExternalWebView == null) {
			try
			{
				m_pLayaExternalWebView = new LayaExternalWebView(mCtx, this);
			}
			catch(Exception e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>Exception"+e.toString());
			}
			catch(Throwable e)
			{
				Log.e("LayaBoxwebView", ">>>>>>>>>>>>>>throwable"+e.toString());
			}
		}

		if (m_pEditBox == null) {

			m_pEditBoxLayout = new AbsoluteLayout(mCtx);
			m_pEditBoxLayout.setBackgroundColor(0x00000000);

			m_pEditBox = new LayaEditBox(mCtx);
			m_pEditBox.CreateEditBox();

			m_pEditBoxLayout.addView(m_pEditBox.m_pEditBox,
					new AbsoluteLayout.LayoutParams(GetScreenWidth(), GetScreenHeight(), 0, 0));
			m_pAbsLayout.addView(m_pEditBoxLayout, new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.WRAP_CONTENT,
					ViewGroup.LayoutParams.WRAP_CONTENT));
		}

		// loading条
		if (m_pProgressBarLayout == null) {
			m_pProgressBarLayout = new AbsoluteLayout(mCtx);
			m_pProgressBarLayout.setBackgroundColor(0x00000000);

			m_pProgressBar = new ProgressBar(mCtx);

			m_pProgressBarLayout.addView(m_pProgressBar,
					new AbsoluteLayout.LayoutParams(76, 76,
							(GetScreenWidth() - 76) / 2,
							(GetScreenHeight() - 76) / 2));
		}
		ViewParent temp=  m_pProgressBarLayout.getParent();
		if(temp==null)
		{	
			Log.e("",">>>>>>>>>>>>>>>>>>>ViewParent");
			m_pAbsLayout.addView(m_pProgressBarLayout, new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.WRAP_CONTENT,
				ViewGroup.LayoutParams.WRAP_CONTENT));//有可能
		}
		else
		{
			Log.e("",">>>>>>>>>>>>>>>>>>>hasViewParent");
		}
		showWaiting(false);
		Log.i("layaConch3", "m_interceptKey = " + m_interceptKey);
		if (m_interceptKey) {
			Log.i("layaConch3", "m_interceptKey OK!");
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
		}

		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics dm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(dm);
		if (m_bHorizontalScreen && dm.heightPixels > dm.widthPixels
				|| !m_bHorizontalScreen && dm.widthPixels > dm.heightPixels) {
			int tmp = dm.heightPixels;
			dm.heightPixels = dm.widthPixels;
			dm.widthPixels = tmp;
		}
		if (m_bShowLoadingView) {
			if (m_pExternalView != null) {
				m_pAbsLayout.addView(m_pExternalView,
						new AbsoluteLayout.LayoutParams(dm.widthPixels,
								dm.heightPixels, 0, 0));
			}
		}

		/*
		 * m_pLoadingView = new LoadingView(mCtx); m_pLoadingView.Init();
		 * m_pAbsLayout.addView(m_pLoadingView.getLoadingView(),new
		 * AbsoluteLayout.LayoutParams(dm.widthPixels, dm.heightPixels, 0, 0));
		 * if (!m_bShowLoading) { m_pLoadingView.show(false); }
		 */
		// No edit added on the activity
		// getLayaApplicationActivity().addContentView( m_pAbsEditLayout, new
		// ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
		// ViewGroup.LayoutParams.WRAP_CONTENT));
	}

	public void showWaiting(boolean p_bShow) {
		if (m_pProgressBarLayout != null) {
			m_bShowProgressBar = p_bShow;
			m_pProgressBar.setLayoutParams(new AbsoluteLayout.LayoutParams(76,
					76, (GetScreenWidth() - 76) / 2,
					(GetScreenHeight() - 76) / 2));
			m_pProgressBarLayout
					.setVisibility(p_bShow ? m_pProgressBarLayout.VISIBLE
							: m_pProgressBarLayout.INVISIBLE);
		}
	}

	int m_iScreenWidth = 0;
	int m_iScreenHeight = 0;
	int m_iDensityDPI = 0;
	float m_fScreenInch = 0;

	// ------------------------------------------------------------------------------
	public int GetScreenWidth() {
		// if (m_iScreenWidth != 0)
		// return m_iScreenWidth;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenWidth = pDm.widthPixels;
		return m_iScreenWidth;
	}

	// ------------------------------------------------------------------------------
	public int GetScreenHeight() {
		// if (m_iScreenHeight != 0)
		// return m_iScreenHeight;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iScreenHeight = pDm.heightPixels;
		return m_iScreenHeight;
	}

	// ------------------------------------------------------------------------------
	public int GetDensityDPI() {
		// if (m_iDensityDPI != 0)
		// return m_iDensityDPI;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		m_iDensityDPI = pDm.densityDpi;
		return m_iDensityDPI;
	}

	// ------------------------------------------------------------------------------
	public float GetScreenInch() {
		// if (m_fScreenInch != 0)
		// return m_fScreenInch;
		WindowManager wm = (WindowManager) (mCtx
				.getSystemService(Context.WINDOW_SERVICE));
		DisplayMetrics pDm = new DisplayMetrics();
		wm.getDefaultDisplay().getMetrics(pDm);
		int nWidth = pDm.widthPixels;
		int nHeight = pDm.heightPixels;
		float nDpi = pDm.densityDpi;
		m_fScreenInch = (float) (Math.sqrt(nWidth * nWidth + nHeight * nHeight) / nDpi);
		return m_fScreenInch;
	}

	// ------------------------------------------------------------------------------
	// 从res/drawable中取出一个图片。
	public Bitmap getBitmapFromResource(String resname) {
		InputStream in = getClass().getResourceAsStream(
				"/res/drawable/" + resname);
		// BufferedReader input = new BufferedReader(new InputStreamReader(in));
		Bitmap ret = BitmapFactory.decodeStream(in);
		return ret;
	}

	public void game_conch3_setAssetInfo(AssetManager am) {
		m_AM = am;
	}

	public void game_conch3_SetIsPlug(boolean b) {
		m_bIsPlug = b;
	}

	public void game_conch3_setAppWorkPath(String runpath) {// 在这里保存运行数据。
		m_strCachePath = runpath;
	}

	public AbsoluteLayout getAbsLayout() {
		return m_pAbsLayout;
	}

	public DevID getDevID() {
		return m_pDevID;
	}

	public MainCanvas getCanvas() {
		return m_pCavans;
	}

	public boolean getIsPlug() {
		return m_bIsPlug;
	}

	public LayaEditBox getEditBox() {
		return m_pEditBox;
	}

	public LayaExternalWebView getExternalWebView() {
		return m_pLayaExternalWebView;
	}

	public Context getGameContext() {
		return mCtx;
	}

	public boolean getHorizontalScreen() {
		return m_bHorizontalScreen;
	}

	public void onPause() 
	{
		if(m_pCavans!=null)
		{
			m_pCavans.onPause();
		}
		else
		{
			Log.e("Canvas", ">>>>>onPause m_pCavans is null");
		}
		LayaWebViewManager.getInstance().onPause();
		GL2JNILib.OnAppPause();
	}

	public NetworkReceiver getNetworkReceiver() {
		return m_pNetWorkReveiver;
	}

	// ------------------------------------------------------------------------------
	public void onResume() 
	{
		if(m_pCavans!=null)
		{
			m_pCavans.onResume();
		}
		else
		{
			Log.e("Canvas", ">>>>>onResume m_pCavans is null");
		}
		LayaWebViewManager.getInstance().onResume();
		GL2JNILib.OnAppResume();
	}

	public void onDestroy() {
		try 
		{
			mCtx.unregisterReceiver(m_pNetWorkReveiver);
		} 
		catch (Exception e) 
		{
			Log.i("0","unregisterReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		if(m_pCavans!=null)
		{
			m_pCavans.onPause();
		}
		else
		{
			Log.e("Canvas", ">>>>>onDestroy m_pCavans is null");
		}
		ExportJavaFunction.DelInstance();
		LayaWebViewManager.getInstance().onDestroy();
		GL2JNILib.ReleaseDLib();
	}
	
	private void destory()
	{
		if(m_pAbsLayout!=null)
			m_pAbsLayout.removeAllViews();
		m_pAbsLayout=null;
		m_pCavans=null;
		ms_layaConche = null;
		m_marketBundle=null;
		
		if(m_pProgressBarLayout!=null)
		{
			m_pProgressBarLayout.removeAllViews();
			m_pProgressBarLayout=null;
		}
		if(m_pProgressBar!=null)
		{
			m_pProgressBar=null;
		}
		if(m_pLayaExternalWebView!=null)
		{
			m_pLayaExternalWebView.destory();
			m_pLayaExternalWebView=null;
		}
		if(m_pEditBoxLayout!=null)
		{
			m_pEditBoxLayout.removeAllViews();
		    m_pEditBoxLayout = null;
		}
		if(m_pEditBox!=null)
		{
			m_pEditBox.destory();
			m_pEditBox=null;
		}
		LayaWrapper.m_pEngine=null;
		m_pProgressBarLayout = null;
		mCtx=null;
	}

	public void onStop() {
		ProcessInfo.uninit();
		try 
		{
			mCtx.unregisterReceiver(m_pNetWorkReveiver);
		} 
		catch (Exception e) 
		{
			Log.i("0","unregisterReceiver error m_pNetWorkReveiver=" + m_pNetWorkReveiver );
		}
		PluginNotify.removeAllPluginLlistener();
		ExportJavaFunction.DelInstance();
		LayaWebViewManager.getInstance().onDestroy();
		LayaPlatformGlue.DelInstance();
		GL2JNILib.ReleaseDLib();
		Log.e("dddd", "uninit LayaAudioMusic");
		LayaAudioMusic.uninit();
		if(m_pCavans!=null)
		{
			m_pCavans.onPause();
		}
		else
		{
			Log.e("Canvas", ">>>>>onStop m_pCavans is null");
		}
		destory();
		
	}

	public void game_plugin_onUrlBack() {
		GL2JNILib.onRunCmd(0x400 + 3333 + 104, -1, 0);
	}

	public void game_plugin_onUrlRefresh() {
		GL2JNILib.onRunCmd(0x400 + 3333 + 102, 0, 0);
	}

	public int game_plugin_getTouchMovRange() {
		return 10;
	}

	public void setGameUrl(String _param) {
		m_strUrl = _param;
	}
	
	public void setParamExt(String _param){
		m_strExt = _param;
	}
	
	public void setRuntimeExt()
	{
		if (m_strExt.length() > 0) {
			GL2JNILib.configSetParamExt(m_strExt);
		}
	}
	
	public void setSoFile(String _param) {
		m_strSoFile = _param;
	}

	public void setSoPath(String _param) {
		m_strSoPath = _param;
	}

	public void setJarFile(String _param) {
		m_strJarFile = _param;
	}

	public void setAppCacheDir(String _param) {
		m_strCachePath = _param;
	}

	// 获得so文件路径
	public String getSoPath() {
		return m_strSoPath;
	}

	// 获得jar文件
	public String getJarFile() {
		return m_strJarFile;
	}

	// 获得游戏缓存目录
	public String getAppCacheDir() {
		return m_strCachePath;
	}

	// 游戏主动退出游戏
	public void game_plugin_exitGame() {
		if (m_layaEventListener != null)
			m_layaEventListener.ExitGame();
	}

	// 游戏插件初始化
	public void game_conch3_init() {
		this.onCreate();
	}

	// 获得游戏需要显示的view
	public View game_conch3_get_view() {
		return m_pAbsLayout;
	}

	// 进入后台时调用
	public void game_conch3_onPause() {
		this.onPause();
	}

	// 恢复前台时调用
	public void game_conch3_onResume() {
		this.onResume();
	}

	// 退出游戏时调用
	public void game_conch3_onStop() {
		this.onStop();
	}

	@Override
	public void setLayaEventListener(ILayaEventListener _lis) {
		m_layaEventListener = _lis;

	}

	@Override
	public void setInterceptKey(boolean _intercept) {
		m_interceptKey = _intercept;
		setGameFocus();
	}

	public boolean getInterceptKey() {
		return m_interceptKey;
	}

	public void setGameFocus() {
		if (m_interceptKey) {
			Log.i("layaConch3", "m_interceptKey OK!");
			if (m_pAbsLayout == null) {
				return;
			}
			m_pAbsLayout.setFocusable(true);
			m_pAbsLayout.setFocusableInTouchMode(true);
			m_pAbsLayout.requestFocus();
		}
	}

	public ILayaJavaJSBridge getJavaJSBridge(){
		return mJavaJSBridge;
	}
	public String executeJavaCode(String jsName,String methodName,String jsonParam){
		return mJavaJSBridge.executeJavaMethod(jsName, methodName, jsonParam);
	}
	@Override
	public void setLoadingView(View _view) {
		m_pExternalView = _view;
	}

	public void setLoadingViewVisible(boolean _bShow) {
		if (m_pExternalView != null)
			m_pExternalView.setVisibility(_bShow ? View.VISIBLE : View.GONE);
	}
	
	static Bundle m_marketBundle = new Bundle();
	static public Bundle getMarketBundle(){
		if(m_marketBundle == null){
			m_marketBundle = new Bundle();
			return m_marketBundle;
		}
		return m_marketBundle;
	}
	static public void setMarketBundle( Bundle bundle){
		m_marketBundle = bundle;
	}

    @Override
    public void setAssetInfo(AssetManager am) {
        game_conch3_setAssetInfo(am);        
    }

    @Override
    public void setIsPlugin(boolean isPlugin) {
        game_conch3_SetIsPlug(isPlugin);
    }
}
