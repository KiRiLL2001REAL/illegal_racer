package ru.killthereal.illegal_racer.graphics.drawable

import android.opengl.GLES30.*
import android.util.Log

class Triangle {
    companion object {
        const val TAG = "Triangle"
    }

    //private external fun triangleInit(vertices: FloatArray)

    /**
     * Coordinates XYZ
     */
    var vertices = floatArrayOf(
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    )
        set(value) {
            if (value.size < field.size) {
                Log.w(TAG, "Coordinates are not updated. Not enough arguments.")
                return
            }
            for (indx in field.indices) {
                if (value[indx] < -1.0f)
                    value[indx] = -1.0f
                else if (value[indx] > 1.0f)
                    value[indx] = 1.0f
            }
            field = value
        }

    /**
     * Object of vertex(es) array(s).
     * Contains: list of vertex attributes, buffers of indexes, buffers of vertex data, and etc.
     * Drawing by function glDrawArrays.
     */
    private var VAO = 0
    private var VBO = 0

    fun updateVAOandVBO(VAO: Int, VBO: Int) {
        this.VAO = VAO
        this.VBO = VBO
    }

    init {
        //triangleInit(vertices)  // c++ call
    }

    fun draw() {
        glBindVertexArray(VAO)
        glDrawArrays(GL_TRIANGLES, 0, 3)
        glBindVertexArray(0)
    }
}