package dev.cobalt.engine;

import android.app.Activity;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public final class MainActivity extends Activity implements SurfaceHolder.Callback {
    private SurfaceView surfaceView;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        surfaceView = new SurfaceView(this);
        surfaceView.getHolder().addCallback(this);
        setContentView(surfaceView);
    }

    @Override
    public void onPause() {
        super.onPause();
        nativePause();
    }

    @Override
    public void onResume() {
        super.onResume();
        nativeResume();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        nativeSurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        nativeSurfaceChanged(width, height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        nativeSurfaceDestroyed();
    }

    @Override
    public boolean onTouchEvent(android.view.MotionEvent event) {
        final int action = event.getActionMasked();
        final int index = event.getActionIndex();
        if (action == android.view.MotionEvent.ACTION_DOWN ||
            action == android.view.MotionEvent.ACTION_POINTER_DOWN ||
            action == android.view.MotionEvent.ACTION_MOVE) {
            nativeTouch(event.getPointerId(index), event.getX(index), event.getY(index), true);
        } else if (action == android.view.MotionEvent.ACTION_UP ||
                   action == android.view.MotionEvent.ACTION_POINTER_UP ||
                   action == android.view.MotionEvent.ACTION_CANCEL) {
            nativeTouch(event.getPointerId(index), event.getX(index), event.getY(index), false);
        }
        return true;
    }

    private static native void nativeSurfaceCreated(android.view.Surface surface);
    private static native void nativeSurfaceChanged(int width, int height);
    private static native void nativeSurfaceDestroyed();
    private static native void nativePause();
    private static native void nativeResume();
    private static native void nativeTouch(int pointerId, float x, float y, boolean down);

    static {
        System.loadLibrary("cobalt_android");
    }
}