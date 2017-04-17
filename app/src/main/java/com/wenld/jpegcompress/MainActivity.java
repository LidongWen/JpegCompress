package com.wenld.jpegcompress;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    private String outFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "test.jpg";
    private String srcFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "compress.jpg";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Bitmap bitmap = BitmapFactory.decodeFile(srcFilePath);
        tv.setText(JpegUtils.compress(bitmap, outFilePath, true));
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
