package laya.game.plugin.webview;

import android.graphics.Bitmap;
import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class LayaWebViewClient extends WebViewClient {
	@Override
	public boolean shouldOverrideUrlLoading(WebView view, String url){
		Log.d("LayaWebViewClient","shouldOverrideUrlLoading url="+url);
		view.loadUrl(url);
	    return true;
	}

	 @Override 
	 public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
		 Log.d("LayaWebViewClient","failingUrl="+failingUrl+" errorcode="+errorCode+" description="+description);
	 }
	@Override
	public void onPageStarted(WebView view, String url, Bitmap favicon) {
		Log.d("LayaWebViewClient","onPageStarted url="+url);
		super.onPageStarted(view, url,favicon);
	}
	@Override
	 public void onPageFinished(WebView view, String url) {
		Log.d("LayaWebViewClient","onPageFinished url="+url);
		super.onPageFinished(view, url);
    }
}
