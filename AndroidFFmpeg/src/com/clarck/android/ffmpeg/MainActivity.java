package com.clarck.android.ffmpeg;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		setDataSource("/sdcard/a.mp4");
	}

	public native void setDataSource(String path);
	
	static {
		System.loadLibrary("ffmpeg");
		System.loadLibrary("ffmpeg_player");
	}
}
