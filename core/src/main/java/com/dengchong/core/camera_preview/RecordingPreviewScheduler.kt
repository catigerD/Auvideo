package com.dengchong.core.camera_preview

import android.view.Surface

class RecordingPreviewScheduler(
    val camera: RecordingPreviewCamera,
    val surfaceView: RecordingPreviewSurfaceView
) : RecordingPreviewCallback, RecordingPreviewSurfaceViewCallback {

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
        initEGLContext(surface, width, height)
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

    private external fun initEGLContext(surface: Surface?, width: Int, height: Int)

    private fun configCameraFromNative(): CameraInfo {
        return camera.configCameraFromNative()
    }

    private fun setPreviewTextureFromNative(texId: Int) {
        camera.setPreviewTextureFromNative(texId)
    }

    private external fun notifyFrameAvailable()

    private fun updateTexImageFromNative() {
        camera.updateTexImageFromNative()
    }

    private external fun resetSurfaceSize(width: Int, height: Int)

    private external fun destroySurface();
}