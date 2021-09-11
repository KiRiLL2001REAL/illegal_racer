package ru.killthereal.illegal_racer

import android.content.Context
import android.renderscript.Float2
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.SurfaceView

class CustomSurfaceView(context: Context?, attrs: AttributeSet?) : SurfaceView(context, attrs) {
    constructor(context: Context?) : this(context, null)

    external fun registerActionDown(id: Int, x: Float, y: Float)
    external fun registerActionUp(id: Int, x: Float, y: Float)
    external fun registerActionMove(id: Int, dx: Float, dy: Float)

    override fun performClick(): Boolean {
        super.performClick()
        return true
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        super.onTouchEvent(event)
        performClick()

        val count = event!!.pointerCount
        val eventAction = event.action

        var id: Int = 0;
        var x: Float = 0f
        var y: Float = 0f
        val xPrecision = event.xPrecision
        val yPrecision = event.yPrecision
        var index = 0
        while (index < count) {
            id = event.getPointerId(index)
            x = event.getX(index) * xPrecision
            y = event.getY(index) * yPrecision
            when (eventAction) {
                // палец поставлен
                MotionEvent.ACTION_DOWN,
                MotionEvent.ACTION_POINTER_DOWN -> {
                    registerActionDown(id, x, y)
                }
                // палец убран
                MotionEvent.ACTION_UP,
                MotionEvent.ACTION_POINTER_UP,
                MotionEvent.ACTION_CANCEL -> {
                    registerActionUp(id, x, y)
                }
                // палец движется по экрану
                MotionEvent.ACTION_MOVE -> {
                    registerActionMove(id, x, y)
                }
            }
            index++
        }

        return true
    }
}