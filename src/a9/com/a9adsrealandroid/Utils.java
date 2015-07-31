package a9.com.a9adsrealandroid;

import android.graphics.PointF;

import java.util.ArrayList;

/**
 * Created by lianchengliang on 7/31/15.
 */
public class Utils {
    public static ArrayList<PointF> arrayToObject(float arr[]){
        ArrayList<PointF> res = new ArrayList<PointF>();
        for(int i = 0; i < arr.length/2; i++){
            res.add(new PointF(arr[i*2], arr[i*2+1]));
        }
        return res;
    }


    public static float[] objectToArray(ArrayList<PointF> arr){
        float []res = new float[arr.size()*2];
        for(int i = 0; i < arr.size(); i++){
            PointF  p = arr.get(i);
            res[i*2] = p.x;
            res[i*2+1] = p.y;
        }
        return res;
    }

    public static PointF arrayToPoint(float []arr){
        return new PointF(arr[0], arr[1]);
    }

    public static float[] pointToArray(PointF pf){
    	float[] arr = new float[2];
    	arr[0] = pf.x;
    	arr[1] = pf.y;
    	return arr;
    }

}
