package ru.killthereal.illegal_racer

import android.content.Context
import android.content.pm.ActivityInfo
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.view.*
import androidx.appcompat.app.AppCompatActivity
import java.io.File
import java.io.FileNotFoundException
import java.io.FileOutputStream
import java.io.IOException
import java.lang.Exception
import android.app.ActivityManager
import android.widget.Toast
import java.lang.RuntimeException

class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    val TAG = "MainActivity"

    private external fun nativeOnStart()
    private external fun nativeOnResume()
    private external fun nativeOnPause()
    private external fun nativeOnStop()
    private external fun nativeSetSurface(surface: Surface?)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE
        setContentView(R.layout.activity_main)

        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val version = configurationInfo.reqGlEsVersion
        if (version < 0x30000) {
            val strVersion = configurationInfo.glEsVersion
            Toast.makeText(this, "Unsupported GLES version. Need 3.0 version at least," +
                    " but your device support only $strVersion", Toast.LENGTH_LONG).show()
            throw RuntimeException("Unsupported GLES version. Need 3.0 version at least, but " +
                    "your device support only $strVersion")
        }

        val surfaceView = findViewById<CustomSurfaceView>(R.id.surfaceView)
        surfaceView.holder.addCallback(this)
    }

    override fun onStart() {
        super.onStart()
        nativeOnStart()
    }

    override fun onResume() {
        super.onResume()
        nativeOnResume()
    }

    override fun onPause() {
        super.onPause()
        nativeOnPause()
    }

    override fun onStop() {
        super.onStop()
        nativeOnStop()
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

    override fun surfaceCreated(holder: SurfaceHolder) {}

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        nativeSetSurface(holder.surface)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeSetSurface(null)
    }

    fun getShaderPaths(vertexName: String, fragmentName: String): ArrayList<String> {
        val result = ArrayList<String>()

        try {
            val shaderDir = getDir("raw", Context.MODE_PRIVATE)
            val vertexShaderFile = File(shaderDir, vertexName)
            val fragmentShaderFile = File(shaderDir, fragmentName)

            // Vertex
            var aInputStream = resources.openRawResource(R.raw.vertex)
            var aOutputStream = FileOutputStream(vertexShaderFile)
            val buffer = ByteArray(4096)
            var bytesRead = aInputStream.read(buffer)
            while (bytesRead != -1) {
                aOutputStream.write(buffer, 0, bytesRead)
                bytesRead = aInputStream.read(buffer)
            }
            aInputStream.close()
            aOutputStream.close()

            // Fragment
            aInputStream = resources.openRawResource(R.raw.fragment_shader)
            aOutputStream = FileOutputStream(fragmentShaderFile)
            bytesRead = aInputStream.read(buffer)
            while (bytesRead != -1) {
                aOutputStream.write(buffer, 0, bytesRead)
                bytesRead = aInputStream.read(buffer)
            }
            aInputStream.close()
            aOutputStream.close()

            result.add(vertexShaderFile.absolutePath)
            result.add(fragmentShaderFile.absolutePath)
        }
        catch (e: FileNotFoundException) {
            Log.e(TAG, "An error occurred in getShaderPaths(): file not found\n" + e.message)
        }
        catch (e: IOException) {
            Log.e(TAG, "An I/O error occurred in getShaderPaths()\n" + e.message)
        }
        catch (e: Exception) {
            e.printStackTrace()
        }

        return result
    }

    fun getAssetFilePath(filename: String): String {
        var path = ""

        try {
            val dir = getDir("assets", Context.MODE_PRIVATE)
            val file = File(dir, filename)

            val aInputStream = assets.open(filename)
            val aOutputStream = FileOutputStream(file)
            val buffer = ByteArray(4096)
            var bytesRead = aInputStream.read(buffer)
            while (bytesRead != -1) {
                aOutputStream.write(buffer, 0, bytesRead)
                bytesRead = aInputStream.read(buffer)
            }
            aInputStream.close()
            aOutputStream.close()

            path = file.absolutePath
        }
        catch (e: FileNotFoundException) {
            Log.e(TAG, "An error occurred in getAssetFilePath(): asset file " +
                    "\"" + filename + "\" not found\n" + e.message)
        }
        catch (e: IOException) {
            Log.e(TAG, "An I/O error occurred in getAssetFilePath()\n" + e.message)
        }
        catch (e: Exception) {
            Log.e(TAG, "An error occurred in getAssetFilePath()\n" + e.message)
        }

        return path
    }
}