#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include "com_lichao_javafilepach_FileUtils.h"

#include <android/log.h>

#define LOGE(FORMAT, ...)  __android_log_print(ANDROID_LOG_ERROR, "lichao", FORMAT, ##__VA_ARGS__)
#define LOGI(FORMAT, ...)  __android_log_print(ANDROID_LOG_INFO, "lichao", FORMAT, ##__VA_ARGS__)


//获取文件大小
long get_file_size(char *path){
	FILE *fp = fopen(path, "rb");
	fseek(fp, 0, SEEK_END);
	return ftell(fp);
}
JNIEXPORT void JNICALL Java_com_lichao_javafilepach_FileUtils_diff
  (JNIEnv *env, jclass jcls, jstring path_jstr, jstring path_patten_jstr, jint file_num){
	//jstring 转 char*
	//需要分割文件
	const char* path = (*env)->GetStringUTFChars(env, path_jstr, NULL);
	//分割的子文件
	const char* path_patten = (*env)->GetStringUTFChars(env, path_patten_jstr, NULL);

	//得到分割之后的子文件的路径表
	char **pathes = malloc(sizeof(char*) * file_num);
	int i = 0;
	for(; i<file_num; i++){
		pathes[i] = malloc(sizeof(char) * 300);
		//元素赋值
		//需要分割的文件 love.png
		//子文件love_1.png
		//写入拆分的文件路径
		sprintf(pathes[i],path_patten,(i+1));
		LOGI("path path:%s", pathes[i]);
	}
	//不断读取path文件，循环写入file_num个文件
	//1、整除
	//文件大小90：分成9个，每个文件10
	//2、不整除
	//文件大小110：分成9个
	//前（9-1）个文件为 (110/(9-1)) = 13
	//最后一个文件(110%(9-1)) = 6

	//总文件大小
	int filesize = get_file_size(path);
	FILE *fpr = fopen(path, "rb");
	//整除
	if(filesize % file_num == 0){
		//单个文件的大小
		int part = filesize / file_num;
		i = 0;
		//逐一写入不同的子文件中
		for(; i<file_num; i++){
			FILE *fpw = fopen(pathes[i], "wb");
			int j = 0;
			for(; j< part; j++){
				//边读边写
				fputc(fgetc(fpr),fpw);
			}
			fclose(fpw);
		}
	}else{//不整除
		int part = filesize / (file_num - 1);
		i = 0;
		//逐一写入不同的子文件中
		for(; i<file_num - 1; i++){
			FILE *fpw = fopen(pathes[i], "wb");
			int j = 0;
			for(; j< part; j++){
				//边读边写
				fputc(fgetc(fpr),fpw);
			}
			fclose(fpw);
		}
		//最后一个文件
		FILE *fpw = fopen(pathes[file_num - 1], "wb");
		i = 0;
		for(; i<filesize % (file_num - 1); i++){
			//边读边写
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpw);
	}
	//关闭被分割的文件
	fclose(fpr);

	//释放手动申请的堆内存空间(数组释放)
	i = 0;
	for(; i < file_num; i++){
		free(pathes[i]);
	}
	free(pathes);

	//释放GetStringUTFChars
	(*env)->ReleaseStringUTFChars(env, path_jstr, path);
	(*env)->ReleaseStringUTFChars(env, path_patten_jstr, path_patten);
}

JNIEXPORT void JNICALL Java_com_lichao_javafilepach_FileUtils_patch
  (JNIEnv *env, jclass jcls, jstring path_patten_jstr, jint file_num, jstring merge_path_jstr){
	//合并之后的文件
	const char* merge_path = (*env)->GetStringUTFChars(env, merge_path_jstr, NULL);
	//分割子文件的patten
	const char* path_patten = (*env)->GetStringUTFChars(env, path_patten_jstr, NULL);

	//得到分割之后的子文件的路径表
	char **pathes = malloc(sizeof(char*) * file_num);
	int i = 0;
	for(; i<file_num; i++){
		pathes[i] = malloc(sizeof(char) * 100);
		//元素赋值
		//需要分割的文件 love.png
		//子文件love_1.png
		sprintf(pathes[i],path_patten,(i+1));
		LOGI("path path:%s", pathes[i]);
	}

	//总文件
	FILE *fpw = fopen(merge_path, "wb");
	//把所有文件读取一遍，写入总文件
	i = 0;
	for(; i<file_num; i++){
		//每个子文件的大小
		int filesize = get_file_size(pathes[i]);
		FILE *fpr = fopen(pathes[i], "rb");
		int j = 0;
		for(; j< filesize; j++){
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpr);
	}
	fclose(fpw);

	//释放手动申请的堆内存空间(数组释放)
	i = 0;
	for(; i < file_num; i++){
		free(pathes[i]);
	}
	free(pathes);

	//释放GetStringUTFChars
	(*env)->ReleaseStringUTFChars(env, merge_path_jstr, merge_path);
	(*env)->ReleaseStringUTFChars(env, path_patten_jstr, path_patten);
}
