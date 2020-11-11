package laya.game.browser;

import laya.game.Notifycation.LayaNotifyManager;
import laya.game.conch.ILayaEventListener;
import laya.game.floatmenu.MyWindowManager;
import laya.game.wrapper.ILayaLibWrapper;
import laya.game.wrapper.LayaWrapperFactroy;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Vibrator;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;
import com.layabox.conch5.R;

@SuppressLint("Wakelock")
public class MainActivity extends Activity {

	//------------------------------------------------------------------------------
	//static MainActivity		ms_pActivity = null;
	//------------------------------------------------------------------------------
	public int 				m_nScreenOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
	private long 			m_nBackPressTime=0;
	private ILayaLibWrapper		mLayaEngine = null;
	
    //------------------------------------------------------------------------------
	public void handleUncaughtException (Thread thread, Throwable e){
		e.printStackTrace(); // not all Android versions will print the stack trace automatically
		Intent intent = new Intent ();
		intent.setAction ("com.dawawa.SEND_LOG"); 
		intent.setFlags (Intent.FLAG_ACTIVITY_NEW_TASK); // required when starting from Application
		startActivity (intent);
		System.exit(1); // kill off the crashed app
	  }	
	// ------------------------------------------------------------------------------
	protected void onCreate(Bundle icicle) {
		enterLightsOutMode(getWindow());
		super.onCreate(icicle);
		setRequestedOrientation(m_nScreenOrientation);
		setFullScreen(true);
		MyWindowManager.createSmallWindow(this);
		
		mLayaEngine = LayaWrapperFactroy.createLayaWrapper();
		mLayaEngine.initEngine(this);
		mLayaEngine.setLayaEventListener(new layaGameListener());
		mLayaEngine.setInterceptKey(true);
		
        Intent intent = getIntent();  
        String scheme = intent.getScheme();  
        Uri uri = intent.getData();  
        Log.e("2jni","scheme:"+scheme);
        if (uri != null) {  
            String host = uri.getHost();  
            String dataString = intent.getDataString();  
            String id = uri.getQueryParameter("d");  
            String path = uri.getPath();  
            String path1 = uri.getEncodedPath();  
            String queryString = uri.getQuery();  
            Log.e("2jni","host:"+host);  
            Log.e("2jni","dataString:"+dataString);  
            Log.e("2jni","id:"+id);  
            Log.e("2jni","path:"+path);  
            Log.e("2jni","path1:"+path1);  
            Log.e("2jni","queryString:"+queryString);  
            if(dataString.length()>0){
            	String gameUrl ="http"+ dataString.substring(7);
            	Log.e("2jni","setUrl from intent:"+gameUrl);
            	mLayaEngine.setGameUrl(gameUrl);            	
            }
        }		
//		HashMap<String, Object> _option = new HashMap<String, Object>();
//		_option.put("sopath", "/data/data/com.layabox.conch3/pluginslib/");
//		mLayaEngine.setOptions(_option);
//		mLayaEngine.setGameUrl("http://10.10.30.29:8888/starthtml/index4.html");
//		mLayaEngine.setGameUrl("http://192.168.0.106:8888/starthtml/default1.html");
		
		mLayaEngine.startGame();
		//删除全部推送消息
		LayaNotifyManager.GetInstance().removeAllNotify();
    }
	
	@SuppressLint("NewApi")
	public static void enterLightsOutMode(Window window)
	{
		/*
		WindowManager.LayoutParams params = window.getAttributes();
		if(Build.VERSION.SDK_INT>=18)
		{
			int newUiOptions=View.SYSTEM_UI_FLAG_HIDE_NAVIGATION|View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
			if(Build.VERSION.SDK_INT>=19)
			{
				newUiOptions|=0x1000;
			}
			params.systemUiVisibility = newUiOptions;// .SYSTEM_UI_FLAG_LOW_PROFILE;  
	    	window.setAttributes(params);
		}*/
	}
	@Override
     public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
       if( hasFocus ) {
    	   enterLightsOutMode(getWindow());
	   }
    }
	// ---------------------------------------------------------------------------
	protected void onActivityResult(int requestCode, int resultCode,
			Intent intent) {
		super.onActivityResult(requestCode, resultCode, intent);
		((LayaWrapper)mLayaEngine).onActivityResult(requestCode, resultCode, intent);
	}
	// ------------------------------------------------------------------------------
	public void setFullScreen(boolean p_bFull) {
		if (p_bFull) {
			requestWindowFeature(Window.FEATURE_NO_TITLE);
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
					WindowManager.LayoutParams.FLAG_FULLSCREEN);
		} else {
			requestWindowFeature(Window.FEATURE_CONTEXT_MENU);
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
					WindowManager.LayoutParams.FLAG_FULLSCREEN);
		}
	}
	// ------------------------------------------------------------------------------
	protected void onPause() {
		super.onPause();
		mLayaEngine.onPause();
	}
    //------------------------------------------------------------------------------
    protected void onResume() {
    	super.onResume();	
    	mLayaEngine.onResume();
    }
    
    protected void onDestroy(){
    	super.onDestroy();	
    	mLayaEngine.onDestroy();
    }
	// ------------------------------------------------------------------------------
	public void onBackPressed() {
		// 有的第三方市场 需要退出的时候 自己处理
		Boolean bPlatformHandleExit = false;
		/*
		 * if( m_pPlatform != null ) { bPlatformHandleExit =
		 * m_pPlatform.LP_onExit(); }
		 */
		// 如果市场没有处理 再按照我们自己的方法处理
		if (bPlatformHandleExit == false) {
			long curtm = System.currentTimeMillis();
			int MaxDelay = 3500; // Toast.LENGTH_LONG 对应的时间
			if (m_nBackPressTime == 0 || (m_nBackPressTime > 0 && curtm - m_nBackPressTime > MaxDelay)) {
				m_nBackPressTime = System.currentTimeMillis();
				String strMsg = getString(R.string.laya_ZaiAnTuiChu);
				Toast.makeText(this, strMsg, Toast.LENGTH_LONG).show();
			} else {
				// 退出
				mLayaEngine.onDestroy();
				//System.exit(0);
				 super.onBackPressed();
			}
		}
	}

	// ------------------------------------------------------------------------------
	public boolean onCreateOptionsMenu(Menu menu) {
		// menu.add(0, 0, 0, "refresh");
		menu.add(0, 1, 1, "version");
		menu.add(Menu.NONE, 2, 2, "url");
		menu.add(Menu.NONE, 3, 3, "deviceInfo");
		return super.onCreateOptionsMenu(menu);
	}
	// ------------------------------------------------------------------------------
	public void MyAlert(String title, String msg) {
		Builder pBuilder = new Builder(this);
		pBuilder.setTitle(title);
		pBuilder.setMessage(msg);
		// 退出按钮
		pBuilder.setPositiveButton("OK", new OnClickListener() {
			public void onClick(DialogInterface p_pDialog, int arg1) {
				p_pDialog.cancel();
			}
		});
		AlertDialog alertdlg = pBuilder.create();
		alertdlg.setCanceledOnTouchOutside(false);
		alertdlg.show();
	}

	// ------------------------------------------------------------------------------
	public boolean onOptionsItemSelected(MenuItem menuItem) {
		super.onOptionsItemSelected(menuItem);
		switch (menuItem.getItemId()) {
		case 0:
			break;
		case 1:
			MyAlert("", LayaWrapper.getAppVersionName());
			break;
		case 2:
			MyAlert("", ExportJavaFunction.m_sHref);
			break;
		case 3: {
			// 测试用的
			int nW = LayaWrapper.GetScreenWidth();
			int nH = LayaWrapper.GetScreenHeight();
			int nDpi = LayaWrapper.GetDensityDPI();
			float nInch = LayaWrapper.GetScreenInch();
			int nMoveRange = LayaWrapper.GetInstance().m_pEngine.game_plugin_getTouchMovRange();
			MyAlert("", "w=" + nW + ",h=" + nH + ",dpi=" + nDpi + ",inch="
					+ nInch + ",MovRange=" + nMoveRange);
		}
			break;
		}
		return true;
	}

	// ------------------------------------------------------------------------------
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		//GL2JNILib.onRunCmd(110,this.getResources().getConfiguration().orientation,0);
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			Log.i("0", "=========onConfigurationChanged ORIENTATION_LANDSCAPE");
		} else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			Log.i("0", "=========onConfigurationChanged ORIENTATION_PORTRAIT");
		}
	}
	
	class layaGameListener implements ILayaEventListener{ 
		@Override
		public void ExitGame() {
			Log.i("=======", "======exit");
			mLayaEngine.onDestroy();
			System.exit(0);
		}
	}

}
