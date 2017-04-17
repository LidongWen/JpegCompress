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
    private String outFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "test.jpg";
    private String srcFilePath = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "compress.jpg";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);

        Bitmap bitmap = BitmapFactory.decodeFile(srcFilePath);
        tv.setText(JpegUtils.compress(bitmap, outFilePath, true));
    }

}
