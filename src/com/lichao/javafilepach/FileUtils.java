package com.lichao.javafilepach;

public class FileUtils {
	
	/**
	 * 拆分
	 * @param path 拆分文件的路径
	 * @param paht_patten 拆分成的小文件路径
	 * @param count 拆分的个数
	 */
	public native static void diff(String path, String path_patten, int count);
	
	/**
	 * 合并
	 * @param path_patten 分开的文件
	 * @param count 要合并的文件个数
	 * @param path_merge 合成功的文件
	 */
	public native static void patch(String path_patten, int count, String path_merge);

	static{
		System.loadLibrary("JavaFilepach");
	}
}
