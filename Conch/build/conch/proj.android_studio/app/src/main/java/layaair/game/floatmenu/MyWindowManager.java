package layaair.game.floatmenu;

import com.layabox.conch5.R;

import layaair.game.browser.LayaWrapper;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.widget.PopupWindow;
import android.widget.PopupWindow.OnDismissListener;

public class MyWindowManager {

	/**
	 * 小悬浮窗View的实例
	 */
	public static FloatWindowSmallView smallWindow;
	/**
	 * 小悬浮窗View的参数
	 */
	public static LayoutParams smallWindowParams;

	private static WindowManager windowManager;

	private static Activity activity;


	public static boolean isRight;


	public  static void DelInstance()
	{
		smallWindowParams=null;
		if(smallWindow!=null) {
			smallWindow.setParams(null);
			//	smallWindow.destroyDrawingCache();
			windowManager.removeViewImmediate(smallWindow);
			smallWindow.destory();
			smallWindow = null;
		}
		windowManager=null;
		activity=null;
		mydisMiss=null;
		if(pw!=null) {
			pw.setOnDismissListener(null);
			pw = null;
		}
		view=null;
		myClick=null;
	}

	/**
	 * 创建一个小悬浮窗。初始位置为屏幕的右部中间位置。
	 * 
	 * @param context
	 *            必须为应用程序的Context.
	 */
	@TargetApi(Build.VERSION_CODES.HONEYCOMB)
	public static void createSmallWindow(Context context) {
		activity = (Activity) context;
		// windowManager = getWindowManager(context);
		windowManager = (WindowManager) context
				.getSystemService(Context.WINDOW_SERVICE);
		int screenWidth = windowManager.getDefaultDisplay().getWidth();
		int screenHeight = windowManager.getDefaultDisplay().getHeight();
		if (smallWindow == null) {
			smallWindow = new FloatWindowSmallView(context, windowManager);
			Log.d("", ">>>>>>>>>>>>>>smallWindowParams:>>>>"+ smallWindowParams);
			if (smallWindowParams == null) {
				Log.d("", ">>>>>>>>>>>>>>smallWindowParams:NULL");
				smallWindowParams = new LayoutParams();
				smallWindowParams.format = PixelFormat.RGBA_8888;
				smallWindowParams.flags = LayoutParams.FLAG_NOT_FOCUSABLE;
				smallWindowParams.gravity = Gravity.LEFT | Gravity.TOP;
				smallWindowParams.width = LayoutParams.WRAP_CONTENT;
				smallWindowParams.height = LayoutParams.WRAP_CONTENT;
				if (context.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
					smallWindowParams.x = screenWidth;
					smallWindowParams.y = screenHeight / 2;
				}
				if (context.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
					smallWindowParams.x = screenHeight;
					smallWindowParams.y = screenWidth / 2;
				}

			}
			smallWindow.setParams(smallWindowParams);
			smallWindow.setOnClickListener(myClick);
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				smallWindow
						.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN);
			}
			try {
				Log.d("", ">>>>>>>>>>>>>>>>>>windowManager:");
				windowManager.addView(smallWindow, smallWindowParams);
			} catch (Exception e) {
				Log.d("", ">>>>>>>>>>>>>" + e.toString());
			}
		}
	}

    private  static OnDismissListener mydisMiss=new OnDismissListener() {

		@Override
		public void onDismiss() {
			Drawable drawable = activity.getResources().getDrawable(R.mipmap.small_view);
			if (drawable != null) {
				drawable.setAlpha(80);
			}
			smallWindow.setImageDrawable(drawable);
			MyWindowManager.isIvok = false;
		}
	};
	private static OnClickListener myClick = new OnClickListener() {

		@Override
		public void onClick(View v) {
			switch (v.getId()) {
				case R.id.back_btn:
					if (isIvok) {// 为真时，此控件为cancel
						//Toast.makeText(activity, "cancel被点击了！", 5).show();
						isIvok = false;
						LayaWrapper.onPopMenu(LayaWrapper.CLOSE_BIG);
						pw.dismiss();
					} else {// 为back
						//Toast.makeText(v.getContext(), "back被点击了！", 5).show();
						pw.dismiss();
						LayaWrapper.onPopMenu(LayaWrapper.BACK_TO_MAIN);
					}

					break;
				case R.id.plush_btn:
					LayaWrapper.onPopMenu(LayaWrapper.REFRESH);
					pw.dismiss();
					break;
				case R.id.cancel_btn:

					if (isIvok) {// 为back
						isIvok = false;
						//Toast.makeText(v.getContext(), "back被点击了！", 5).show();
						pw.dismiss();
						LayaWrapper.onPopMenu(LayaWrapper.BACK_TO_MAIN);
					} else {// 为cancel
						//Toast.makeText(activity, "cancel被点击了！", 5).show();
						pw.dismiss();
						LayaWrapper.onPopMenu(LayaWrapper.CLOSE_BIG);
					}

					break;
				default:
					if (pw != null && pw.isShowing()) {
						Drawable drawable = activity.getResources().getDrawable(R.mipmap.small_view);
						if (drawable != null) {
							drawable.setAlpha(255);
						}
						smallWindow.setImageDrawable(drawable);
						pw.dismiss();
						pw.setOnDismissListener(null);
						pw = null;
					} else {
						showPopupView(v);
					}

					break;
			}

		}
	};

	public static PopupWindow pw;

	public static View view;

	public static boolean isIvok;

	public static void showPopupView(View v) {

		LayoutInflater layout = (LayoutInflater) v.getContext()
				.getApplicationContext()
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		view = layout.inflate(R.layout.big_float_menu, null);

		if (smallWindow != null) {
			if (smallWindow.isRight) {
				Log.d("", ">>>>>>>>>>>>>>>>>>>>>right");
				Drawable drawable = activity.getResources().getDrawable(R.mipmap.right);
				drawable.setAlpha(255);
				smallWindow.setImageDrawable(drawable);
			} else {
			    Drawable drawable = activity.getResources().getDrawable(R.mipmap.left);
			    drawable.setAlpha(255);
			    smallWindow.setImageDrawable(drawable);
			}
		}
		Log.d("", ">>>>>>>>>>>>>>>>>click");
		final View llView = view.findViewById(R.id.big_float_ll);
		View back = llView.findViewById(R.id.back_btn);
		View plush = llView.findViewById(R.id.plush_btn);
		View cancel = llView.findViewById(R.id.cancel_btn);
		back.setOnClickListener(myClick);
		plush.setOnClickListener(myClick);
		cancel.setOnClickListener(myClick);
		pw = new PopupWindow(view, LayoutParams.WRAP_CONTENT,
				LayoutParams.WRAP_CONTENT);
		pw.setFocusable(true);
		pw.setTouchable(true);
		pw.setBackgroundDrawable(new ColorDrawable(0xe00000));
		pw.setOutsideTouchable(true);
		//pw.setOnDismissListener(mydisMiss);
		if (smallWindow.isRight) {
			pw.showAtLocation(smallWindow, Gravity.RIGHT,
					(smallWindow.getWidth()), 0);
			translateView(view, Animation.RELATIVE_TO_SELF, 1.0f,
					Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF,
					0f, Animation.RELATIVE_TO_SELF, 0f);

		} else {

			pw.showAtLocation(smallWindow, Gravity.LEFT,
					smallWindow.getWidth(), 0);
			translateView(view, Animation.RELATIVE_TO_SELF, -1.0f,
					Animation.RELATIVE_TO_SELF, 0f,
					Animation.RELATIVE_TO_PARENT, 0f,
					Animation.RELATIVE_TO_PARENT, 0f);
			translateView(cancel, Animation.RELATIVE_TO_SELF, 0.0f,
					Animation.RELATIVE_TO_SELF, -4.0f,
					Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF,
					0f);
			translateView(back, Animation.RELATIVE_TO_SELF, 0.0f,
					Animation.RELATIVE_TO_SELF, 4.0f,
					Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF,
					0f);
			isIvok = true;

		}
		pw.update();
	}

	public static void translateView(View view, int fromXType,
			float fromXValue, int toXType, float toXValue, int fromYType,
			float fromYValue, int toYType, float toYValue) {
		TranslateAnimation animation = new TranslateAnimation(fromXType,
				fromXValue, toXType, toXValue, fromYType, fromYValue, toYType,
				toYValue);
		animation.setDuration(300);
		animation.setFillAfter(true);
		view.startAnimation(animation);
	}

	public static void setVisibleForsmall(int visibility) {
		if (smallWindow != null) {
			smallWindow.setVisibility(visibility);
		}
	}

}
