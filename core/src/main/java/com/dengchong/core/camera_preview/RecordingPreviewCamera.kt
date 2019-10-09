package com.dengchong.core.camera_preview

import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.util.Log
import java.lang.RuntimeException

data class CameraInfo(val cameraWidth: Int, val cameraHeight: Int)

interface RecordingPreviewCallback {
    fun onFrameAvailable()
}

private const val TAG = "RecordingPreviewCamera"

class RecordingPreviewCamera {

    var camera: Camera? = null
    var surfaceTexture: SurfaceTexture? = null
    var callback: RecordingPreviewCallback? = null

    fun configCameraFromNative(): CameraInfo {
        camera = Camera.open()
        camera?.apply {
            if (ImageFormat.NV21 in parameters.supportedPreviewFormats) {
                parameters.previewFormat = ImageFormat.NV21
            }
            val size = Size(640, 480)
            if (size in parameters.supportedPreviewSizes) {
                parameters.setPreviewSize(640, 480)
            }
            return CameraInfo(640, 480)
        }
        throw RuntimeException("Camera Open failed")
    }

    fun setPreviewTextureFromNative(textureId: Int) {
        camera ?: return
        surfaceTexture = SurfaceTexture(textureId)
        surfaceTexture?.setOnFrameAvailableListener {
            callback?.onFrameAvailable()
            Log.i("RecordingPreviewCamera", " set onFrameAvailable")
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
            surfaceTexture?.apply {
                // this causes a bunch of warnings that appear harmless but might
                // confuse someone:
                // W BufferQueue: [unnamed-3997-2] cancelBuffer: BufferQueue has
                // been abandoned!
                release()
            }
            camera?.apply {
                release()
            }
        } catch (e: Throwable) {
            Log.e(TAG, Log.getStackTraceString(e))
        }
    }
}