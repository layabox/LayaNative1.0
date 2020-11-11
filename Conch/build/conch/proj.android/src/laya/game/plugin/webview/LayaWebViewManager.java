package laya.game.plugin.webview;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;

import laya.game.browser.GL2JNILib;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.webkit.JavascriptInterface;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.widget.AbsoluteLayout;
import android.widget.LinearLayout;

public class LayaWebViewManager {

	private static LayaWebViewManager _instance;
	private Context mContext = null;
	private Handler mHandler = new Handler(Looper.getMainLooper());
	private HashMap<String,LayaWebView> mWebViewMap;
	private List<LayaWebView> mArrWebView = new ArrayList<LayaWebView>();
	public LayaWebView localWebView = null;
	
	public LayaWebViewManager(){
		mWebViewMap = new HashMap();
	}
	public Handler getHandler(){return mHandler;}
	static public LayaWebViewManager getInstance(){
		if( _instance == null )
			_instance = new LayaWebViewManager();
		return _instance;
	}

	public void setContext(Context _ctx){
		mContext = _ctx;
	}
	
	public void callWebviewJsMethod(String _json){
		try {
			JSONObject _obj = new JSONObject(_json);
			callWebviewJsMethod(_obj);
		} catch (JSONException e) {
			e.printStackTrace();
		}		
	}
	public void createWebView(String _json){ 
		try {
			JSONObject _obj = new JSONObject(_json);
			createWebView(_obj);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	private void callWebviewJsMethod(JSONObject _object){
		try {
			String _name = _object.getString("name");
			LayaWebView _webView = mWebViewMap.get(_name);
			if( _webView == null ){
				return;
			}
			String _method = _object.getString("method");
			String _args = _object.getString("params");
			String str1 = _args.replace("\\", "\\\\");
		    String str2 = "javascript:" + _method + "(" + str1 + ");";
		    _webView.loadUrl(str2);
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
	}
	
	public void showExternalWebView(String p_sUrl)
	{
		//localWebView.loadUrl(p_sUrl);
		localWebView.mWebViewClient.shouldOverrideUrlLoading(localWebView, p_sUrl);
		localWebView.setVisibility(View.VISIBLE);
		localWebView.showDialog();
	}
	public void closeExternalLink()
	{
		localWebView.setVisibility(View.INVISIBLE);
		localWebView.closeDialog();
		GL2JNILib.closeExternalWebView();
	}
	public LayaWebView createWebView()
	{
		localWebView = new LayaWebView(mContext);
		localWebView.setHorizontalScrollBarEnabled(false);
	    localWebView.setVerticalScrollBarEnabled(false);
	    localWebView.setVerticalScrollbarOverlay(false);
	    localWebView.addJavascriptInterface(localWebView, "layabox_webview");
	    localWebView.setWebChromeClient(new LayaWebChromeClient(this));
	    localWebView.setOnTouchListener(new LayaWebTouchListener(this));
	    WebSettings localWebSettings = localWebView.getSettings();
	    localWebSettings.setJavaScriptCanOpenWindowsAutomatically(true);
	    localWebSettings.setSupportMultipleWindows(true);
	    localWebSettings.setUseWideViewPort(true);
	    localWebView.getSettings().setLoadWithOverviewMode(true);
	    
	    localWebView.refreshDrawableState();
	    //localWebView.setVisibility(View.VISIBLE);
	    return localWebView;
	}
	private void createWebView(JSONObject _object){
		try {
			String _name = _object.getString("name");
			LayaWebView _webView = mWebViewMap.get(_name);
			if( _webView != null )
				return;
			String _url = _object.getString("url");	
			String _strHidden = _object.getString("hidden");
			boolean _hidden = _strHidden == "true"; 
			LayaWebView localWebView = new LayaWebView(mContext);
			localWebView.setHorizontalScrollBarEnabled(false);
		    localWebView.setVerticalScrollBarEnabled(false);
		    localWebView.setVerticalScrollbarOverlay(false);
		    localWebView.addJavascriptInterface(localWebView, "layabox_webview");
//		    localWebView.setScrollBarSize(0);
//		    localWebView.setWebViewClient(new LayaWebViewClient());
		    localWebView.setWebChromeClient(new LayaWebChromeClient(this));
		    localWebView.setOnTouchListener(new LayaWebTouchListener(this));
		    //localWebView.resumeTimers();
		    WebSettings localWebSettings = localWebView.getSettings();
		    localWebSettings.setJavaScriptEnabled(true);
//		    localWebSettings.setDefaultTextEncodingName("utf-8");
		    localWebSettings.setJavaScriptCanOpenWindowsAutomatically(true);
		    localWebSettings.setSupportMultipleWindows(true);
		    localWebSettings.setUseWideViewPort(true);
		    localWebView.getSettings().setLoadWithOverviewMode(true);
		    
		    localWebView.loadUrl(_url);
		    localWebView.refreshDrawableState();
		    localWebView.setVisibility(_hidden?View.GONE:View.VISIBLE);
		    if(!_hidden)
		    	localWebView.showDialog();
			mWebViewMap.put(_name, localWebView);
			addLayaWebView(localWebView);
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}
	public void addLayaWebView(LayaWebView _view){
		if( _view == null )
			return;
		mArrWebView.add(_view);
	}
	public void removeWebView(LayaWebView _view){
		if( _view == null )
			return;
		for(LayaWebView wb : mArrWebView){
			if( wb == _view )
				mArrWebView.remove(wb);
		}
	}
	
	public Context getContext() {
		return mContext;
	}
	public void onDestroy(){
		Log.d("LayaWebView", "onDestory");
		for(LayaWebView wb : mArrWebView){
			if( wb == null )
				continue;
			wb.destroy();
			wb = null;
		}
		mArrWebView.clear();
		mWebViewMap.clear();
		_instance = null;
	}
	public void onPause() {
		Log.d("libgl2jni", "onPause");
//		for(LayaWebView wb : mArrWebView){
//			if( wb == null )
//				continue;
//			wb.pauseTimers();
//		}
	}
	  
	public void onResume() {
		Log.d("libgl2jni", "webview onResume = "+mArrWebView.size());
		for(LayaWebView wb : mArrWebView){
			if( wb == null )
				continue;
			wb.LayaWebviewOnResume();
		}
	}
	
	@JavascriptInterface
	public void sendMessage(String msg){
	    Log.d("LayaWebView", "sendMessage:" + msg);
//	    this.gameEngine.game_engine_get_externalInterface().call("webview_to_runtime_js_data", msg);
	}
}
