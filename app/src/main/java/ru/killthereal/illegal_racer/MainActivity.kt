package ru.killthereal.illegal_racer

import android.content.pm.ActivityInfo
import android.opengl.GLSurfaceView
import android.os.Build
import android.os.Bundle
import android.view.View
import android.view.WindowInsets
import android.view.WindowInsetsController
import android.view.WindowManager
import androidx.appcompat.app.AppCompatActivity
import ru.killthereal.illegal_racer.render.Renderer
import java.util.logging.Logger


class MainActivity : AppCompatActivity() {
	companion object {
		val LOG = Logger.getLogger(MainActivity::class.java.name)
	}

	private lateinit var glSurfaceView: GLSurfaceView

	/**
	 * A variable that controls the operation of the GLSurfaceView class in
	 * the pause and wait of the main Activity.
	 */
	private var rendererSet = false

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)

		requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE

		glSurfaceView = GLSurfaceView(this)

		// Request an OpenGL ES 3.0 compatible context.
		glSurfaceView.setEGLContextClientVersion(3)
		// Assign renderer
		glSurfaceView.setRenderer(Renderer())
		rendererSet = true

		setContentView(glSurfaceView)
	}

	override fun onResume() {
		super.onResume()
		if (rendererSet)
			glSurfaceView.onResume()
	}

	override fun onPause() {
		super.onPause()
		if (rendererSet)
			glSurfaceView.onPause()
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
							or WindowInsets.Type.displayCutout()
				)
			} else {
				window?.decorView?.setSystemUiVisibility(
					View.SYSTEM_UI_FLAG_LAYOUT_STABLE
							or View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
							or View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
							or View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
							or View.SYSTEM_UI_FLAG_FULLSCREEN
							or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
				)
			}
		}
	}
}