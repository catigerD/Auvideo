package com.dengchong.core.camera_preview

import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.Camera
import java.lang.RuntimeException

data class CameraInfo(val cameraWidth: Int, val cameraHeight: Int)

interface RecordingPreviewCallback {
    fun onFrameAvailable()
}

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
        }
        camera?.setPreviewTexture(surfaceTexture)
    }

    fun updateTexImageFromNative() {
        surfaceTexture?.updateTexImage()
    }

    fun releaseCameraFromNative() {
        surfaceTexture?.apply {
            release()
            surfaceTexture = null
        }
        camera?.apply {
            release()
            camera = null
        }
    }
}