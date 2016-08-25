package com.lichao.javafilepach;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;

/**
 * 文件拆分与合并
 * @author dell
 *
 */
public class MainActivity extends Activity {

	private String SD_CARD_PATH;//SD卡根目录
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        SD_CARD_PATH = Environment.getExternalStorageDirectory().getAbsolutePath();
        
    }
    
    /**
     * 拆分
     * @param btn
     */
    public void mDiff(View btn){
    	//分隔符相当于  "/"  File.separatorChar
    	String path = SD_CARD_PATH + File.separatorChar + "sintel.mp4";
    	File file = new File(path);
    	if(file.exists()){
    		Log.d("lichao", "文件存在:"+file.getAbsolutePath());
    		Log.d("lichao", "文件存在:"+file.getAbsoluteFile());
    	}else{
    		Log.d("lichao", "文件不存在");
    	}
    	String path_patten = SD_CARD_PATH + File.separatorChar + "sintel_%d.mp4";
    	FileUtils.diff(path, path_patten, 3);
    	Log.d("lichao", "拆分完毕...");
    }
    
    /**
     * 合并
     * @param btn
     */
    public void pach(View btn){
    	String path_patten = SD_CARD_PATH + File.separatorChar + "sintel_%d.mp4";
    	String path_merge = SD_CARD_PATH + File.separatorChar + "sintel_merge.mp4";
    	FileUtils.patch(path_patten, 3, path_merge);
    	Log.d("lichao", "合并完毕...");
    }
}
