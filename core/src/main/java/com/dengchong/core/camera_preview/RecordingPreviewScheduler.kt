package com.dengchong.core.camera_preview

import android.hardware.Camera
import android.view.Surface

class RecordingPreviewScheduler(
    val camera: RecordingPreviewCamera,
    val surfaceView: RecordingPreviewSurfaceView
) : RecordingPreviewCallback, RecordingPreviewSurfaceViewCallback {

    var defaultCameraId: Int = Camera.CameraInfo.CAMERA_FACING_BACK

    companion object {
        init {
            System.loadLibrary("core")
        }
    }

    init {
        camera.callback = this
        surfaceView.callback = this
    }

    override fun surfaceCreated(surface: Surface?, width: Int, height: Int) {
        initEGLContext(surface, width, height, defaultCameraId)
    }

    override fun surfaceChanged(width: Int, height: Int) {
        resetSurfaceSize(width, height)
    }

    override fun surfaceDestroyed() {
        destroySurface()
    }

    override fun onFrameAvailable() {
        notifyFrameAvailable()
    }

    private external fun initEGLContext(surface: Surface?, width: Int, height: Int, cameraId: Int)

    private fun configCameraFromNative(cameraId: Int): CameraInfo {
        return camera.configCameraFromNative(cameraId)
    }

    private fun setPreviewTextureFromNative(texId: Int) {
        camera.setPreviewTextureFromNative(texId)
    }

    private external fun notifyFrameAvailable()

    private fun updateTexImageFromNative() {
        camera.updateTexImageFromNative()
    }

    private external fun resetSurfaceSize(width: Int, height: Int)

    private external fun destroySurface()

    private fun releaseCameraFromNative() {
        //native 调用 java方法，java 方法再调用 native 方法 crash
        // signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x730073 JavaVM::DetachCurrentThread()
        //todo
//        camera.releaseCameraFromNative()
    }
}