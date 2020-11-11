package laya.game.browser;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import laya.game.PlatformInterface.LayaPlatformInterface;
import laya.game.autoupdateversion.AutoUpdateAPK;
import laya.game.conch.IGameExternalInterfaceCallBack;
import laya.game.conch.ILayaEventListener;
import laya.game.conch.ILayaExternalInterface;
import laya.game.conch.ILayaJavaJSBridge;
import laya.game.conch.LayaConch3;
import laya.game.config.config;
import laya.game.plugin.webview.LayaWebViewManager;
import laya.game.wrapper.ILayaLibWrapper;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;
import android.widget.Toast;

@SuppressLint("Wakelock")
public class LayaWrapper implements ILayaExternalInterface,ILayaLibWrapper{
	public static final int AR_CHECK_UPDATE = 1;
	public static final int AR_INIT_PLATFORM = 2;
	private static int 		m_nStartActivityType = 0;	//设置网络之后，再次调用到哪个函数的标记
	
	public static final int BACK_TO_MAIN = 0;
	public static final int REFRESH = 1;
	public static final int CLOSE_BIG = 2;
	
	//------------------------------------------------------------------------------
	static 	LayaWrapper	ms_layaEngine = null;
    
    //更新apk相关的
    public 	AutoUpdateAPK 	m_pAutoUpdateAPK = null;
    public boolean 			m_bPopAD = true;
    static public AbsoluteLayout 	m_pAbsEditLayout = null;
    private LayaPlatformInterface   m_pPlatform = null;
    private long 			m_nBackPressTime=0;
    
    //kuo change code here
    static private String 	m_sAlertMsg = "";
    static private int		m_nAlertCallbackType = 0;
    public  LoadingView	m_pLoadingView = null;
    public View			m_pExternalLoadingView = null;
    static private Toast mToast;  
    
    //------------------------------------------------------------------------------
	public SensorManager 		m_pSensorManager = null;
	public Sensor				m_pSensor = null;
	public SensorEventListener 	m_pSensorListener = null;
	public  int					SENSOR_OFFSET = 6;
	public double				m_fSensorX = 0;
	public double				m_fSensorY = 0;
	public double				m_fSensorZ = 0;
	public int 					m_nScreenOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
	private String 				tempSoPath = "";
	private String 				tempSoFile = "";
	//------------------------------------------------------------------------------
	static public boolean				m_bShowProgressBar = false;
	//------------------------------------------------------------------------------
	public static Activity 	m_LayaEngineContext = null;
	public static LayaConch3	m_pEngine =  null;		
	
	
	//------------------------------------------------------------------------------
    public static LayaWrapper GetInstance()
    {
    	if (ms_layaEngine == null) {
			ms_layaEngine = new LayaWrapper();
		}
    	return ms_layaEngine; 
    }
    //------------------------------------------------------------------------------
    public LayaWrapper()
    {
    	
    }
    public void setLayaEventListener(ILayaEventListener _lis){
    	if(m_pEngine!=null)
    		m_pEngine.setLayaEventListener(_lis);
    }
    //------------------------------------------------------------------------------
    public void initEngine(Activity pContext)
    {
    	ms_layaMainActivity = pContext;
    	ms_layaEngine = this;
    	m_LayaEngineContext = pContext;
		ms_mCtx = m_LayaEngineContext;//.getApplicationContext();
		LayaWebViewManager.getInstance().setContext(ms_mCtx);
		try{
			InputStream is = m_LayaEngineContext.getResources().getAssets().open("config.ini");
			config.GetInstance().init( is );
		}catch(Exception e){
			Log.e("","打开配置文件错误。");
        }
		m_pEngine = new laya.game.conch.LayaConch3(ms_mCtx);
		m_pLoadingView = new LoadingView(m_pEngine);
		m_pEngine.game_conch3_SetIsPlug(false);
		m_pEngine.MarketOnCreate();
		if (tempSoPath.length()>0) {
			m_pEngine.setSoPath(tempSoPath);
		}
		if (tempSoFile.length()>0) {
			m_pEngine.setSoPath(tempSoFile);
		}
    }
    //------------------------------------------------------------------------------
    public void setSoPath(String pSoPath)
    {
    	tempSoPath = pSoPath;
    	if (m_pEngine!=null) {
			m_pEngine.setSoPath(pSoPath);
		}
    }
  //------------------------------------------------------------------------------
    public void setSoFile(String pSoFile)
    {
    	tempSoFile = pSoFile;
    	if (m_pEngine!=null) {
			m_pEngine.setSoFile(pSoFile);
		}
    }
    //------------------------------------------------------------------------------
    public void setGameUrl(String pUrl)
    {
    	m_pEngine.setGameUrl(pUrl);
    }
    //------------------------------------------------------------------------------
    public void startGame()
    {
    	EngineStart();
    }
    // ------------------------------------------------------------------------------
    public void EngineStart()
    {
        String strLayaCache = m_LayaEngineContext.getCacheDir().toString() + "/LayaCache";
        File cacheFolder=new File(strLayaCache); 
        if(!cacheFolder.exists()) {
        	cacheFolder.mkdir();
        }
		AssetManager am = LayaWrapper.getLayaApplicationAsset();
		String cachePath = m_LayaEngineContext.getCacheDir().toString();// + "/LayaCache";
		if (m_pExternalLoadingView!=null) {
			m_pEngine.setLoadingView(m_pExternalLoadingView);
		}else {
			m_pEngine.setLoadingView(m_pLoadingView);
		}
		m_pEngine.game_conch3_setAppWorkPath(cachePath);
		m_pEngine.game_conch3_setAssetInfo(am);
		m_pEngine.game_conch3_init();
		//准备localStorage目录
		File localStoragePath = new File(strLayaCache + "/localstorage");
		if (!localStoragePath.exists()) {
			if (!localStoragePath.mkdirs()) {
				Log.e("", "创建localstorage目录失败！");
			}
		}
		GL2JNILib.SetLocalStoragePath(strLayaCache + "/localstorage");

		//环境都准备好了，开始流程了。
		FlashScreen pFlashScreen = new FlashScreen(m_LayaEngineContext);
		pFlashScreen.start();
		
    }
    //------------------------------------------------------------------------------
 	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
 	public void InitView() {
 		View view = m_pEngine.game_conch3_get_view();
 		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
 			view.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
 	    }
 		m_LayaEngineContext.setContentView(view);
 	}
    //------------------------------------------------------------------------------
 	//控制WaitingView(转圈的)
 	public static void showWaiting( boolean p_bShow , LayaConch3 _pEngine) {
 		_pEngine.showWaiting(p_bShow);
 	}
 	//------------------------------------------------------------------------------
	//获取当前应用的版本名称
	static public String getAppVersionName() 
	{    
		String versionName = "";    
		int versioncode = 0;
		try {
			// ---get the package info---    
			PackageManager pm = LayaWrapper.ms_mCtx.getPackageManager();    
			PackageInfo pi = pm.getPackageInfo(LayaWrapper.ms_mCtx.getPackageName(), 0);    
			versionName = pi.versionName;    
			versioncode = pi.versionCode;  
			if (versionName == null || versionName.length() <= 0) {
				return "";
			}
		} catch (Exception e) {
			Log.e("VersionInfo", "Exception", e);
		}
		return versionName;
	}
	// ------------------------------------------------------------------------------
	// 这个函数是在flashScreen.java中回调回来的
	public void checkApkUpdate() {
		if (isOpenNetwork()) {
			// 自动版本更新
			if (config.GetInstance().m_bIsHandleUpdateAPK) {
				Log.e("0", "==============Java流程 checkApkUpdate");
				m_pAutoUpdateAPK = new AutoUpdateAPK(m_LayaEngineContext);
			} else {
				Log.e("0", "==============Java流程 checkApkUpdate 不许要自己管理update，调用initPlatform");
				InitView();
			}
		} else {
			settingNetwork(AR_CHECK_UPDATE);
		}
	}
	// ------------------------------------------------------------------------------
	/*
	 * 从assets目录下拷贝文件 srcFile 只能是资源根目录下的文件
	 */
	public void CopyFileFromAssets(String srcFile, String destPath) {
		File curWorkingPath = new File(destPath);
		if (!curWorkingPath.exists()) {
			Log.i("", "mkdir:" + destPath);
			if (!curWorkingPath.mkdirs()) {
				// TODO 要验证，看文件夹和文件的处理方式
				Log.e("", "copyasserts error： 创建文件夹出错，dir=" + destPath);
			}
		}

		try {
			File outFile = new File(curWorkingPath, srcFile);
			InputStream in = null;

			try {
				in = m_LayaEngineContext.getAssets().open(srcFile);
				Log.i("", "copy file: " + srcFile);
			} catch (IOException e) {
				Log.e("", "open file err:" + srcFile);
				return;
			}

			OutputStream out = new FileOutputStream(outFile);
			// Transfer bytes from in to out
			byte[] buf = new byte[1024];
			int len;
			while ((len = in.read(buf)) > 0) {
				out.write(buf, 0, len);
			}
			in.close();
			out.close();
			Log.e("", "拷贝文件" + srcFile + "成功");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// ------------------------------------------------------------------------------
	public void InitSensor() {
		m_pSensorManager = (SensorManager) m_LayaEngineContext.getSystemService(Context.SENSOR_SERVICE);
		m_pSensor = m_pSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		m_pSensorListener = new SensorEventListener() {
			public void onAccuracyChanged(Sensor sensor, int accuracy) {
			}

			public void onSensorChanged(SensorEvent event) {
				double fX = event.values[SensorManager.DATA_X];
				double fY = event.values[SensorManager.DATA_Y];
				double fZ = event.values[SensorManager.DATA_Z];
				if (Math.abs(m_fSensorX - fX) > 0.1
						|| Math.abs(m_fSensorY - fY) > 0.1
						|| Math.abs(m_fSensorZ - fZ) > 0.1) {
					m_fSensorX = fX;
					m_fSensorY = fY;
					m_fSensorZ = fZ;
					if (fX >= (SENSOR_OFFSET - 1) && fX <= (SENSOR_OFFSET + 1)
							&& Math.abs(fY) < 2) {
						GL2JNILib.onSensorChanged(-1);
						// Log.e("0", ">>>>>>>arc=-1" + ",x=" + fX + ",y=" + fY
						// + ",z=" + fZ );
					} else {
						double fArc = (double) (Math.atan2(fY, fX
								- SENSOR_OFFSET))
								- Math.PI / 2.0f;
						fArc = (fArc < 0) ? (fArc + Math.PI * 2) : fArc;
						GL2JNILib.onSensorChanged((float) (fArc));
						// Log.e("0", ">>>>>>>arc=" + fArc + ",x=" + fX + ",y="
						// + fY + ",z=" + fZ );
					}
				}
			}
		};
		m_pSensorManager.registerListener(m_pSensorListener, m_pSensor,
				SensorManager.SENSOR_DELAY_GAME);
	}
	
	public void handleUncaughtException (Thread thread, Throwable e){
		e.printStackTrace(); // not all Android versions will print the stack trace automatically
		Intent intent = new Intent ();
		intent.setAction ("com.dawawa.SEND_LOG"); 
		intent.setFlags (Intent.FLAG_ACTIVITY_NEW_TASK); // required when starting from Application
		m_LayaEngineContext.startActivity (intent);
		System.exit(1); // kill off the crashed app
	  }	
	// ------------------------------------------------------------------------------
    public static void onPopMenu(int id)
    {
    	switch (id) {
		case BACK_TO_MAIN:
			//Toast.makeText(GetInstance(), "BACK_TO_MAIN", Toast.LENGTH_SHORT).show();
			GL2JNILib.onRunCmd(0x400+3333+104, -1, 0);
			showWaiting( false , m_pEngine);
			break;
		case CLOSE_BIG:
			//Toast.makeText(GetInstance(), "CLOSE_BIG", Toast.LENGTH_SHORT).show();
			break;
		case REFRESH:
			//Toast.makeText(GetInstance(), "REFRESH", Toast.LENGTH_SHORT).show();
			GL2JNILib.onRunCmd(0x400+3333+102, 0, 0);
			showWaiting( false , m_pEngine);
			break;
		}
    }
	// ------------------------------------------------------------------------------
	public void OnUpdateEnd(int ecode) {
		if (ecode == 2) {
			Log.e("0", "==============Java流程 OnupdateEnd 调用InitPlatform()");
			InitView();
		} else {
			m_bPopAD = false;
			m_LayaEngineContext.finish();
		}
	}

	// ------------------------------------------------------------------------------
//	public void initPlatform() {
//		// 初始化平台c
//		m_pEngine.setMarketName( config.GetInstance().m_sMarketPlatformClassname!=null?config.GetInstance().m_sMarketPlatformClassname:"");
//		if ((config.GetInstance().m_sMarketPlatformClassname != null)
//				&& (config.GetInstance().m_sMarketPlatformClassname.length() > 1)) {
//			if (isOpenNetwork()) {
//				Log.e("0", "==============Java流程 第三方平台初始化");
//				m_pPlatform.LP_Init(m_LayaEngineContext);
//			} else {
//				settingNetwork(AR_INIT_PLATFORM);
//			}
//		} else {
//			Log.e("0", "==============Java流程 没有第三方平台直接调用 PlatformInitOK");
//			PlatformInitOK(0);
//		}
//	}

	// ------------------------------------------------------------------------------
	public void PlatformInitOK(int p_nFlag) {
		Log.e("0", "==============Java流程 InitMainCanvas()");
		InitView();
	}

	// ------------------------------------------------------------------------------
	public void settingNetwork(int p_nType) {
		LayaWrapper.m_nStartActivityType = p_nType;
		Builder pBuilder = new Builder(m_LayaEngineContext);
		pBuilder.setTitle("连接失败，请检查网络或与开发商联系").setMessage("是否对网络进行设置?");
		// 退出按钮
		pBuilder.setPositiveButton("是", new OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				Intent intent = null;
				try {
					String sdkVersion = android.os.Build.VERSION.SDK;
					if (Integer.valueOf(sdkVersion) > 10) {
						intent = new Intent(
								android.provider.Settings.ACTION_WIRELESS_SETTINGS);
					} else {
						intent = new Intent();
						ComponentName comp = new ComponentName(
								"com.android.settings",
								"com.android.settings.WirelessSettings");
						intent.setComponent(comp);
						intent.setAction("android.intent.action.VIEW");
					}
					m_LayaEngineContext.startActivityForResult(intent, LayaWrapper.m_nStartActivityType);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
		pBuilder.setNegativeButton("否", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				dialog.cancel();
				m_LayaEngineContext.finish();
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}

	// ------------------------------------------------------------------------------
	public boolean isOpenNetwork() {
		if (!config.GetInstance().m_bCheckNetwork)
			return true;
		ConnectivityManager connManager = (ConnectivityManager) m_LayaEngineContext.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connManager.getActiveNetworkInfo() != null) {
			return (connManager.getActiveNetworkInfo().isAvailable() && connManager
					.getActiveNetworkInfo().isConnected());
		}
		return false;
	}

	// ---------------------------------------------------------------------------
	public void onActivityResult(int requestCode, int resultCode,Intent intent) {
		if (requestCode == AR_INIT_PLATFORM) {
			InitView();
		} else if (requestCode == AR_CHECK_UPDATE) {
			checkApkUpdate();
		}
		/*
		 * if( m_pPlatform != null ) { m_pPlatform.LP_handleActivityResult(
		 * requestCode, resultCode, intent); }
		 */
	}

	// ------------------------------------------------------------------------------
	public void onPause() {
		if (m_pSensorManager != null) {
			m_pSensorManager.unregisterListener(m_pSensorListener);
		}
		if (m_pPlatform != null) {
			m_pPlatform.LP_onPause("");
		}
		m_pEngine.game_conch3_onPause();
	}
    //------------------------------------------------------------------------------
    public void onResume() {

        if( m_pSensorManager != null ) {
        	m_pSensorManager.registerListener( m_pSensorListener, m_pSensor, SensorManager.SENSOR_DELAY_GAME );
        }
        if(m_pPlatform!=null) {
        	m_pPlatform.LP_onResume(m_LayaEngineContext);
        }
        m_pEngine.game_conch3_onResume();
    }
    
    public void onDestroy(){
    	m_pEngine.game_conch3_onStop();
    }
    public void setInterceptKey(boolean _intercept)
    {
    	m_pEngine.setInterceptKey(_intercept);
    }
    public void setOptions(HashMap<String, Object> _option)
    {
    	Iterator iter = _option.entrySet().iterator();
		while(iter.hasNext()){
			Map.Entry entry = (Entry) iter.next();
			String key = (String) entry.getKey(); 
			Log.i("setOption","setOptions() key="+key+" value="+entry.getValue());
			if( key.compareToIgnoreCase("url") == 0 ){
				m_pEngine.setGameUrl((String) entry.getValue());
			}else if(key.compareToIgnoreCase("sopath") == 0){
				setSoPath((String)entry.getValue());
			}else if(key.compareToIgnoreCase("sofile") == 0){
				setSoFile((String)entry.getValue());
			}
		}
    }
    /**
     * 判断activity是否处于可用状态 
     * @param context
     * @return
     */
    public static boolean IsFinishing(Activity context)
    {
    	if(context==null||context.isFinishing())
    		return true;
    	return false;
    }
    // ------------------------------------------------------------------------------
    public static void ShowMessage(String pMessage, LayaConch3 _pEngine){
    	Activity context= (Activity) (_pEngine.getGameContext());
		if(IsFinishing(context))return;
		if(mToast == null) {  
	        mToast = Toast.makeText(context, pMessage, Toast.LENGTH_LONG);  
	    } else {  
	    	mToast.setText(pMessage); 
	    	mToast.setDuration(Toast.LENGTH_LONG);  
	    }  
	    mToast.show();
	}
	// ------------------------------------------------------------------------------
	static public int GetScreenWidth() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		return pDm.widthPixels;
	}

	// ------------------------------------------------------------------------------
	static public int GetScreenHeight() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		return pDm.heightPixels;
	}
	
	// ------------------------------------------------------------------------------
	static public int GetDensityDPI() {
		//debug for qq browser
		return 1;
		/*
		DisplayMetrics pDm = new DisplayMetrics();
        MainActivity.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
        return pDm.densityDpi;
        */
	}
	// ------------------------------------------------------------------------------
	static public float GetScreenInch() {
		DisplayMetrics pDm = new DisplayMetrics();
		LayaWrapper.getLayaApplicationActivity().getWindowManager().getDefaultDisplay().getMetrics( pDm );
		int nWidth = pDm.widthPixels;
		int nHeight = pDm.heightPixels;
		float nDpi = pDm.densityDpi;
		return (float) (Math.sqrt(nWidth * nWidth + nHeight * nHeight) / nDpi);
	}

	// ------------------------------------------------------------------------------
	public Bitmap getResImage(String name) {
		ApplicationInfo pAppInfo = m_LayaEngineContext.getApplicationInfo();
		int nResID = m_LayaEngineContext.getResources().getIdentifier(name, "drawable",pAppInfo.packageName);
		return BitmapFactory.decodeResource(m_LayaEngineContext.getResources(), nResID);
	}

	// ------------------------------------------------------------------------------
	static public void reloadApp(){
		// m_pCavans.onPause();
		// GL2JNILib.OnAppPause();
		// ExportJavaFunction.DelInstance();
		// GL2JNILib.ReleaseDLib();
		// EngineStart();
		GL2JNILib.reloadJS();
	}

	// ------------------------------------------------------------------------------
	// 通过 AlarmManager 来重新启动应用
	public void restartApp() {
		Intent intent = new Intent(m_LayaEngineContext.getApplicationContext(), LayaWrapper.class);
		PendingIntent restartIntent = PendingIntent.getActivity(
				m_LayaEngineContext.getApplicationContext(), 0, intent,
				Intent.FLAG_ACTIVITY_NEW_TASK);
		// 退出程序
		AlarmManager mgr = (AlarmManager) m_LayaEngineContext.getSystemService(Context.ALARM_SERVICE);
		mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 1000,
				restartIntent); // 1秒钟后重启应用
		m_LayaEngineContext.finishActivity(0);
	}

	// ------------------------------------------------------------------------------
	static public void MyJSAlert(String title, String msg, int p_nCallbackType, final LayaConch3 _gameEngine) {
		if (true){//m_sAlertMsg.equals(msg) == false) {
			m_sAlertMsg = msg;
			m_nAlertCallbackType = p_nCallbackType;
			//TODO 不知道为什么，插件情况下传入ctx就行，但是直接执行必须用Activity对象。
			Activity context= (Activity) (_gameEngine!=null?_gameEngine.getGameContext():LayaWrapper.getLayaApplicationActivity());
			if(IsFinishing(context))return;
			Builder pBuilder = new Builder(context);
			pBuilder.setTitle(title);
			pBuilder.setMessage(msg);
			pBuilder.setCancelable(false);
			// 退出按钮
			pBuilder.setPositiveButton("OK", new OnClickListener() {
				public void onClick(DialogInterface p_pDialog, int arg1) {
					p_pDialog.cancel();
					if (m_nAlertCallbackType == 1) {
						GL2JNILib.alertCallback();
					} else if (m_nAlertCallbackType == 2) {
						_gameEngine.game_plugin_exitGame();
					}
				}
			});
			AlertDialog alertdlg = pBuilder.create();
			alertdlg.setCanceledOnTouchOutside(false);
			alertdlg.show();
		}
	}
	//------------------------------------------------------------------------------
	static public void setScreenWakeLock( boolean p_bWakeLock ) {
		Activity activity = LayaWrapper.getLayaApplicationActivity();
		if(activity==null)
		{
			return;
		}
		if( p_bWakeLock )
		{
			LayaWrapper.getLayaApplicationActivity().getWindow().addFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
		}
		else
		{
			LayaWrapper.getLayaApplicationActivity().getWindow().clearFlags( WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON );
		}
	}
	// ------------------------------------------------------------------------------
	/* 
	 * The follow code for plugin implementation
	 * author:kuo
	 *{{ begin
	 */
	static private Activity ms_layaMainActivity=null;
	static public Context ms_mCtx=null;
	
	static public Context  getLayaApplicationContext()
	{
		return ms_mCtx;
	}
	
	static public Activity getLayaApplicationActivity()
	{
		return ms_layaMainActivity;
	}
		
	static public AssetManager  getLayaApplicationAsset()
	{
		return ms_layaMainActivity.getAssets();
	}	
	private String getLayaCacheDir()
	{
		return ms_layaMainActivity.getCacheDir().toString();
	}
	
	
	
	class CallbackExcutor implements Runnable{

		private IGameExternalInterfaceCallBack _callback;
		private String _param;
		public CallbackExcutor(IGameExternalInterfaceCallBack callback, String param){
			_callback = callback;
			_param = param;
		}
		@Override
		public void run() {
			Log.e("LayaWapper","plugin-----------------CallbackExcutor param = "+_param);
			_callback.callBack(_param);
		}
		
	}
	class JavaExternalInterface implements ExternalInterface.IExternalInterface{
		LayaWrapper _engine;
		IGameExternalInterfaceCallBack _javacallback;
		public JavaExternalInterface(LayaWrapper engine,IGameExternalInterfaceCallBack javacallback){
			_engine = engine;
			_javacallback = javacallback;
		}
		@Override
		public void call(String param) {
			Log.e("LayaWrapper","plugin-----------------JavaExternalInterface param = "+param);
			_engine.callBackExternalInterface(_javacallback, param);
		}
		@Override
		public String call(String[] param) {			
			return _engine.callBackExternalInterface(_javacallback, param);
		}		
	}
	class JavaToJSRunable implements Runnable{

		private String mMethod = null;
		private String[] mParams = null;
		JavaToJSRunable(String method, String[] param){
			mMethod = method;
			mParams = param;
		}
		@Override
		public void run() {
			ExternalInterface.getInstance().call(mMethod, mParams);
		}
		
	}
	public ILayaExternalInterface GetExternalInterface(){
		return this;
	}
	public String callBackExternalInterface(
			IGameExternalInterfaceCallBack _javacallback, String[] param) {		
		return _javacallback.callBack(param);
	}
	private void callBackExternalInterface(IGameExternalInterfaceCallBack _iGameExternalInterfaceCallBack, String param){
		View _view = m_pEngine.game_conch3_get_view();
	    if (_view != null) {
	    	_view.post(new CallbackExcutor(_iGameExternalInterfaceCallBack, param));
	    }
	}
	@Override
	public void addCallBack(String method,
			IGameExternalInterfaceCallBack _gameExternalInterfaceCallBack) {
		Log.e("","plugin-----------------addCallBack() param="+method);
		ExternalInterface.getInstance().addCallBack(method, new JavaExternalInterface(this, _gameExternalInterfaceCallBack));
	}
	@Override
	public void call(String method, String[] params) {
		Log.e("","plugin-----------------call() param1="+method+" param2="+params);
		View _view = m_pEngine.game_conch3_get_view();
		_view.post(new JavaToJSRunable(method,params));		
	}
	@Override
	public void run() {
		ExternalInterface.getInstance().run();		
	}
	@Override
	public void setLoadingView(View view) {
		m_pExternalLoadingView = view;		
	}
	@Override
	public ILayaJavaJSBridge getJavaJSBridge() {
		return m_pEngine.getJavaJSBridge();
	}
}
