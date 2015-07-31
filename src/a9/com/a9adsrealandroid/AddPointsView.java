package a9.com.a9adsrealandroid;


import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by lianchengliang on 7/26/15.
 */
public class AddPointsView extends View {


    Canvas cv;
    Paint mPaint;
    private ArrayList<PointF> mVertices;
    private static final int MAX_VERT = 4;
    float mWidth;
    float mHeight;

    public void addPoint(PointF p){
        if(mVertices.size() < MAX_VERT){
            mVertices.add(p);
        }
    }


    public AddPointsView(Context context) {
        super(context);
        Init();
    }

    public AddPointsView(Context context, AttributeSet attrs) {
        super(context, attrs);
        Init();
    }


    private void Init(){
        mPaint = new Paint();
        mPaint.setColor(Color.RED);
        mPaint.setStrokeWidth(10);
        mPaint.setStyle(Paint.Style.FILL);
        mPaint.setAntiAlias(true);
        mVertices = new ArrayList<PointF>();
    }



    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        for (PointF p : mVertices){
            canvas.drawCircle(p.x * mWidth, p.y * mHeight, 0.0001f, mPaint);
            Log.e("would like to draw", "x" + p.x*mWidth + "\ty" + p.y*mHeight );
        }

    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if(event.getAction() == MotionEvent.ACTION_UP) {
            float x = event.getX()/mWidth;
            float y = event.getY()/mHeight;
            addPoint(new PointF(x, y));
            invalidate();
//            Log.e("the location on screen ", "x: " + x + "\t y:" + y + "\n");
        }
        return false;
    }


    @Override
    protected void onSizeChanged(int xNew, int yNew, int xOld, int yOld){
        Log.e("the width is :", "" + xNew);
        super.onSizeChanged(xNew, yNew, xOld, yOld);
        mWidth = xNew;
        mHeight = yNew;
    }
}
