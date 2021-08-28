package ru.killthereal.illegal_racer.graphics

import android.content.Context
import android.opengl.GLES30.*
import android.opengl.GLSurfaceView
import android.widget.Toast
import ru.killthereal.illegal_racer.R
import ru.killthereal.illegal_racer.graphics.drawable.Triangle

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class RenderWrapper(
    private var context: Context
    ) : GLSurfaceView.Renderer {

    private lateinit var shader: Shader
    private lateinit var triangle: Triangle

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f)

        val vsh = context.resources.openRawResource(R.raw.vertex).bufferedReader()
            .use { it.readText() }
        val fsh = context.resources.openRawResource(R.raw.fragment_shader).bufferedReader()
            .use { it.readText() }

        shader = Shader()
        if (!shader.createProgram(vsh, fsh))
            Toast.makeText(context, "Failed to load shader", Toast.LENGTH_SHORT).show()
        shader.release()

        triangle = Triangle()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        glViewport(0, 0, width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        glClear(GL_COLOR_BUFFER_BIT)

        shader.use()
        triangle.draw()
    }

}
