package ru.killthereal.illegal_racer.render

import android.opengl.GLSurfaceView
import android.opengl.GLES30.glClearColor
import android.opengl.GLES30.glClear
import android.opengl.GLES30.glViewport
import android.opengl.GLES30.GL_COLOR_BUFFER_BIT

import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class Renderer : GLSurfaceView.Renderer {
	override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
		glClearColor(1.0F, 0.0F, 0.0F, 0.0F)
	}

	override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
		glViewport(0, 0, width, height)
	}

	override fun onDrawFrame(gl: GL10?) {
		glClear(GL_COLOR_BUFFER_BIT)
	}

}
