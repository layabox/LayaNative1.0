package laya.game.utility;

import java.io.File;
import java.io.FileInputStream;

import laya.game.browser.GL2JNILib;

import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.util.Log;

public class LayaAudioMusic 
{
	private static MediaPlayer mBackgroundMediaPlayer = null;
	private static float mLeftVolume = 0.5f;
	private static float mRightVolume = 0.5f;
	private static boolean mPaused = false;// whether music is paused state.
	private static boolean mManualPaused = false;// whether music is paused manually before the program is switched to the background.
	private static String mCurrentPath = null;
	private static Object s_lock=new Object();
	public static void uninit(){
		
		synchronized (s_lock) {
			if(mBackgroundMediaPlayer!=null){
				mBackgroundMediaPlayer.stop();
				mBackgroundMediaPlayer=null;
			}
			mCurrentPath=null;
			mPaused=false;
			mManualPaused=false;
		}
	}
	static public void playBackgroundMusic( String p_sFilePath,int p_nTimes ) 
	{  
		synchronized (s_lock) {
			if (mCurrentPath == null) 
			{
				File localStoragePath = new File(p_sFilePath);
				if( !localStoragePath.exists()){
					return;
				}
				// it is the first time to play background music or end() was called
				mBackgroundMediaPlayer = createMediaplayer( p_sFilePath );
				mCurrentPath = p_sFilePath;
			} else {
				if (!mCurrentPath.equals(p_sFilePath)) {
					// play new background music
	
					// release old resource and create a new one
					if (mBackgroundMediaPlayer != null) {
						mBackgroundMediaPlayer.release();
					}
					mBackgroundMediaPlayer = createMediaplayer(p_sFilePath);
					// record the path
					mCurrentPath = p_sFilePath;
				}
			}
			if (mBackgroundMediaPlayer != null) 
			{
				// if the music is playing or paused, stop it
				if (mBackgroundMediaPlayer.isPlaying()) {
					mBackgroundMediaPlayer.stop();
					//Log.i("MediaPlayer", "stop play");
				}
				mBackgroundMediaPlayer.reset();
				try {
					if (p_sFilePath.startsWith("/")) {
						final FileInputStream fis = new FileInputStream(p_sFilePath);
						mBackgroundMediaPlayer.setDataSource(fis.getFD());
						fis.close();
					} 
					/*else {
						//final AssetFileDescriptor assetFileDescritor = this.mContext.getAssets().openFd(pPath);
						//mediaPlayer.setDataSource(assetFileDescritor.getFileDescriptor(), assetFileDescritor.getStartOffset(), assetFileDescritor.getLength());
					}*/
		
					mBackgroundMediaPlayer.setVolume(mLeftVolume, mRightVolume);
				} catch (final Exception e) {
					Log.e("Audio",">>>>>>"+e.toString());
					mBackgroundMediaPlayer = null;
					return;//hugao
				} 
				Boolean bLoop = ((p_nTimes==-1)?true:false);
				mBackgroundMediaPlayer.setLooping( bLoop );
				try {
					mBackgroundMediaPlayer.prepare();
					//mBackgroundMediaPlayer.seekTo(0);
					mBackgroundMediaPlayer.start();
					mPaused = false;
					mBackgroundMediaPlayer.setOnCompletionListener(new OnCompletionListener(){
	                    public void onCompletion(MediaPlayer arg0) 
	                    {
	                    	GL2JNILib.audioMusicPlayEnd();
	                    }
					});
				} catch (final Exception e) {
				}
			}
		}
	} 
	static public void stopBackgroundMusic() 
	{  
		if (mBackgroundMediaPlayer != null) {
			mBackgroundMediaPlayer.stop();
			// should set the state, if not, the following sequence will be error
			// play -> pause -> stop -> resume
			mPaused = false;
		}
	}
	static public void pauseBackgroundMusic() 
	{  
		if (mBackgroundMediaPlayer != null && mBackgroundMediaPlayer.isPlaying()) {
			mBackgroundMediaPlayer.pause();
			mPaused = true;
			mManualPaused = true;
		}
	}
	static public void resumeBackgroundMusic() 
	{  
		if (mBackgroundMediaPlayer != null && mPaused) {
			mBackgroundMediaPlayer.start();
			mPaused = false;
			mManualPaused = false;
		}
	}
	static public void setBackgroundMusicVolume( float p_fVolume ) 
	{  
		if (p_fVolume < 0.0f) {
			p_fVolume = 0.0f;
		}

		if (p_fVolume > 1.0f) {
			p_fVolume = 1.0f;
		}

		mLeftVolume = mRightVolume = p_fVolume;
		if (mBackgroundMediaPlayer != null) {
			mBackgroundMediaPlayer.setVolume(mLeftVolume, mRightVolume);
		}
	}
	
	private static MediaPlayer createMediaplayer(final String pPath) 
	{
		MediaPlayer mediaPlayer = new MediaPlayer();
		return mediaPlayer;
	}
}
