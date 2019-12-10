package com.dengchong.core.camera_preview

import android.content.Context
import android.content.Intent
import android.hardware.Camera
import android.net.Uri
import android.os.Environment
import android.view.Surface
import java.io.File

class RecordPreviewScheduler(
    val camera: RecordPreviewCamera,
    val surfaceView: RecordPreviewSurfaceView,
    val context: Context
) : FrameAvailableCallback, RecordPreviewSurfaceViewCallback {

    var defaultCameraId: Int = Camera.CameraInfo.CAMERA_FACING_FRONT

    companion object {
        init {
            System.loadLibrary("core")
//            System.loadLibrary("avfilter")
//            System.loadLibrary("avformat")
//            System.loadLibrary("avcodec")
//            System.loadLibrary("postproc")
//            System.loadLibrary("swresample")
//            System.loadLibrary("swscale")
//            System.loadLibrary("avutil")
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
        camera.releaseCameraFromNative()
    }

    private val dir: String by lazy {
        val dst =
            Environment.getExternalStorageDirectory().absolutePath + "${File.separator}${Environment.DIRECTORY_DCIM}${File.separator}Auvido"
        val dirFile = File(dst)
        if (!dirFile.exists()) {
            dirFile.mkdirs()
        }
        return@lazy dst
    }
    private var curFilePath: String = ""

    fun startRecording(
        width: Int,
        height: Int,
        bitRate: Int,
        frameRate: Int,
        hwEncoding: Boolean,
        waterPath: String = ""
    ) {
        val path = dir + File.separator + "Auvideo.mp4"
        startRecording(path, width, height, bitRate, frameRate, hwEncoding, waterPath)
    }

    fun startRecording(
        path: String,
        width: Int,
        height: Int,
        bitRate: Int,
        frameRate: Int,
        hwEncoding: Boolean,
        waterPath: String = ""
    ) {
        curFilePath = path
        startRecording2Native(path, width, height, bitRate, frameRate, hwEncoding, waterPath)
    }

    external fun startRecording2Native(
        path: String,
        width: Int,
        height: Int,
        bitRate: Int,
        frameRate: Int,
        hwEncoding: Boolean,
        waterPath: String = ""
    )

    fun stopRecording() {
        stopRecording2Native()
//        updateDCIM()
    }

    private external fun stopRecording2Native()

    private fun updateDCIM() {
        if (File(curFilePath).exists()) {
            val intent = Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE)
            intent.data = Uri.fromFile(File(curFilePath))
            context.sendBroadcast(intent)
        }
    }

    external fun switchCamera()
}