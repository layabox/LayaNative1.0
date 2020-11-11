package laya.game.plugin.webview;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.webkit.ConsoleMessage;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;

public class LayaWebChromeClient extends WebChromeClient {
	private LayaWebViewManager mWebViewMgr = null;
	public LayaWebChromeClient(LayaWebViewManager _mgr){
		mWebViewMgr = _mgr;
	}
	@Override
	public void onCloseWindow(WebView window) {
		Log.d("LayaWebChromeClient","onCloseWindow"+window.getId());
		((LayaWebView)window).closeDialog();
	}
	@Override
	public boolean onConsoleMessage(ConsoleMessage consoleMessage) {
		Log.d("LayaWebChromeClient", "onConsoleMessage = "+consoleMessage.sourceId() + "\n" + consoleMessage.message() + "\n" + consoleMessage.lineNumber());
		return true;
	}
	@SuppressLint("NewApi")
	@Override 
	public boolean onCreateWindow(WebView view, boolean isDialog,
            boolean isUserGesture, Message resultMsg) {
		Log.d("LayaWebChromeClient","onCreateWindow = "+view.getId()+" url="+view.getUrl());
		LayaWebView localView = new LayaWebView(mWebViewMgr.getContext());
		//localView.setWebViewClient(new LayaWebViewClient());
	    localView.setWebChromeClient(new LayaWebChromeClient(mWebViewMgr));
	    localView.resumeTimers();
	    
		WebSettings localWebSettings = localView.getSettings();
		localWebSettings.setJavaScriptEnabled(true);
	    
	    mWebViewMgr.addLayaWebView(localView);

//		localView.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
	    ((WebView.WebViewTransport)resultMsg.obj).setWebView(localView);
	    resultMsg.sendToTarget();
	    
	    localView.showDialog();
//		WebView adapter = new WebView(mWebViewMgr.getContext());
//		adapter.setWebViewClient(new WebViewClient(){
//			@Override
//			public boolean shouldOverrideUrlLoading(WebView view, final String url){
//				mWebViewMgr.getHandler().post(new Runnable() {
//					
//						@Override
//						public void run() {
//							LayaWebView localView = new LayaWebView(mWebViewMgr.getContext());
//							localView.setHorizontalScrollBarEnabled(false);
//							localView.setVerticalScrollBarEnabled(false);
//							localView.setVerticalScrollbarOverlay(false);
//							localView.setScrollBarSize(0);
//							localView.setWebViewClient(new LayaWebViewClient());
//						    localView.setWebChromeClient(new LayaWebChromeClient(mWebViewMgr));
//						    localView.setOnTouchListener(new LayaWebTouchListener(mWebViewMgr));
//						    localView.resumeTimers();
//						    
//							WebSettings localWebSettings = localView.getSettings();
//							localWebSettings.setJavaScriptEnabled(true);
//						    localWebSettings.setDefaultTextEncodingName("utf-8");
//						    localWebSettings.setJavaScriptCanOpenWindowsAutomatically(true);
//						    localWebSettings.setSupportMultipleWindows(true);
//						    localWebSettings.setUseWideViewPort(true);	    
//						    localWebSettings.setLoadWithOverviewMode(true);
//						    
//						    localView.addJavascriptInterface(localView, "layabox_webview");
//						    localView.refreshDrawableState();	  
//						    localView.loadUrl(url);
//						    mWebViewMgr.addLayaWebView(localView);
//						    localView.showDialog();
//						}
//					}
//				);
//				view.destroy();
//			    return true;
//			}
//		});
//		((WebView.WebViewTransport)resultMsg.obj).setWebView(adapter);
//	    resultMsg.sendToTarget();    
	    
        return true;
    }
}
