package laya.game.wrapper;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import laya.game.conch.ILayaEventListener;
import laya.game.conch.ILayaExternalInterface;
import laya.game.conch.ILayaJavaJSBridge;

import android.app.Activity;
import android.content.Intent;
import android.util.Log;
import android.view.View;


public abstract interface ILayaLibWrapper {
	public abstract void initEngine(Activity context);
	public abstract void setLayaEventListener(ILayaEventListener listener);
	public abstract void setLoadingView(View view);
	public abstract void setGameUrl(String url);
	public abstract void startGame();
	public abstract void setInterceptKey(boolean pInterceptKey);
	public abstract void setOptions(HashMap<String, Object> _option);
	
	public abstract void onPause();
	public abstract void onResume();
	public abstract void onDestroy();
	public abstract ILayaJavaJSBridge getJavaJSBridge();
	
	
	public abstract ILayaExternalInterface GetExternalInterface();
}
