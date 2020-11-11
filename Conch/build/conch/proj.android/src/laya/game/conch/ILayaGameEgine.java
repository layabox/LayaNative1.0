package laya.game.conch;

import android.content.Context;
import android.content.res.AssetManager;
import android.view.View;
import android.widget.AbsoluteLayout;

public abstract interface ILayaGameEgine {
    //  设置是否插件方式运行
    public abstract void setIsPlugin(boolean isPlugin);
	//	设置游戏缓存目录
	public abstract void setAppCacheDir(String _cacheDir);
	//	设置so路径
	public abstract void setSoPath(String _soPath);
	//	设置jar文件名
	public abstract void setJarFile(String _jarFile);
    //  设置jar路径asset
    public abstract void setAssetInfo(AssetManager am);
	//	设置so文件名
	public abstract void setSoFile(String _soFile);
	//	引擎销毁
	public abstract void onDestroy();
	//	获得游戏显示的view
	public abstract AbsoluteLayout getAbsLayout();
	//	监听游戏事件（退出）
	public abstract void setLayaEventListener(ILayaEventListener _lis);

	//	设置游戏activity的context
	public abstract void setContext(Context ctx);
	//	设置游戏地址
	public abstract void setGameUrl(String _url);
	//	游戏初始化
	public abstract void onCreate();
	//	游戏切到后台
	public abstract void onPause();
	//	游戏恢复
	public abstract void onResume();
	//	游戏停止
	public abstract void onStop();	
	//	设置是否显示loadingview
	public abstract void setShowLoadingView(boolean _isShow);
	//	设置是否拦截返回按键 默认不拦截
	public abstract void setInterceptKey(boolean _intercept);
	
	public abstract void setLoadingView(View _view);
}
