package laya.game.browser;


import laya.game.conch.LayaConch3;
import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AbsoluteLayout;
import android.widget.ImageButton;


//------------------------------------------------------------------------------
class MyWebViewClient extends WebViewClient 
{
	public LayaExternalWebView m_pExternalWebView = null;
	//------------------------------------------------------------------------------
	public MyWebViewClient()
	{
	}
	//------------------------------------------------------------------------------
	public boolean shouldOverrideUrlLoading( WebView p_pView, String p_sUrl ) 
	{
		Log.i("", "shouldOverrideUrlLoading>>>>>>>>>>>>>>>>>>>>");
		p_pView.loadUrl( p_sUrl );
		return true;
	}
	//------------------------------------------------------------------------------
	public void onPageStarted(WebView view, String url, Bitmap favicon) {
		Log.d("","onPageStarted url="+url);
		super.onPageStarted(view, url,favicon);
	}
	//------------------------------------------------------------------------------
    public void onLoadResource(WebView view, String url) 
	{
		super.onLoadResource(view, url);
    }
    //------------------------------------------------------------------------------
	public void onPageFinished(android.webkit.WebView view, java.lang.String url)
	{
		Log.i("", "onPageFinished>>>>>>>>>>>>>>>>>>>>");
		super.onPageFinished(view, url);
	}
	//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
/*class ButtonClickListener implements View.OnClickListener
{ 	 
	public LayaExternalWebView m_pExternalWebView = null;
	public ButtonClickListener( LayaExternalWebView p_pExternalWebView )
	{
		m_pExternalWebView = p_pExternalWebView;
	}
	public void onClick( View v ) 
	{ 
	   if( m_pExternalWebView.m_pCloseButton == v )
	   {
		   m_pExternalWebView.closeWebView();
	   }
	   else if( m_pExternalWebView.m_pBackButton == v )
	   {
		   m_pExternalWebView.getWebView().goBack();
	   }
	   else if( m_pExternalWebView.m_pForwardButton == v )
	   {
		   m_pExternalWebView.getWebView().goForward();
	   }
	   else if( m_pExternalWebView.m_pReloadButton == v )
	   {
		   m_pExternalWebView.getWebView().reload();
	   }
	}
}*/
//------------------------------------------------------------------------------
class ReadyData
{
	public int m_nX = 0;
	public int m_nY = 0;
	public int m_nWidth = 0;
	public int m_nHeight = 0;
	public String m_sUrl = ""; 
}
//------------------------------------------------------------------------------
public class LayaExternalWebView extends AbsoluteLayout 
{
	public ImageButton			m_pCloseButton = null;
	public ImageButton			m_pBackButton = null;
	public ImageButton			m_pForwardButton = null;
	public ImageButton			m_pReloadButton = null;
	public ReadyData			m_pReadyData = new ReadyData();
	private MyWebViewClient 	m_pWebViewClient = null;
	private WebView				m_pWebView = null;
	//private MainActivity		m_pMainActivity = null;
	private boolean				m_bIsAdd = false;
	private int					m_nX = 0;
	private int					m_nY = 0;
	private int					m_nWidth = 0;
	private int					m_nHeight = 0;
	public LayaConch3 m_pEngine=null;
	//------------------------------------------------------------------------------
	public LayaExternalWebView( Context context, LayaConch3 eng ) 
	{
		super( context );
		m_pEngine = eng;
		boolean bPlug = m_pEngine.getIsPlug();
		//m_pMainActivity = p_pActivity;
		m_pWebViewClient = new MyWebViewClient();
		m_pWebViewClient.m_pExternalWebView=this;
		m_pWebView = new WebView( context );
		m_pWebView.setWebViewClient( m_pWebViewClient );
		m_pWebView.getSettings().setJavaScriptEnabled(true);
		m_pWebView.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
		this.addView( m_pWebView );
		m_pWebView.setWebChromeClient( new WebChromeClient() );
		m_pWebView.addJavascriptInterface(this, "ExtWebView" );  
		m_pWebView.setOnKeyListener(new View.OnKeyListener() {  
			@Override    
			public boolean onKey(View v, int keyCode, KeyEvent event) {    
                if (event.getAction() == KeyEvent.ACTION_DOWN) {    
                    if (keyCode == KeyEvent.KEYCODE_BACK && m_pWebView.canGoBack()) {  //表示按返回键时的操作  
                    	//m_pWebView.goBack();   //后退    
                    	closeWebView();
                        return true;    //已处理    
                    }else if (keyCode == KeyEvent.KEYCODE_BACK && !m_pWebView.canGoBack()) {
                    	closeWebView();
                    	return true;
					}
                }    
                return false;    
            }   
		});
	}
	public void destory()
	{
		this.removeAllViews();
		if(m_pWebViewClient!=null)
		{
			m_pWebViewClient.m_pExternalWebView=null;
			m_pWebViewClient=null;
		}
		m_pEngine=null;
		m_pWebView=null;
	}
	//------------------------------------------------------------------------------
	public WebView getWebView()
	{
		return m_pWebView;
	}
	//------------------------------------------------------------------------------
	public boolean showWebView()
	{
		if( m_pReadyData == null ) return false;
		return showWebView( m_pReadyData.m_sUrl, m_pReadyData.m_nX, m_pReadyData.m_nY, m_pReadyData.m_nWidth, m_pReadyData.m_nHeight );
	}
	
	public void closeWebView()
	{
		m_pWebView.clearFocus();
		m_pWebView.loadDataWithBaseURL(null, "","text/html", "utf-8",null);  
		setVisibility( INVISIBLE );
		GL2JNILib.closeExternalWebView();
	}
	//------------------------------------------------------------------------------
	public boolean showWebView( String p_sUrl,int p_nX,int p_nY,int p_nWidth,int p_nHeight )
	{
		if( m_pEngine==null && LayaWrapper.GetInstance()== null ) return false;
		m_pWebView.requestFocus();
		if( p_nWidth < 310 ) p_nWidth = 310;
		if( p_nHeight < 310 ) p_nHeight = 310;
		//第一次添加
		if( m_bIsAdd == false )
		{
			m_nX = p_nX;
			m_nY = p_nY;
			m_nWidth = p_nWidth;
			m_nHeight = p_nHeight;
			if(m_pEngine.getIsPlug()){
				m_pEngine.getAbsLayout().addView(this);
			}else{
				LayaWrapper.GetInstance().m_LayaEngineContext.addContentView( this, new LayoutParams( LayoutParams.WRAP_CONTENT,LayoutParams.WRAP_CONTENT, 0, 0 ) );
			}
			//排版
			this.composing( m_nX,m_nY,m_nWidth,m_nHeight );
			this.LoadUrl( p_sUrl );
			m_bIsAdd = true;
			return true;
		}
		//宽和高度改变
		else if( m_nWidth != p_nWidth || m_nHeight != p_nHeight || m_nX != p_nX || m_nY != p_nY )
		{
			m_nWidth = p_nWidth;
			m_nHeight = p_nHeight;
			//排版
			this.composing( p_nX,p_nY,m_nWidth,m_nHeight );
			this.LoadUrl( p_sUrl );
			this.setVisibility( this.VISIBLE );
			return true;
		}
		//都没有改变
		else
		{
			this.composing( p_nX,p_nY,m_nWidth,m_nHeight );	//不调用这个会导致这种情况出问题：开web，关，开editbox，关，再开web，web无法显示
			this.LoadUrl( p_sUrl );
			this.setVisibility( this.VISIBLE );
			return true;
		}
	}
	//------------------------------------------------------------------------------
	private void composing( int p_nX,int p_nY,int p_nWidth,int p_nHeight )
	{
		AbsoluteLayout.LayoutParams pWebViewLP = new AbsoluteLayout.LayoutParams( (int)(0.8*p_nWidth),(int)(0.8*p_nHeight), p_nX+(int)(0.1*p_nWidth), p_nY+(int)(0.1*p_nHeight) );
		m_pWebView.setLayoutParams( pWebViewLP );
	}
	//------------------------------------------------------------------------------
	private void LoadUrl( String p_sUrl )
	{
		if( m_pWebViewClient != null )
		{
			m_pWebViewClient.shouldOverrideUrlLoading( m_pWebView,p_sUrl );
		}
	}
	//------------------------------------------------------------------------------
}
