package a9.com.a9adsrealandroid;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Created by lianchengliang on 7/25/15.
 */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private SurfaceHolder mHolder;
    private Camera mCamera;
    private String TAG = this.getClass().getName();
    private VideoRegionSelectActivity mActivity;
    private byte[] mFrameData;
    private static int timeThres = 200; //in milliseconds
    private static long mPreTime;

    public CameraPreview(VideoRegionSelectActivity activity, Camera camera) {
        super(activity.getApplicationContext());
        mCamera = camera;
        mActivity = activity;
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = getHolder();
        mHolder.addCallback(this);
        // deprecated setting, but required on Android versions prior to 3.0
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        if (mCamera == null) {
            return;
        }
        mPreTime = System.currentTimeMillis();
        // The Surface has been created, now tell the camera where to draw the
        // preview.
        try {
            mCamera.setPreviewDisplay(holder);
            mCamera.startPreview();
            mCamera.setPreviewCallback(new Camera.PreviewCallback() {

                @Override
                public void onPreviewFrame(byte[] data, Camera camera) {
                    long curTime = System.currentTimeMillis();
                    // process the image (just an example, you should do this inside an AsyncTask)
//                    Camera.Size previewSize = mCamera.getParameters().getPreviewSize();
//                    ByteArrayOutputStream out = new ByteArrayOutputStream();
//                    YuvImage yuvImage = new YuvImage(data, mCamera.getParameters().getPreviewFormat(), previewSize.width, previewSize.height, null);
//                    yuvImage.compressToJpeg(new Rect(0, 0, previewSize.width, previewSize.height), 100, out);
//                    byte[] imageBytes = out.toByteArray();
//
//                    BitmapFactory.Options options = new BitmapFactory.Options();
//                    options.inSampleSize = 1;
//                    Bitmap image = BitmapFactory.decodeByteArray(imageBytes, 0, imageBytes.length, options);
                    if(curTime - mPreTime > timeThres) {
                        // you now have the image bitmap which you can use to apply your processing ...
                        //TODO add a timer here, to control the updation
                        mFrameData = data;
                        mActivity.trackingPoint(null, data);
                        mPreTime = curTime;
                    }
                }
            });
        } catch (IOException e) {
            Log.d(TAG, "Error setting camera preview: " + e.getMessage());
        }
    }

    public byte[] getFrameData() {
        return mFrameData;
    }


    public void surfaceDestroyed(SurfaceHolder holder) {
        // empty. Take care of releasing the Camera preview in your activity.
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        if (mCamera == null) {
            return;
        }

        mCamera.setDisplayOrientation(90);

        // start preview with new settings
        try {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();
        } catch (Exception e) {
            Log.d(TAG, "Error starting camera preview: " + e.getMessage());
        }
    }
}
