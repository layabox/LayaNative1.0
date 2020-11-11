package laya.game.autoupdateversion;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Random;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import laya.game.autoupdateversion.data.VersionData;
import laya.game.browser.LayaWrapper;
import laya.game.browser.MainActivity;
import laya.game.config.config;

public class AutoUpdateAPK {
	//static public final String CHECK_VERSION_URL = "http://localhost:80/setuptest_update.xml";
	//static public final String APK_DOWNLOAD_PATH = "mnt/sdcard/";
	//static public final String DOWNLOAD_APK_NAME = "autoupdate.apk";

	static private final int UPDATE_CHECKCOMPLETED = 1;
	static private final int UPDATE_DOWNLOADING = 2;
	static private final int UPDATE_DOWNLOAD_ERROR = 3;
	static private final int UPDATE_DOWNLOAD_COMPLETED = 4;
	static private final int UPDATE_DOWNLOAD_CANCELED = 5;

	static private AutoUpdateAPK s_instance = null;

	private IUpdateCallback m_callback = new UpdateCallback();
	private String m_szVersionUpdateURL;//= CHECK_VERSION_URL;
	private String m_szDownloadAPKName;// = DOWNLOAD_APK_NAME;
	private String m_szDownloadPath;
	private int m_iVersionCode = 0;
	private int m_iProgressValue = 0;
	private Context m_context = null;
	private VersionData m_versionData = new VersionData();
	private boolean m_hasNewVersion = false;
	private boolean m_hasCanceled = false;

	public AutoUpdateAPK(Context p_context)
	{
		s_instance = this;
		m_context = p_context;
		
		this.m_szVersionUpdateURL = config.GetInstance().m_sApkUpdateUrl;
		this.m_szDownloadAPKName = config.GetInstance().m_sUpdateAPKFileName;
		this.m_szDownloadPath = config.GetInstance().m_sUpdateDownloadPath;
		
		setVersionCode(p_context);
		downloadVersionXML(m_szVersionUpdateURL);
	}

	static public AutoUpdateAPK getInstance() {
		return s_instance;
	}

	public void cancelDownload() {
		m_hasCanceled = true;
	}

	Handler updateHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {

			switch (msg.what) {
			case UPDATE_CHECKCOMPLETED:
				m_callback.checkUpdateCompleted(getHasNewVersion(),
						m_versionData.getName() + m_versionData.getVersion());
				break;
			case UPDATE_DOWNLOADING:
				m_callback.downloadProgressChanged(m_iProgressValue);
				break;
			case UPDATE_DOWNLOAD_ERROR:

				if( LayaWrapper.GetInstance()!=null)
				{
					LayaWrapper.GetInstance().OnUpdateEnd(1);
				}

				m_callback.downloadCompleted(false, msg.obj.toString());
				break;
			case UPDATE_DOWNLOAD_COMPLETED:
				if( LayaWrapper.GetInstance()!=null)
					LayaWrapper.GetInstance().OnUpdateEnd(0);

				m_callback.downloadCompleted(true, "");
				break;
			case UPDATE_DOWNLOAD_CANCELED:

				m_callback.downloadCanceled();
			default:
				break;
			}
		}
	};

	public void updateAPK() {
		Intent intent = new Intent(Intent.ACTION_VIEW);

		intent.setDataAndType(
				Uri.fromFile(new File(m_szDownloadPath, m_szDownloadAPKName)),
				"application/vnd.android.package-archive");
		m_context.startActivity(intent);
	}

	public void downloadAPK() {
		new Thread() {
			public void run() {
				try {
					URL url = new URL(m_versionData.getDownloasURL());
					Log.i("","update apk url:"+url.toString());
					HttpURLConnection conn = (HttpURLConnection) url
							.openConnection();
					conn.setConnectTimeout(6 * 1000);
					conn.connect();
					// if (conn.getResponseCode() != 200)
					// throw new RuntimeException("请求url失败");

					int length = conn.getContentLength();
					InputStream is = conn.getInputStream();

					File fileAPK = new File(m_szDownloadPath,
							m_szDownloadAPKName);

					if (fileAPK.exists())
						fileAPK.delete();

					FileOutputStream fos = new FileOutputStream(fileAPK);

					int count = 0;
					byte buf[] = new byte[512];

					int nLastProg=0;
					do {

						int numread = is.read(buf);
						count += numread;
						m_iProgressValue = (int) (((float) count / length) * 100);
						if(m_iProgressValue!=nLastProg)
						{
							updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOADING));
							nLastProg = m_iProgressValue;
						}
						if (numread <= 0) {
							updateHandler.sendEmptyMessage(UPDATE_DOWNLOAD_COMPLETED);
							break;
						}
						fos.write(buf, 0, numread);
					} while (!m_hasCanceled);
					if (m_hasCanceled) {
						updateHandler.sendEmptyMessage(UPDATE_DOWNLOAD_CANCELED);
					}
					fos.close();
					is.close();
				} catch (MalformedURLException e) {
					e.printStackTrace();
					updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOAD_ERROR, e.getMessage()));
				} catch (IOException e) {
					e.printStackTrace();
					updateHandler.sendMessage(updateHandler.obtainMessage(UPDATE_DOWNLOAD_ERROR, e.getMessage()));
				}
			}
		}.start();
	}

	public void setVersionCode(Context p_context) {
		try {
			m_iVersionCode = p_context.getPackageManager().getPackageInfo(
					p_context.getPackageName(), 0).versionCode;
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public int getVersionCode() {
		return m_iVersionCode;
	}

	public void downloadVersionXML(String p_szURL) {
		this.setHasNewVersion(false);
		new Thread() {
			// ***************************************************************
			@Override
			public void run() {
				try {
					Random rnd=new Random();
					String url=m_szVersionUpdateURL+"?r="+rnd.nextInt();
					Log.i("","update url="+url);
					String verjson = NetHelper
							.httpStringGet(url);
					parseVersionXMLFromString(verjson);
					if (m_versionData.getVersionCode() > m_iVersionCode) {
						setHasNewVersion(true);
					}
				} catch (Exception e) {
					// TODO Auto-generated catch block
					Log.e("","自动更新失败，应该是无法连接到"+m_szVersionUpdateURL);
					if(e!=null)
					{
						//e.printStackTrace();
						//Log.i("@@@@@", e.getMessage());
					}
				}
				updateHandler.sendEmptyMessage(UPDATE_CHECKCOMPLETED);
			}
		}.start();
	}

	public boolean parseVersionXML(InputStream p_in) {
		DocumentBuilder db = null;
		Document document = null;
		try {
			db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			document = db.parse(p_in);
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Element root = document.getDocumentElement();
		return doParseVersionXML(root);
	}

	public boolean parseVersionXMLFromFile(String p_szFileName) {
		DocumentBuilder db = null;
		Document document = null;
		try {
			db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			document = db.parse(new File(p_szFileName));
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		Element root = document.getDocumentElement();
		// System.out.println("根节点名称："+root.getTagName()); //根节点名称
		return doParseVersionXML(root);
	}

	public boolean parseVersionXMLFromString(String p_szContent) {
		DocumentBuilder db = null;
		Document document = null;
		try {
			db = DocumentBuilderFactory.newInstance().newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			InputStream in = new ByteArrayInputStream(p_szContent.getBytes());
			document = db.parse(in);
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		Element root = document.getDocumentElement();
		// System.out.println("根节点名称："+root.getTagName()); //根节点名称
		return doParseVersionXML(root);
	}

	@SuppressLint({ "NewApi", "NewApi", "NewApi", "NewApi", "NewApi" })
	private boolean doParseVersionXML(Element p_root) {
		NodeList __ls = p_root.getChildNodes();
		Node node = null;
		for (int i = 0; i < __ls.getLength(); i++) {
			node = __ls.item(i);
			if (node.getNodeName().toLowerCase().equals("versioncode")) {
				m_versionData.setVersionCode(Integer.parseInt(node
						.getTextContent()));
				continue;
			}
			if (node.getNodeName().toLowerCase().equals("version")) {
				m_versionData.setVersion(node.getTextContent());
				continue;
			}
			if (node.getNodeName().toLowerCase().equals("name")) {
				m_versionData.setName(node.getTextContent());
				continue;
			}
			if (node.getNodeName().toLowerCase().equals("url")) {
				m_versionData.setDownloasURL(node.getTextContent());
				continue;
			}
		}
		//给apk地址加个参数，防止服务器的缓存
		m_versionData.setDownloasURL(m_versionData.getDownloasURL()+"?vc="+m_versionData.getVersionCode());
		return false;
	}

	public boolean getHasNewVersion() {
		return m_hasNewVersion;
	}

	public void setHasNewVersion(boolean m_hasNewVersion) {
		this.m_hasNewVersion = m_hasNewVersion;
	}

	public Context getContext() {
		// TODO Auto-generated method stub
		return this.m_context;
	}
}
