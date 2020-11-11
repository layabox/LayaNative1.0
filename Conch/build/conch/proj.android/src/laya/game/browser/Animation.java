package laya.game.browser;

import java.io.InputStream;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;

public class Animation {
	// 上一次播放的时间
	private long lastPlayTime = 0;
	// 本次播放帧的ID
	private int playId = 0;
	// 动画帧的数量
	private int frameCount = 0;
	// 动画的图片资源
	private Bitmap[] frameBitmap = null;
	// 动画是否循环播放
	private boolean isLoop = false;
	// 动画结束
	private boolean isEnd = false;
	// 动画间隔时间
	private static final int ANIM_TIME = 80;

	public Animation(Context context, int[] frameBitmapId, boolean isLoop) {
		frameCount = frameBitmapId.length;
		frameBitmap = new Bitmap[frameCount];
		for (int i = 0; i < frameCount; i++) {
			frameBitmap[i] = ReadBitmap(context, frameBitmapId[i]);
		}
		this.isLoop = isLoop;
	}

	public Animation(Context context, Bitmap[] frameBitmap, boolean isLoop) {
		frameCount = frameBitmap.length;
		this.frameBitmap = frameBitmap;
		this.isLoop = isLoop;
	}

	public void DrawFrame(Canvas canvas, Paint mPaint, int x, int y, int frameId) {
		canvas.drawBitmap(frameBitmap[frameId], x, y, mPaint);
	}

	public void DrawAnimation(Canvas canvas, Paint mPaint, int x, int y) {
		if (!isEnd) {
			canvas.drawBitmap(frameBitmap[playId], x, y, mPaint);
			long time = System.currentTimeMillis();
			if (time - lastPlayTime > ANIM_TIME) {
				playId++;
				lastPlayTime = time;
				if (playId >= frameCount) {
					isEnd = true;
					if (isLoop) {
						isEnd = false;
						playId = 0;
					}
				}
			}
		}
	}

	private Bitmap ReadBitmap(Context context, int resId) {
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inPreferredConfig = Bitmap.Config.RGB_565;
		opt.inPurgeable = true;
		opt.inInputShareable = true;
		// 图片输入流
		InputStream is = context.getResources().openRawResource(resId);
		return BitmapFactory.decodeStream(is, null, opt);
	}
}