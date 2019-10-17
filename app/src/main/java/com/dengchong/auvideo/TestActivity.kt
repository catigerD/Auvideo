package com.dengchong.auvideo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import com.dengchong.core.camera_preview.RecordingPreviewCamera
import com.dengchong.core.camera_preview.RecordingPreviewScheduler
import kotlinx.android.synthetic.main.activity_test.*
import java.io.File

class TestActivity : AppCompatActivity() {

    private lateinit var previewCamera: RecordingPreviewCamera
    private lateinit var previewScheduler: RecordingPreviewScheduler
    private var recording: Boolean = false
    private val path =
        Environment.getExternalStorageDirectory().absolutePath + File.separator + "Auvideo.h264"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_test)
        previewCamera = RecordingPreviewCamera(this)
        previewScheduler = RecordingPreviewScheduler(previewCamera, sv_preview)
        btn_recording.setOnClickListener {
            recording = !recording
            if (recording) {
                previewScheduler.startRecording(path, 360, 640, 700 * 1024, 25, false)
            } else {
                previewScheduler.stopRecording()
            }
        }
    }
}
