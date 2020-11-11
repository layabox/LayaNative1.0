package laya.game.plugin.webview;

import laya.game.browser.ExportJavaFunction;
import laya.game.browser.GL2JNILib;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.PopupWindow;

public class LayaWebView extends WebView {


	public class LayaWebViewShowRunable implements Runnable {
	
		private LayaWebView mView = null;
		public LayaWebViewShowRunable(LayaWebView _view){
			mView = _view;
		}
		@Override
		public void run() {
			//mView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
			mView.show();
			//mView.resumeTimers();
		}	
	}

	public class LayaWebViewCloseRunable implements Runnable {
	
		private LayaWebView mView = null;
		public LayaWebViewCloseRunable(LayaWebView _view){
			mView = _view;
		}
		@Override
		public void run() {
			//mView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
			mView.close();
			//mView.resumeTimers();
		}	
	}
	private FrameLayout layout = null; 
	private boolean layoutChangedOnce = false;
	private Handler mainThreadHandler = null;
	private PopupWindow popup = null;
	private int width;
	private int height;
	private int offsetX;
	private int offsetY;
	public LayaWebViewClient mWebViewClient = null;
	@SuppressWarnings("deprecation")
	public LayaWebView(Context context) {
		super(context);
		mainThreadHandler = new Handler(Looper.getMainLooper());
		mWebViewClient = new LayaWebViewClient();
		setWebViewClient( mWebViewClient);
	    getSettings().setDomStorageEnabled(true);
	    addJavascriptInterface(this, "layabox_webview");
	    Display localDisplay = ((WindowManager)context.getSystemService("window")).getDefaultDisplay();
	    int _width = localDisplay.getWidth();
	    int _height = localDisplay.getHeight();
	    width = ((int)(0.9D * _width));
	    height = ((int)(0.9D * _height));
	    offsetX = _width/20;
	    offsetY = _height/20;
	    
	    this.layout  = new FrameLayout(context);
	    FrameLayout.LayoutParams localLayoutParams1 = new FrameLayout.LayoutParams(width, height);
	    this.layout.setLayoutParams(localLayoutParams1);
	    LinearLayout.LayoutParams localLayoutParams2 = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,
	    		LinearLayout.LayoutParams.MATCH_PARENT);
	    setLayoutParams(localLayoutParams2);	    
	    this.layout.setFocusableInTouchMode(true);
	    this.popup = new PopupWindow(layout, width, height, true);  
	    this.setOnKeyListener(new OnKeyListener() {
			@Override
			public boolean onKey(View v, int keyCode, KeyEvent event) {
				if (event.getAction() == KeyEvent.ACTION_DOWN && keyCode == KeyEvent.KEYCODE_BACK) {
					closeDialog();
				}
				return false;
			}
		});
	    this.layout.addView(this);
	}
	private void show(){
	    if (this.popup == null) {
	      return;
	    }
	    this.setFocusable(true);
	    this.requestFocus();
	    this.popup.showAtLocation(this.layout, 0, this.offsetX, this.offsetY);
	}
	public void LayaWebviewOnResume(){
		Log.d("libgl2jni", "webview LayaWebviewOnResume123 w="+width+" h="+height);
		popup.setFocusable(true);
		this.scrollTo(0, 1);
		this.scrollTo(0, 0);
		this.onResume();
	}
	private void close(){
	    if (this.popup == null) {
	      return;
	    }
	    this.clearFocus();
	    this.popup.dismiss();
	}
	@JavascriptInterface
	public void closeDialog(){
		this.mainThreadHandler.post(new LayaWebViewCloseRunable(this));
	}
	@Override
	public boolean onCheckIsTextEditor(){
		return true;
	}
//	@Override
//	public boolean onCheckIsTextEditor(){
//		return true;
//	}
	@Override
	protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRec){
		Log.d("LayaWebView", "onFocusChanged direction = "+direction+" gainFocus"+gainFocus);
		 super.onFocusChanged(true, direction, previouslyFocusedRec);
	}
	@SuppressWarnings("deprecation")
	@Override
	protected void onLayout (boolean changed, int left, int top, int right, int bottom){
		Log.d("LayaWebView", "onLayout");
		if (!this.layoutChangedOnce){
			super.onLayout(changed, left, top, right, bottom);
			this.layoutChangedOnce = true;
	    }
	}
	
	@JavascriptInterface
	public void showDialog(){
		this.mainThreadHandler.post(new LayaWebViewShowRunable(this));
	}
//	private boolean mCurShow = false;
//	@Override
//	public void invalidate(){
//		super.invalidate();
//		if(getContentHeight()>0 && !mCurShow){
//			mCurShow = true;
//			showDialog();
//		}
//	}


	/**
	 * webview调用runtime中的js函数
	 * @param method 函数名
	 * @param params json参数
	 */
	@JavascriptInterface
	public void postMsgToRuntime(String method,String params){
		Log.d("LayaWebView", "postMsgToRuntime m="+method+" params="+params);
		if(method==null || method == "")
			return;
		GL2JNILib.postMsgToRuntime(method,params);
	}
	
	@JavascriptInterface
	public void setLWebViewSize(final int width,final int height){
		Log.d("LayaWebView", "setSize w="+width+" h="+height);
		final LayaWebView tmp = this;
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		pExport.m_Handler.post(new Runnable(){
				public void run(){
					LinearLayout.LayoutParams lay = (android.widget.LinearLayout.LayoutParams) tmp.getLayoutParams();
					lay.width = width;
					lay.height = height;
					//lay.gravity = Gravity.CENTER;
					tmp.setLayoutParams(lay);
				}
			}
		);
	}
	@JavascriptInterface
	public void setLWebViewMargin(final int marginLeft, final int marginTop,final int marginRight, final int marginBottom){
		final LayaWebView tmp = this;
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		pExport.m_Handler.post(new Runnable(){
				public void run(){
					LinearLayout.LayoutParams lay = (android.widget.LinearLayout.LayoutParams) tmp.getLayoutParams();
					lay.setMargins(marginLeft, marginTop, marginRight, marginBottom);
					//lay.gravity = Gravity.CENTER;
					tmp.setLayoutParams(lay);
				}
			}
		);
	}
	@JavascriptInterface
	public void setFullScreen(){
		final LayaWebView tmp = this;
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		pExport.m_Handler.post(new Runnable(){
				public void run(){
					LinearLayout.LayoutParams lay = (android.widget.LinearLayout.LayoutParams) tmp.getLayoutParams();
					lay.width = LinearLayout.LayoutParams.MATCH_PARENT;
					lay.height = LinearLayout.LayoutParams.MATCH_PARENT;
					lay.setMargins(0, 0, 0, 0);
					//lay.gravity = Gravity.CENTER;
					tmp.setLayoutParams(lay);
				}
			}
		);
	}
	@JavascriptInterface
	public void setLBackgroundColor(final long color){
		final LayaWebView tmp = this;
		ExportJavaFunction pExport = ExportJavaFunction.GetInstance();
		pExport.m_Handler.post(new Runnable(){
				public void run(){
					tmp.setBackgroundColor((int) color);
				}
			}
		);
	}
}
