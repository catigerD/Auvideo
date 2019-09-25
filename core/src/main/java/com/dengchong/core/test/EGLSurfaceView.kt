package com.dengchong.core.test

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

class EGLSurfaceView(
    context: Context?,
    attrs: AttributeSet? = null
) : SurfaceView(context, attrs), SurfaceHolder.Callback {

    companion object {
        init {
            System.loadLibrary("core")
        }
    }

    init {
        holder.addCallback(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        EGLTestMgr.resetRenderSize(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
        EGLTestMgr.destroyEglContext()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        EGLTestMgr.prepareEglContext(holder?.surface, width, height)
    }

}