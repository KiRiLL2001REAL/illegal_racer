package ru.killthereal.illegal_racer

import android.content.pm.ActivityInfo
import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.view.WindowInsetsController
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import ru.killthereal.illegal_racer.graphics.MyOpenGLSurfaceView

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    private external fun jniCreateRenderer()
    private external fun jniDestroyRenderer()
    private external fun jniIsRendererAlive() : Boolean

    private var glSurfaceSurfaceView: MyOpenGLSurfaceView? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE
        setContentView(R.layout.activity_main)

        glSurfaceSurfaceView = findViewById(R.id.myOpenGLSurfaceView)
        glSurfaceSurfaceView = MyOpenGLSurfaceView(this)
    }

    override fun onResume() {
        super.onResume()
        if (!jniIsRendererAlive())
            jniCreateRenderer()
        glSurfaceSurfaceView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        glSurfaceSurfaceView?.onPause()
        jniDestroyRenderer()
    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                window?.decorView?.windowInsetsController?.systemBarsBehavior =
                    WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
                window?.attributes?.layoutInDisplayCutoutMode =
                    WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES
                window?.decorView?.windowInsetsController?.hide(
                    WindowInsets.Type.statusBars()
                            or WindowInsets.Type.navigationBars()
                            or WindowInsets.Type.displayCutout())
                //TODO solve problem with screen cutout
            } else {
                window?.decorView?.systemUiVisibility = (
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                            or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                            or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                            or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                            or View.SYSTEM_UI_FLAG_FULLSCREEN
                            or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY)
            }
        }
    }

}