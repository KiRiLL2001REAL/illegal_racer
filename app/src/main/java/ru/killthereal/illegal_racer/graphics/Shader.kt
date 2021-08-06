package ru.killthereal.illegal_racer.graphics

import android.opengl.GLES30.*
import android.util.Log

class Shader {
    companion object {
        const val UNKNOWN_PROGRAM = -1
        const val UNKNOWN_ATTRIBUTE = -1
        const val TAG = "Shader"
    }

    var program: Int = UNKNOWN_PROGRAM

    /**
     * Delete OpenGL ES shader program
     */
    fun release() = glDeleteProgram(program)

    /**
     * Use loaded shader
     */
    fun use() = glUseProgram(program)

    /**
     * Creating program of shader from source code of vertex and fragment shader
     */
    fun createProgram(vertexSource: String, fragmentSource: String): Boolean {
        if (program != ShaderMaker.UNKNOWN_PROGRAM) {
            // удаляем программу
            glDeleteProgram(program)
            program = ShaderMaker.UNKNOWN_PROGRAM
        }
        // загружаем вершинный шейдер
        val vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource)
        if (vertexShader == ShaderMaker.UNKNOWN_PROGRAM)
            return false

        // загружаем фрагментный шейдер
        val fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource)
        if (fragmentShader == ShaderMaker.UNKNOWN_PROGRAM)
            return false

        program = glCreateProgram()
        if (program != ShaderMaker.UNKNOWN_PROGRAM) {
            glAttachShader(program, vertexShader)
            checkGlError("glAttachShader: vertex")
            glAttachShader(program, fragmentShader)
            checkGlError("glAttachShader: pixel")
            val success = linkProgram()
            if (!success) {
                glDeleteShader(vertexShader)
                glDeleteShader(fragmentShader)
            }
            return success
        }
        return true
    }

    private fun linkProgram(): Boolean {
        if (program == ShaderMaker.UNKNOWN_PROGRAM)
            return false

        glLinkProgram(program)
        val linkStatus = IntArray(1)
        glGetProgramiv(program, GL_LINK_STATUS, linkStatus, 0)
        if (linkStatus[0] != GL_TRUE) {
            Log.e(ShaderMaker.TAG, "Could not link program: ")
            Log.e(ShaderMaker.TAG, glGetProgramInfoLog(program))
            glDeleteProgram(program)
            program = ShaderMaker.UNKNOWN_PROGRAM
            return false
        }
        return true
    }

    private fun loadShader(shaderType: Int, source: String): Int {
        var shader = glCreateShader(shaderType)
        if (shader != ShaderMaker.UNKNOWN_PROGRAM) {
            glShaderSource(shader, source)
            glCompileShader(shader)
            val compiled = IntArray(1)
            glGetShaderiv(shader, GL_COMPILE_STATUS, compiled, 0)
            if (compiled[0] == ShaderMaker.UNKNOWN_PROGRAM) {
                Log.e(ShaderMaker.TAG, "Could not compile shader $shaderType:")
                Log.e(ShaderMaker.TAG, glGetShaderInfoLog(shader))
                glDeleteShader(shader)
                shader = ShaderMaker.UNKNOWN_PROGRAM
            }
        }
        return shader
    }

    private fun checkGlError(op: String) {
        var error: Int
        while (glGetError().also { error = it } != GL_NO_ERROR) {
            Log.e(ShaderMaker.TAG, "$op: glError $error")
            throw RuntimeException("$op: glError $error")
        }
    }
}