package laya.game.plugin.webview;

import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class LayaWebTouchListener implements OnTouchListener {

	private LayaWebViewManager mWebViewMgr = null;
	public LayaWebTouchListener(LayaWebViewManager _mgr){
		mWebViewMgr = _mgr;
	}
	@Override
	public boolean onTouch(View v, MotionEvent event) {
		//Log.d("LayaWebTouchListener","Touche event = "+event.getAction());
		switch(event.getAction()){
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_UP:
			v.requestFocusFromTouch();
		}
		return false;
	}

}
