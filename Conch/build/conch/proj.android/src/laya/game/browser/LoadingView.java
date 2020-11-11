package laya.game.browser;

import java.io.IOException;
import java.io.InputStream;

import laya.game.conch.LayaConch3;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AbsoluteLayout;

public class LoadingView extends View {
	// 动画的数量
	private static final int FRAMECOUNT = 12;
	private int row = 3;
	private int column = 4;
	Animation loadingAnimation = null;
	Paint mPaint = null;
	// 绘制的当前帧
	int mAnimationState = 0;
	Bitmap mBitmap = null;
	
	private int mScreenWidth = 0,mScreenHeight = 0,mImageWidth = 0,mImageHeight = 0;
	private Context				m_pCtx = null;
	private DisplayMetrics 		m_pDm = null;
	
	public LoadingView( LayaConch3 _engine ) {
		super(_engine.getGameContext());
		m_pCtx = _engine.getGameContext();
		 
		m_pDm = new DisplayMetrics();
		WindowManager wm = (WindowManager)(m_pCtx.getSystemService(Context.WINDOW_SERVICE));
		wm.getDefaultDisplay().getMetrics(m_pDm);
		if(_engine.getHorizontalScreen() && m_pDm.heightPixels>m_pDm.widthPixels||
				!_engine.getHorizontalScreen() && m_pDm.widthPixels>m_pDm.heightPixels){
			int tmp = m_pDm.heightPixels;
			m_pDm.heightPixels = m_pDm.widthPixels;
			m_pDm.widthPixels = tmp;
		}
		mPaint = new Paint();
		Bitmap testBitmap = ReadBitmap(m_pCtx, "laya_loadingbg.jpg");
		Bitmap[] bitmap = new Bitmap[FRAMECOUNT];
		int tileWidth = testBitmap.getWidth() / column;
		int tileHeight = testBitmap.getHeight() / row;
		mScreenWidth = m_pDm.widthPixels;
		mScreenHeight = m_pDm.heightPixels;
		mImageWidth = tileWidth;
		mImageHeight = tileHeight;
		int i = 0,j=0, x = 0, y = 0,index = 0;
		for ( i = 0; i < row; i++) {
			for ( j = 0; j < column; j++) {
				bitmap[index] = BitmapClipBitmap(testBitmap, x, y, tileWidth, tileHeight);
				x += tileWidth;
				index+=1;
			}
			y += tileHeight;
			x = 0;
		}
		loadingAnimation = new Animation(m_pCtx,bitmap, true);
		this.setBackgroundColor(Color.WHITE);
	}

	protected void onDraw(Canvas canvas) {
		loadingAnimation.DrawAnimation(canvas, mPaint, (int)(mScreenWidth-mImageWidth)/2, (int)(mScreenHeight - mImageHeight)/2);
		super.onDraw(canvas);
		invalidate();
	}

	protected void onConfigurationChanged( Configuration newConfig )
	{
		super.onConfigurationChanged(newConfig);
		WindowManager wm = (WindowManager)(m_pCtx.getSystemService(Context.WINDOW_SERVICE));
		wm.getDefaultDisplay().getMetrics(m_pDm);
		/*
		boolean bComparWH = false;
		switch( newConfig.orientation )
		{
		case ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE:
			bComparWH= true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_PORTRAIT:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_USER:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_BEHIND:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_NOSENSOR:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE:
			bComparWH = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE:
			bComparWH = true;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT:
			bComparWH = false;
			break;
		case ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR:
			bComparWH = false;
			break;
		}
		*/
		mScreenWidth = m_pDm.widthPixels;
		mScreenHeight = m_pDm.heightPixels;
		Log.i("0",">>>>>>>>loadingView onConfigurationChanged orientation=" + newConfig.orientation + ",w=" + mScreenWidth + ",h=" + mScreenHeight );
		this.setLayoutParams( new AbsoluteLayout.LayoutParams( mScreenWidth,mScreenHeight, 0, 0) );
	}

	public Bitmap BitmapClipBitmap(Bitmap bitmap, int x, int y, int w, int h) {
		return Bitmap.createBitmap(bitmap, x, y, w, h);
	}

	public Bitmap ReadBitmap(Context context, String picName) {
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inPreferredConfig = Bitmap.Config.RGB_565;
		opt.inPurgeable = true;
		opt.inInputShareable = true;
		// 图片的输入流
		Bitmap image = null;
		try {
			InputStream is = m_pCtx.getResources().getAssets().open(picName);
			image = BitmapFactory.decodeStream(is, null, opt);
			is.close();
		} catch (IOException e) {
			e.printStackTrace();
		}//getClass().getResourceAsStream("/res/drawable/"+picName);
		return image;
	}
}