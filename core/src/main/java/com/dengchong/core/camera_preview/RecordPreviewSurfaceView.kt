package com.dengchong.core.camera_preview

import android.content.Context
import android.util.AttributeSet
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

interface RecordPreviewSurfaceViewCallback {
    fun surfaceCreated(surface: Surface?, width: Int, height: Int)
    fun surfaceChanged(width: Int, height: Int)
    fun surfaceDestroyed()
}

class RecordPreviewSurfaceView(context: Context?, attrs: AttributeSet? = null) :
    SurfaceView(context, attrs), SurfaceHolder.Callback {

    var callback: RecordPreviewSurfaceViewCallback? = null

    init {
        holder?.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        callback?.surfaceCreated(holder?.surface, width, height)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        callback?.surfaceChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        callback?.surfaceDestroyed()
    }
}