package org.libsdl.app;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.InputType;
import android.util.Log;
import android.view.Gravity;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;

public class SDLActivity extends Activity {
    private static final String TAG = "SDL";

    public static native int nativeInit();

    protected static SDLActivity mSingleton;

    public static SDLActivity get() {
        return mSingleton;
    }

    public static void initialize() {
        mSingleton.nativeInit();
    }

    protected SDLSurface mSurface;
    protected View mTextEdit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // So we can call stuff from static callbacks
        mSingleton = this;

        // Fullscreen
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        mSurface = new SDLSurface(getApplication());
        setContentView(mSurface);
    }

    @Override
    protected void onPause() {
        super.onPause();
        SDLActivity.onNativePause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        SDLActivity.onNativeResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        SDLActivity.onNativeQuit();
        mSingleton = null;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        SDLActivity.onNativeWindowFocusChanged(hasFocus);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        int keyCode = event.getKeyCode();
        // Ignore certain special keys so they're handled by Android
        if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN ||
            keyCode == KeyEvent.KEYCODE_VOLUME_UP ||
            keyCode == KeyEvent.KEYCODE_CAMERA ||
            keyCode == KeyEvent.KEYCODE_ZOOM_IN ||
            keyCode == KeyEvent.KEYCODE_ZOOM_OUT) {
            return false;
        }
        return super.dispatchKeyEvent(event);
    }

    // Dummy edit text for software keyboard
    protected class DummyEdit extends View implements View.OnKeyListener {
        String mText = "";

        public DummyEdit(Context context) {
            super(context);
            setFocusableInTouchMode(true);
            setFocusable(true);
            setOnKeyListener(this);
        }

        @Override
        public boolean onKey(View v, int keyCode, KeyEvent event) {
            if (event.getAction() == KeyEvent.ACTION_DOWN) {
                if (event.isPrintingKey()) {
                    mText += (char) event.getUnicodeChar();
                }
                SDLActivity.onNativeKeyDown(keyCode);
                return true;
            } else if (event.getAction() == KeyEvent.ACTION_UP) {
                SDLActivity.onNativeKeyUp(keyCode);
                return true;
            }
            return false;
        }

        @Override
        public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
            outAttrs.inputType = InputType.TYPE_CLASS_TEXT;
            outAttrs.imeOptions = EditorInfo.IME_FLAG_NO_EXTRACT_UI;
            return new BaseInputConnection(this, false) {
                @Override
                public boolean commitText(CharSequence text, int newCursorPosition) {
                    for (int i = 0; i < text.length(); i++) {
                        SDLActivity.onNativeKeyDown(text.charAt(i));
                        SDLActivity.onNativeKeyUp(text.charAt(i));
                    }
                    return true;
                }
            };
        }
    }

    // ─── Native methods ──────────────────────────────────────
    public static native void onNativePause();
    public static native void onNativeResume();
    public static native void onNativeQuit();
    public static native void onNativeWindowFocusChanged(boolean hasFocus);
    public static native void onNativeKeyDown(int keycode);
    public static native void onNativeKeyUp(int keycode);
    public static native void onNativeTouch(int touchDeviceId, int pointerFingerId,
                                             int action, float x, float y, float p);
    public static native void onNativeAccel(float x, float y, float z);
    public static native void onNativeSurfaceCreated();
    public static native void onNativeSurfaceChanged();
    public static native void onNativeSurfaceDestroyed();
}

// ─── SDLSurface ──────────────────────────────────────────────
class SDLSurface extends SurfaceView implements SurfaceHolder.Callback, View.OnKeyListener {
    public SDLSurface(Context context) {
        super(context);
        getHolder().addCallback(this);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
        setOnKeyListener(this);
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        switch (event.getAction()) {
            case KeyEvent.ACTION_DOWN:
                SDLActivity.onNativeKeyDown(keyCode);
                return true;
            case KeyEvent.ACTION_UP:
                SDLActivity.onNativeKeyUp(keyCode);
                return true;
        }
        return false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        final int action = event.getActionMasked();
        for (int i = 0; i < event.getPointerCount(); i++) {
            SDLActivity.onNativeTouch(
                event.getDeviceId(),
                event.getPointerId(i),
                action,
                event.getX(i),
                event.getY(i),
                event.getPressure(i)
            );
        }
        return true;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        SDLActivity.onNativeSurfaceCreated();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        SDLActivity.onNativeSurfaceChanged();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        SDLActivity.onNativeSurfaceDestroyed();
    }
}

// ─── HIDDevice (Bluetooth controller support) ────────────────
class SDLGenericMotionListener_API12 implements View.OnGenericMotionListener {
    @Override
    public boolean onGenericMotion(View v, MotionEvent event) {
        return false;
    }
}
