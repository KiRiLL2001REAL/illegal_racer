package ru.killthereal.illegal_racer.graphics

import android.opengl.GLES30
import android.util.Log

class ShaderMaker {
    companion object {
        const val UNKNOWN_PROGRAM = -1
        const val UNKNOWN_ATTRIBUTE = -1
        const val TAG = "GLESShaderMaker"
    }

    /**
     * Locator of shader program
     */
    var program = UNKNOWN_PROGRAM

    /**
     * Creating program of shader from source code of vertex and fragment shader
     */
    fun createProgram(vertexSource: String, fragmentSource: String): Boolean {
        if (program != UNKNOWN_PROGRAM) {
            // удаляем программу
            GLES30.glDeleteProgram(program)
            program = UNKNOWN_PROGRAM
        }
        // загружаем вершинный шейдер
        val vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, vertexSource)
        if (vertexShader == UNKNOWN_PROGRAM) {
            return false
        }
        // загружаем фрагментный шейдер
        val pixelShader = loadShader(GLES30.GL_FRAGMENT_SHADER, fragmentSource)
        if (pixelShader == UNKNOWN_PROGRAM) {
            return false
        }
        program = GLES30.glCreateProgram()
        if (program != UNKNOWN_PROGRAM) {
            GLES30.glAttachShader(program, vertexShader)
            checkGlError("glAttachShader: vertex")
            GLES30.glAttachShader(program, pixelShader)
            checkGlError("glAttachShader: pixel")
            return linkProgram()
        }
        return true
    }

    private fun linkProgram(): Boolean {
        if (program == UNKNOWN_PROGRAM) {
            return false
        }
        GLES30.glLinkProgram(program)
        val linkStatus = IntArray(1)
        GLES30.glGetProgramiv(program, GLES30.GL_LINK_STATUS, linkStatus, 0)
        if (linkStatus[0] != GLES30.GL_TRUE) {
            Log.e(Companion.TAG, "Could not link program: ")
            Log.e(Companion.TAG, GLES30.glGetProgramInfoLog(program))
            GLES30.glDeleteProgram(program)
            program = UNKNOWN_PROGRAM
            return false
        }
        return true
    }

    private fun loadShader(shaderType: Int, source: String): Int {
        var shader = GLES30.glCreateShader(shaderType)
        if (shader != UNKNOWN_PROGRAM) {
            GLES30.glShaderSource(shader, source)
            GLES30.glCompileShader(shader)
            val compiled = IntArray(1)
            GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compiled, 0)
            if (compiled[0] == UNKNOWN_PROGRAM) {
                Log.e(Companion.TAG, "Could not compile shader $shaderType:")
                Log.e(Companion.TAG, GLES30.glGetShaderInfoLog(shader))
                GLES30.glDeleteShader(shader)
                shader = UNKNOWN_PROGRAM
            }
        }
        return shader
    }

    private fun checkGlError(op: String) {
        var error: Int
        while (GLES30.glGetError().also { error = it } != GLES30.GL_NO_ERROR) {
            Log.e(Companion.TAG, "$op: glError $error")
            throw RuntimeException("$op: glError $error")
        }
    }
}