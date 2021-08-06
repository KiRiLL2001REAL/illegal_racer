package ru.killthereal.illegal_racer.graphics

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet

class MyOpenGLSurfaceView : GLSurfaceView {

    private var renderer: ru.killthereal.illegal_racer.graphics.Renderer? = null

    constructor (context: Context) : super(context) {
        initOpenGLView()
    }

    constructor (context: Context, attrs: AttributeSet) : super(context, attrs) {
        initOpenGLView()
    }

    private fun initOpenGLView() {
        // Request an OpenGL ES 3.0 compatible context.
        setEGLContextClientVersion(3)
        preserveEGLContextOnPause = true

        // Set renderer for drawing stuff
        renderer = Renderer(context)
        setRenderer(renderer)
    }

}