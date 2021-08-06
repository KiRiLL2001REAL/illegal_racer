package ru.killthereal.illegal_racer.graphics.drawable

import android.opengl.GLES30.*
import java.nio.IntBuffer

class Triangle {

    /**
     * Coordinates XYZ
     */
    var vertices = floatArrayOf(
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.5f, 0.0f, 0.0f
    )
        set(value) {
            for (i in value.indices) {
                if (value[i] < -1.0f)
                    value[i] = -1.0f
                else if (value[i] > 1.0f)
                    value[i] = 1.0f
            }
            field = value
        }

    /**
     * Object of vertex(es) array(s).
     * Contains:
     *      list of vertex attributes,
     *      buffers of indexes,
     *      buffers of vertex data,
     *      and etc.
     * Drawing by function glDrawArrays
     */
    private lateinit var VAO: IntBuffer

    private lateinit var VBO: IntBuffer
}