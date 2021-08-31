package ru.killthereal.illegal_racer.graphics

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import ru.killthereal.illegal_racer.R
import java.io.File
import java.io.FileNotFoundException
import java.io.FileOutputStream
import java.io.IOException
import java.lang.Exception

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class Renderer(
    private var context: Context
    ) : GLSurfaceView.Renderer {

    val TAG = "Renderer"

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        try {
            val shaderDir = context.getDir("raw", Context.MODE_PRIVATE)
            val vertexShaderFile = File(shaderDir, "vertex.vsh")
            val fragmentShaderFile = File(shaderDir, "fragment_shader.fsh")

            // Vertex
            var aInputStream = context.resources.openRawResource(R.raw.vertex)
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
            aInputStream = context.resources.openRawResource(R.raw.fragment_shader)
            aOutputStream = FileOutputStream(fragmentShaderFile)
            bytesRead = aInputStream.read(buffer)
            while (bytesRead != -1) {
                aOutputStream.write(buffer, 0, bytesRead)
                bytesRead = aInputStream.read(buffer)
            }
            aInputStream.close()
            aOutputStream.close()

            //loadShader(vertexShaderFile.absolutePath, fragmentShaderFile.absolutePath)
        }
        catch (e: FileNotFoundException) {
            Log.e(TAG, "An error occurred: file not found\n" + e.message)
        }
        catch (e: IOException) {
            Log.e(TAG, "An I/O error occurred")
        }
        catch (e: Exception) {
            e.printStackTrace()
        }
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        //setViewport(0, 0, width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        //drawFrame()

        //mShader.use()
        //mTriangle.draw()
    }

}
