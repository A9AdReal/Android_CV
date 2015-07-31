package a9.com.a9adsrealandroid;

import a9.com.a9adsrealandroid.util.SystemUiHider;

import android.app.Activity;
import android.graphics.PointF;
import android.os.Bundle;
import android.util.Log;
import android.hardware.Camera;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.GridLayout;

import java.util.ArrayList;
import java.util.Arrays;


/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 *
 * @see SystemUiHider
 */
public class VideoRegionSelectActivity extends Activity{
    private Camera mCamera;
    private CameraPreview mCameraPreview;
    private RenderView mRenderView;
    private AddPointsView mAddPointsView;
    private Button bRenderRegion;
    private Button bReset;
    private Button bChooseAds;
    private int mWidth;
    private int mHeight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_vedio_region_select);

        mCamera = getCameraInstance();
        mCameraPreview = new CameraPreview(this, mCamera);
        mWidth = mCamera.getParameters().getPreviewSize().width;
        mHeight = mCamera.getParameters().getPreviewSize().height;
        FrameLayout preview = (FrameLayout) findViewById(R.id.cameraLayout);
        preview.addView(mCameraPreview);
        //add render view to the layout
        mRenderView = new RenderView(this.getApplicationContext(), this);
        preview.addView(mRenderView);
        //add point view to the layout
//        mAddPointsView = new AddPointsView(this.getApplicationContext());
//        preview.addView(mAddPointsView);
        //init the button
        bRenderRegion = new Button(this);
        bRenderRegion.setText("Render");
        bRenderRegion.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRenderView.pleaseRender();
            }
        });

        //add a reset button
        bReset = new Button(this);
        bReset.setText("Reset");
        bReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mRenderView.resetRegion();
            }
        });
        //
        GridLayout layoutButton = new GridLayout(this.getApplicationContext());
        layoutButton.setRowCount(1);
        layoutButton.setColumnCount(2);
        layoutButton.addView(bReset);
        layoutButton.addView(bRenderRegion);
        //add layout to view
        FrameLayout.LayoutParams layoutParam = new FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParam.gravity = Gravity.BOTTOM;
        preview.addView(layoutButton, layoutParam);
        init();
    }



    public PointF findCornerOnScreen(PointF curP){
        Log.e("the input point is ", "x: " + curP.x + " y: " + curP.y);
    	float[] res = findCornerOnScreen(Utils.pointToArray(curP), mCameraPreview.getFrameData(), mWidth, mHeight);
        PointF pR =  Utils.arrayToPoint(res);
        Log.e("the output point is x: ", + pR.x + " y:" + pR.y);
        return pR;
    }

    //TODO implement this using JNI
    public native float[] findCornerOnScreen(float[] curP, byte[] preFrame, int width, int height);

    public void trackingPoint(byte[] preFrame, byte[] curFrame){
//        float[] ps = trackingPoint(Utils.objectToArray(mRenderView.getVertices()), preFrame, curFrame, mWidth, mHeight);
//        Log.e("the return value of postions ", "postion: " + Arrays.toString(ps));
//        mRenderView.updateVertices(Utils.arrayToObject(ps));
    }

    //TODO implement this
    public native float[] trackingPoint(float[] curPs, byte[] preFrame, byte[] curFrame, int width, int height);


    //TODO impliment this
    public native void init();

    public static Camera getCameraInstance(){
        Camera c = null;
        try {
            c = Camera.open(); // attempt to get a Camera instance
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return c; // returns null if camera is unavailable
    }
    
     static { System.loadLibrary("detection_based_tracker"); }

}