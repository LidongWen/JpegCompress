#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include "com_wenld_jpegcompress_JpegUtils.h"


extern "C" {
#include "jpeg/jpeglib.h"
}

#include <android/log.h>

#define TAG "fileEncrypt"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

typedef u_int8_t BYTE;

char *jstringToChar(JNIEnv *pEnv, jbyteArray pArray);

int generateJPEG(BYTE *data, jint w, jint h, jint quality, char *path, jboolean optimize);

jstring JNICALL Java_com_wenld_jpegcompress_JpegUtils_compressBitmap
        (JNIEnv *env, jclass jclz, jobject bitmap, jint width, jint height, jint quality,
         jbyteArray filePathBytes, jboolean optimize) {
//    LOGE(w);
//解析bitmap的像素，生成字节数组
    BYTE *pixelsColor;//byte数组
    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixelsColor);
    BYTE *data;     //用来存3原色  r  g  b
    data = (BYTE *) malloc(width * height * 3);       //分配空间
    BYTE *tempData;//缓存数组
    tempData = data;
    int i = 0, j = 0;
    BYTE r, g, b;
    int color;//自然颜色值，有三原色组成
    //循环加入三原色；
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            color = *(int *) pixelsColor;
            r = ((color & 0x00FF0000) >>
                                      16);//与操作获得rgb，参考java Color定义alpha color >>> 24 red (color >> 16) & 0xFF
            g = ((color & 0x0000FF00) >> 8);
            b = color & 0X000000FF;

            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data += 3;
            pixelsColor += 4;   //整数为四个字节。
        }
    }
    AndroidBitmap_unlockPixels(env, bitmap);    //解锁
    //获取输出图片的char指针
    char *filePath = jstringToChar(env, filePathBytes);
    //图片数据交给jpeg库进行哈夫曼重新编码，生成新的压缩图片
    int resultCode=generateJPEG(tempData,width,height,quality,filePath,optimize);

    free(tempData);
    if (resultCode == 0) {
        jstring jstring1=env->NewStringUTF("压缩失败");
        return jstring1;
    }
    return env->NewStringUTF("压缩成功");

}

int generateJPEG(BYTE *data, jint w, jint h, jint quality, char *path, jboolean optimize) {
    int nComponent = 3;

    struct jpeg_compress_struct jcs;
    //error处理
    struct jpeg_error_mgr jem;
    jcs.err = jpeg_std_error(&jem);


    //为JPEG对象分配空间并初始化
    jpeg_create_compress(&jcs);
    //获取文件信息
    FILE *f = fopen(path, "wb");
    if (f == NULL) {
        return 0;
    }
    //指定压缩数据源
    jpeg_stdio_dest(&jcs, f);
    jcs.image_width = w;//image_width->JDIMENSION->typedef unsigned int
    jcs.image_height = h;
    jcs.arith_code = false;
    //input_components为1代表灰度图，在等于3时代表彩色位图图像
    jcs.input_components = nComponent;
    if (nComponent == 1)
        //in_color_space为JCS_GRAYSCALE表示灰度图，在等于JCS_RGB时代表彩色位图图像
        jcs.in_color_space = JCS_GRAYSCALE;
    else
        jcs.in_color_space = JCS_RGB;
    jpeg_set_defaults(&jcs);
    //optimize_coding为TRUE，将会使得压缩图像过程中基于图像数据计算哈弗曼表，由于这个计算会显著消耗空间和时间，默认值被设置为FALSE。
    jcs.optimize_coding = optimize;
    //为压缩设定参数，包括图像大小，颜色空间
    jpeg_set_quality(&jcs, quality, true);
    //开始压缩
    jpeg_start_compress(&jcs, TRUE);
    JSAMPROW row_pointer[1];//JSAMPROW就是一个字符型指针 定义一个变量就等价于=========unsigned char *temp
    int row_stride;
    row_stride = jcs.image_width * nComponent;
    while (jcs.next_scanline < jcs.image_height) {
        row_pointer[0] = &data[jcs.next_scanline * row_stride];
        //写入数据 http://www.cnblogs.com/darkknightzh/p/4973828.html
        jpeg_write_scanlines(&jcs, row_pointer, 1);
    }
    //压缩完毕
    jpeg_finish_compress(&jcs);
    //释放资源
    jpeg_destroy_compress(&jcs);
    fclose(f);
    return 1;
}


JNIEXPORT char *jstringToChar(JNIEnv *pEnv, jbyteArray pArray) {
    char *rtn = NULL;
    jsize alen = pEnv->GetArrayLength(pArray);//byte数组长度
    jbyte *ba = pEnv->GetByteArrayElements(pArray, 0);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    pEnv->ReleaseByteArrayElements(pArray, ba, 0);
    return rtn;
}
