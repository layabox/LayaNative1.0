package laya.game.floatmenu;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

import com.layabox.conch5.R;

import laya.game.browser.LayaWrapper;
import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.TranslateAnimation;
import android.webkit.WebView.FindListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.PopupWindow;
import android.widget.PopupWindow.OnDismissListener;
import android.widget.Toast;

public class MyWindowManager {

	protected static final View FloatWindowSmallView = null;

	/**
	 * 小悬浮窗View的实例
	 */
	public static FloatWindowSmallView smallWindow;
	/**
	 * 小悬浮窗View的参数
	 */
	public static LayoutParams smallWindowParams;

	/**
	 * 大悬浮窗View的参数
	 */
	private static LayoutParams bigWindowParams;

	/**
	 * 用于控制在屏幕上添加或移除悬浮窗
	 */
	private static WindowManager mWindowManager;

	/**
	 * 用于获取手机可用内存
	 */
	private static ActivityManager mActivityManager;

	private static WindowManager windowManager;

	public static boolean isRight;

	private static Activity activity;

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
				.getSystemService(context.WINDOW_SERVICE);
		int screenWidth = windowManager.getDefaultDisplay().getWidth();
		int screenHeight = windowManager.getDefaultDisplay().getHeight();
		if (smallWindow == null) {
			smallWindow = new FloatWindowSmallView(context, windowManager);
			Log.d("", ">>>>>>>>>>>>>>smallWindowParams:>>>>"+ smallWindowParams);
			if (smallWindowParams == null) {
				Log.d("", ">>>>>>>>>>>>>>smallWindowParams:"
						+ smallWindowParams);
				smallWindowParams = new LayoutParams();
				// smallWindowParams.type = LayoutParams.TYPE_PHONE;
				smallWindowParams.format = PixelFormat.RGBA_8888;
				smallWindowParams.flags = LayoutParams.FLAG_NOT_FOCUSABLE;// |
																			// LayoutParams.FLAG_NOT_TOUCH_MODAL
																			// ;
				smallWindowParams.gravity = Gravity.LEFT | Gravity.TOP;
				// smallWindowParams.width = FloatWindowSmallView.viewWidth;
				// smallWindowParams.height = FloatWindowSmallView.viewHeight;

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
				    Drawable drawable = activity.getResources().getDrawable(R.drawable.small_view);
				    drawable.setAlpha(255);
				    smallWindow.setImageDrawable(drawable);
//					smallWindow.setImageResource(R.drawable.small_view);
//					smallWindow.setAlpha(1.0f);

					pw.dismiss();

					pw = null;
				} else {
					showPopupView(v);
				}

				break;
			}

		}
	};

	public static PopupWindow pw;

	public static View iv;

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
				// (view.findViewById(R.id.iv)).setVisibility(View.VISIBLE);
				Drawable drawable = activity.getResources().getDrawable(R.drawable.right);
				drawable.setAlpha(255);
				smallWindow.setImageDrawable(drawable);
//				smallWindow.setImageResource(R.drawable.right);
//				smallWindow.setAlpha(1.0f);
			} else {
				// (view.findViewById(R.id.iv_left)).setVisibility(View.VISIBLE);
			    Drawable drawable = activity.getResources().getDrawable(R.drawable.left);
			    drawable.setAlpha(255);
			    smallWindow.setImageDrawable(drawable);
//				smallWindow.setImageResource(R.drawable.left);
//				smallWindow.setAlpha(1.0f);
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
		// pw=new PopupWindow(activity);
		// pw.setWidth(LayoutParams.WRAP_CONTENT);
		// pw.setHeight(LayoutParams.WRAP_CONTENT);
		// pw.setContentView(view);
		// popupView之外设置半透明
		// if(activity!=null){
		// WindowManager.LayoutParams params = activity
		// .getWindow().getAttributes();
		// params.alpha = 0.77f;
		// activity.getWindow().setAttributes(params);}
		// 设置popupView进出动画
		// if (smallWindow.isRight) {
		// Log.d("", ">>>>>>>>>>>rigth");
		// pw.setAnimationStyle(R.style.from_right_enter);
		// } else {
		// Log.d("", ">>>>>>>>>>>>>left");
		// pw.setAnimationStyle(R.style.from_left_enter);
		// }
		pw.setFocusable(true);
		pw.setTouchable(true);
		pw.setBackgroundDrawable(new ColorDrawable(0xe00000));
		pw.setOutsideTouchable(true);
		// int[] location = new int[2];
		// smallWindow.getLocationOnScreen(location);
		// Log.d("", ">>>>>>>>>>>>>location[0]:" + location[0]);
		// 给popupView设置触摸事件
		// view.setOnTouchListener(new OnTouchListener() {
		//
		// @Override
		// public boolean onTouch(View v, MotionEvent event) {
		// // if (activity != null)
		// // closePopupView(activity);
		// Log.d("", ">>>>>>>>>>>>>>>>llView");
		// if (MyWindowManager.smallWindow.isRight) {
		// Log.d("", ">>>>>>>>>>>>>>>>llView_right");
		// translateView(llView, Animation.RELATIVE_TO_SELF, 0.0f,
		// Animation.RELATIVE_TO_SELF, 1.2f,
		// Animation.RELATIVE_TO_SELF, 0f,
		// Animation.RELATIVE_TO_SELF, 0f);
		// smallWindow.setImageResource(R.drawable.small_view);
		// smallWindow.setAlpha(1.0f);
		// } else {
		// Log.d("", ">>>>>>>>>>>>>>>>llView_left");
		// translateView(llView, Animation.RELATIVE_TO_SELF, 0.0f,
		// Animation.RELATIVE_TO_SELF, -1.2f,
		// Animation.RELATIVE_TO_SELF, 0f,
		// Animation.RELATIVE_TO_SELF, 0f);
		// smallWindow.setImageResource(R.drawable.small_view);
		// smallWindow.setAlpha(1.0f);
		// }
		//
		// pw.dismiss();
		// return false;
		// }
		//
		// });
		// Configuration configuration = v.getContext().getResources()
		// .getConfiguration();
		// int orientation = configuration.orientation;
		pw.setOnDismissListener(new OnDismissListener() {

			@Override
			public void onDismiss() {
				//Log.d("", ">>>>>>>>>>OnDismissListener");
			    Drawable drawable = activity.getResources().getDrawable(R.drawable.small_view);
			    drawable.setAlpha(80);
			    smallWindow.setImageDrawable(drawable);
//				MyWindowManager.smallWindow
//						.setImageResource(R.drawable.small_view);
//				MyWindowManager.smallWindow.setAlpha(0.3f);
				MyWindowManager.isIvok = false;
			}
		});
		if (smallWindow.isRight) {
			//Log.d("",">>>>>>>>>>>>>>>smallWindow width:"+ smallWindow.getWidth());
			pw.showAtLocation(smallWindow, Gravity.RIGHT,
					(smallWindow.getWidth()), 0);// (smallWindow.getWidth() +
			translateView(view, Animation.RELATIVE_TO_SELF, 1.0f,
					Animation.RELATIVE_TO_SELF, 0f, Animation.RELATIVE_TO_SELF,
					0f, Animation.RELATIVE_TO_SELF, 0f);
			// translateView(plush, Animation.RELATIVE_TO_SELF, 0.0f,
			// Animation.RELATIVE_TO_PARENT, -0.1f, Animation.RELATIVE_TO_SELF,
			// 0f, Animation.RELATIVE_TO_SELF, 0f);
			// translateView(back, Animation.RELATIVE_TO_SELF, 0.0f,
			// Animation.RELATIVE_TO_PARENT, -0.1f, Animation.RELATIVE_TO_SELF,
			// 0f, Animation.RELATIVE_TO_SELF, 0f);
			// translateView(cancel, Animation.RELATIVE_TO_SELF, 0.0f,
			// Animation.RELATIVE_TO_PARENT, -0.1f, Animation.RELATIVE_TO_SELF,
			// 0f, Animation.RELATIVE_TO_SELF, 0f);// 10)

		} else {

			pw.showAtLocation(smallWindow, Gravity.LEFT,
					smallWindow.getWidth(), 0);// smallWindow.getWidth() + 10
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
			// translateView(plush, Animation.RELATIVE_TO_SELF, 0.0f,
			// Animation.RELATIVE_TO_PARENT, 0.12f, Animation.RELATIVE_TO_SELF,
			// 0f, Animation.RELATIVE_TO_SELF, 0f);
			// pw.showAtLocation(smallWindow, Gravity.RIGHT, 0, 0);

		}
		pw.update();
		// pw.showAsDropDown(v,0,(-(smallWindowParams.height+pw.getHeight())));
	}

	public static void closePopupView(Activity ac) {

		if (pw != null && pw.isShowing()) {
			pw.dismiss();
			pw = null;
			LayoutParams layoutParams = ac.getWindow().getAttributes();
			layoutParams.alpha = 1.0f;
			ac.getWindow().setAttributes(layoutParams);
			smallWindow.setImageResource(R.drawable.small_view);
		}
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

	/**
	 * 将小悬浮窗从屏幕上移除。
	 * 
	 * @param context
	 *            必须为应用程序的Context.
	 */
	public static void removeSmallWindow(Context context) {
		Log.d("", ">>>>>>>>>>>context:" + context);
		if (smallWindow != null) {
			// WindowManager windowManager = getWindowManager(context);
			try {
				windowManager.removeView(smallWindow);
			} catch (Exception e) {
			}
			// windowManager.removeView(smallWindow);
			smallWindow = null;
			smallWindowParams = null;
		}
	}

	public static void setVisibleForsmall(int visibility) {
		if (smallWindow != null) {
			smallWindow.setVisibility(visibility);
		}
	}

	/**
	 * 是否有悬浮窗(包括小悬浮窗和大悬浮窗)显示在屏幕上。
	 * 
	 * @return 有悬浮窗显示在桌面上返回true，没有的话返回false。
	 */
	public static boolean isWindowShowing() {
		return smallWindow != null;
	}

	/**
	 * 如果WindowManager还未创建，则创建一个新的WindowManager返回。否则返回当前已创建的WindowManager。
	 * 
	 * @param context
	 *            必须为应用程序的Context.
	 * @return WindowManager的实例，用于控制在屏幕上添加或移除悬浮窗。
	 */
	private static WindowManager getWindowManager(Context context) {
		if (mWindowManager == null) {
			mWindowManager = (WindowManager) context
					.getSystemService(Context.WINDOW_SERVICE);
		}
		return mWindowManager;
	}

	/**
	 * 如果ActivityManager还未创建，则创建一个新的ActivityManager返回。否则返回当前已创建的ActivityManager。
	 * 
	 * @param context
	 *            可传入应用程序上下文。
	 * @return ActivityManager的实例，用于获取手机可用内存。
	 */
	private static ActivityManager getActivityManager(Context context) {
		if (mActivityManager == null) {
			mActivityManager = (ActivityManager) context
					.getSystemService(Context.ACTIVITY_SERVICE);
		}
		return mActivityManager;
	}

	/**
	 * 计算已使用内存的百分比，并返回。
	 * 
	 * @param context
	 *            可传入应用程序上下文。
	 * @return 已使用内存的百分比，以字符串形式返回。
	 */
	public static String getUsedPercentValue(Context context) {
		String dir = "/proc/meminfo";
		try {
			FileReader fr = new FileReader(dir);
			BufferedReader br = new BufferedReader(fr, 2048);
			String memoryLine = br.readLine();
			String subMemoryLine = memoryLine.substring(memoryLine
					.indexOf("MemTotal:"));
			br.close();
			long totalMemorySize = Integer.parseInt(subMemoryLine.replaceAll(
					"\\D+", ""));
			long availableSize = getAvailableMemory(context) / 1024;
			int percent = (int) ((totalMemorySize - availableSize)
					/ (float) totalMemorySize * 100);
			return percent + "%";
		} catch (IOException e) {
			e.printStackTrace();
		}
		return "悬浮窗";
	}

	/**
	 * 获取当前可用内存，返回数据以字节为单位。
	 * 
	 * @param context
	 *            可传入应用程序上下文。
	 * @return 当前可用内存。
	 */
	private static long getAvailableMemory(Context context) {
		ActivityManager.MemoryInfo mi = new ActivityManager.MemoryInfo();
		getActivityManager(context).getMemoryInfo(mi);
		return mi.availMem;
	}

}
