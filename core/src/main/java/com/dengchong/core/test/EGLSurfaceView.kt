package com.dengchong.core.test

import android.content.Context
import android.os.Environment
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.dengchong.core.tools.logd
import java.io.File

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
        //应用退回后台会调用 SurfaceView 的 surfaceDestroyed 方法。
        logd("surfaceDestroyed")
        EGLTestMgr.destroyEglContext()
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
        val imagePath = "${Environment.getExternalStorageDirectory()}${File.separator}container.jpg"
        logd("${imagePath}  isExit : ${File(imagePath).exists()}")
        EGLTestMgr.prepareEglContext(holder?.surface, width, height, imagePath)
    }

}