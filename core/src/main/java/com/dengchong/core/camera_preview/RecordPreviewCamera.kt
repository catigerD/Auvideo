package com.dengchong.core.camera_preview

import android.content.Context
import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.util.Log
import android.view.Surface
import android.view.WindowManager
import com.dengchong.core.tools.logd
import java.lang.RuntimeException

data class CameraInfo(val previewWidth: Int, val previewHeight: Int, val degress: Int)

interface FrameAvailableCallback {
    fun onFrameAvailable()
}

private const val TAG = "RecordPreviewCamera"

class RecordPreviewCamera(val context: Context) {

    var camera: Camera? = null
    var surfaceTexture: SurfaceTexture? = null
    var callback: FrameAvailableCallback? = null
    val PREVIEW_WIDTH: Int = 1280
    val PREVIEW_HEIGHT: Int = 720

    fun configCameraFromNative(cameraId: Int): CameraInfo {
        camera = Camera.open(cameraId)
        camera?.apply {
            if (ImageFormat.NV21 in parameters.supportedPreviewFormats) {
                parameters.previewFormat = ImageFormat.NV21
            }
            val size = Size(PREVIEW_WIDTH, PREVIEW_HEIGHT)
            if (size in parameters.supportedPreviewSizes) {
                parameters.setPreviewSize(PREVIEW_WIDTH, PREVIEW_HEIGHT)
            }
            return CameraInfo(PREVIEW_WIDTH, PREVIEW_HEIGHT, getCameraDisplayOrientation(cameraId))
        }
        throw RuntimeException("Camera Open failed")
    }

    private fun getCameraDisplayOrientation(cameraId: Int): Int {
        val wgr = context.getSystemService(Context.WINDOW_SERVICE) as WindowManager
        val rotation = wgr.defaultDisplay.rotation
        var degress: Int = 0
        when (rotation) {
            Surface.ROTATION_0 -> {
                degress = 0
            }
            Surface.ROTATION_90 -> {
                degress = 90
            }
            Surface.ROTATION_180 -> {
                degress = 180
            }
            Surface.ROTATION_270 -> {
                degress = 270
            }
        }
        val result: Int
        val cameraInfo = Camera.CameraInfo()
        Camera.getCameraInfo(cameraId, cameraInfo)
        if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (cameraInfo.orientation + degress) % 360
        } else {
            result = (cameraInfo.orientation - degress + 360) % 360
        }
        logd("------------degress : ${result}")
        return result
    }

    fun setPreviewTextureFromNative(textureId: Int) {
        camera ?: return
        surfaceTexture = SurfaceTexture(textureId)
        surfaceTexture?.setOnFrameAvailableListener {
            callback?.onFrameAvailable()
            Log.i("RecordPreviewCamera", " set onFrameAvailable")
        }
        camera?.setPreviewTexture(surfaceTexture)
        camera?.startPreview()
    }

    fun updateTexImageFromNative() {
        surfaceTexture?.updateTexImage()
    }

    fun releaseCameraFromNative() {
        try {
            camera?.stopPreview()
            // this causes a bunch of warnings that appear harmless but might
            // confuse someone:
            // W BufferQueue: [unnamed-3997-2] cancelBuffer: BufferQueue has
            // been abandoned!
            surfaceTexture?.release()
            camera?.release()
        } catch (e: Throwable) {
            Log.e(TAG, Log.getStackTraceString(e))
        }
    }
}