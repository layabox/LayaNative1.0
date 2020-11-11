package layaair.game.floatmenu;

import java.lang.reflect.Field;

import com.layabox.conch5.R;

import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.ImageView;

public class FloatWindowSmallView extends ImageView {

	/**
	 * 记录小悬浮窗的宽度
	 */
	public static int viewWidth;

	/**
	 * 记录小悬浮窗的高度
	 */
	public static int viewHeight;

	/**
	 * 记录系统状态栏的高度
	 */
	private static int statusBarHeight;

	/**
	 * 用于更新小悬浮窗的位置
	 */
	private WindowManager windowManager;

	/**
	 * 小悬浮窗的参数
	 */
	private WindowManager.LayoutParams mParams;

	/**
	 * 记录当前手指位置在屏幕上的横坐标值
	 */
	private float xInScreen;

	/**
	 * 记录当前手指位置在屏幕上的纵坐标值
	 */
	private float yInScreen;

	/**
	 * 记录手指按下时在屏幕上的横坐标的值
	 */
	private float xDownInScreen;

	/**
	 * 记录手指按下时在屏幕上的纵坐标的值
	 */
	private float yDownInScreen;

	/**
	 * 记录手指按下时在小悬浮窗的View上的横坐标的值
	 */
	private float xInView;

	/**
	 * 记录手指按下时在小悬浮窗的View上的纵坐标的值
	 */
	private float yInView;

	public  boolean isRight = true;

	private boolean isMove = false;

	private OnClickListener onClickListener;
	
	private static final int toRight=1;
	
	private static final int toLeft=2;
	
	private Handler handler=new Handler(){
		public void handleMessage(android.os.Message msg) {
			
			int width=windowManager.getDefaultDisplay().getWidth();
			switch (msg.what) {
			case toRight:
				mParams.x+=30;
				if(mParams.x<width){
					windowManager.updateViewLayout(FloatWindowSmallView.this, mParams);
					sendEmptyMessage(toRight);
				}else{
					mParams.x=width;
					windowManager.updateViewLayout(FloatWindowSmallView.this, mParams);
				}
				break;
			case toLeft:
				mParams.x-=30;
				if(mParams.x>0){
					windowManager.updateViewLayout(FloatWindowSmallView.this, mParams);
					sendEmptyMessage(toLeft);
				}else{
					mParams.x=0;
					windowManager.updateViewLayout(FloatWindowSmallView.this, mParams);
				}
				
				break;

			default:
				break;
			}
		};
	};


	public void destory()
	{
		handler.removeCallbacksAndMessages(null);
		handler=null;
		this.setOnClickListener(null);
		this.windowManager=null;
		this.setImageDrawable(null);
		mParams=null;

	}

	public FloatWindowSmallView(Context context, WindowManager windowManager) {
		super(context);
		this.windowManager = windowManager;
		// windowManager = (WindowManager)
		// context.getSystemService(Context.WINDOW_SERVICE);
		// int m_iFloat_window_small_layout =
		// context.getResources().getIdentifier("laya_float_window_small","layout",
		// context.getPackageName());
		// int m_iSmall_window_layout_id =
		// context.getResources().getIdentifier("laya_small_window_layout","id",
		// context.getPackageName());
		// LayoutInflater.from(context).inflate(m_iFloat_window_small_layout,
		// this);
		Drawable drawable = context.getResources().getDrawable(R.mipmap.small_view);
		drawable.setAlpha(80);
		if (isRight) {
		    this.setImageDrawable(drawable);
//			this.setImageResource(R.drawable.small_view);
			MyWindowManager.isRight=isRight;
		} else {
		    this.setImageDrawable(drawable);
//			this.setImageResource(R.drawable.small_view);
			MyWindowManager.isRight=isRight;
		}
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		Rect frame =  new  Rect();  
		getWindowVisibleDisplayFrame(frame);
		int  statusBarHeight = frame.top;
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			// 手指按下时记录必要数据,纵坐标的值都需要减去状态栏高度
			xInView = event.getX();
			yInView = event.getY();
			xDownInScreen = event.getRawX();
			yDownInScreen = event.getRawY() -statusBarHeight;//- getStatusBarHeight();
			xInScreen = event.getRawX();
			yInScreen = event.getRawY()-statusBarHeight;//- getStatusBarHeight();
			isMove = false;
			break;
		case MotionEvent.ACTION_MOVE:
			//Log.d("", ">>>>>>>>>>>>move:");
			xInScreen = event.getRawX();
			yInScreen = event.getRawY() -statusBarHeight;//- getStatusBarHeight();
			if (!(Math.abs(xDownInScreen - xInScreen) < 7
					&& Math.abs(yDownInScreen - yInScreen) < 7)) {
				isMove = true;
				this.setImageResource(R.mipmap.small_view);
			}
			
			

			// 手指移动的时候更新小悬浮窗的位置
			updateViewPosition();
			
//			if(xInScreen > 35 && (windowManager.getDefaultDisplay().getWidth() - xInScreen) >35) {
//				isMove = true;
//				setImageResource(R.drawable.small_view);
//				updateViewPosition();
//			}
			break;
		case MotionEvent.ACTION_UP:
			//Log.d("", ">>>>>>>>>>>>>>>>>>up");
			if (isMove) {
				isMove = false;
				if (xInScreen > (windowManager.getDefaultDisplay().getWidth() / 2)) {// 右
					handler.sendEmptyMessage(toRight);
					isRight = true;
				} else {// 左
					handler.sendEmptyMessage(toLeft);
					isRight = false;
				}
			} else {
				//Log.d("", ">>>>>>>>>>>>>>>>>>click_up"); 
				// 如果手指离开屏幕时，xDownInScreen和xInScreen相等，且yDownInScreen和yInScreen相等，则视为触发了单击事件。
				if (Math.abs(xDownInScreen - xInScreen) < 7
						&& Math.abs(yDownInScreen - yInScreen) < 7) {
					// openBigWindow();
					
						this.setImageResource(R.mipmap.small_view);
				}
				//Log.d("", ">>>>>>>>>>>>>>>>>onClickListener:"+onClickListener);
				if (onClickListener != null) {
					onClickListener.onClick(this);
				}

			}
			break;
		default:
			break;

		}
		return true;
	}

	@Override
	public void setOnClickListener(OnClickListener l) {
		super.setOnClickListener(l);
		this.onClickListener = l;
	}

	/**
	 * 将小悬浮窗的参数传入，用于更新小悬浮窗的位置。
	 * 
	 * @param params
	 *            小悬浮窗的参数
	 */
	public void setParams(WindowManager.LayoutParams params) {
		mParams = params;
	}

	/**
	 * 更新小悬浮窗在屏幕中的位置。
	 */
	private void updateViewPosition() {
		mParams.x = (int) (xInScreen - xInView);
		mParams.y = (int) (yInScreen - yInView);
		windowManager.updateViewLayout(this, mParams);
	}

	/**
	 * 用于获取状态栏的高度。
	 * 
	 * @return 返回状态栏高度的像素值。
	 */
	private int getStatusBarHeight() {
		if (statusBarHeight == 0) {
			try {
				Class<?> c = Class.forName("com.android.internal.R$dimen");
				Object o = c.newInstance();
				Field field = c.getField("status_bar_height");
				int x = (Integer) field.get(o);
				statusBarHeight = getResources().getDimensionPixelSize(x);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return statusBarHeight;
	}

}
