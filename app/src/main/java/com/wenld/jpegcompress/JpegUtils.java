package com.wenld.jpegcompress;

import android.graphics.Bitmap;

/**
 * Created by wenld on 2017/4/9.
 */

public class JpegUtils {

    private static int DEFAULT_QUALITY = 20;

    /**
     * @param bitmap
     * @param filePath
     * @param optimize 是否进行压缩
     * @return
     */
    public static String compress(Bitmap bitmap, String filePath, boolean optimize) {
        return compressBitmap(bitmap, bitmap.getWidth(), bitmap.getHeight(), DEFAULT_QUALITY, filePath.getBytes(), optimize);
    }

    static {
        try {
            System.loadLibrary("jpeg");
            System.loadLibrary("compress-jpeg");
        }catch (Exception e){

        }
    }

    /**
     * @param bitmap
     * @param width
     * @param height
     * @param quality
     * @param filePathBytes
     * @param optmize       是否进行压缩
     * @return
     */
    public static native String compressBitmap(Bitmap bitmap, int width, int height, int quality, byte[] filePathBytes, boolean optmize);
}
