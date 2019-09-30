package com.dengchong.auvideo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.dengchong.core.camera_preview.RecordingPreviewCamera
import com.dengchong.core.camera_preview.RecordingPreviewScheduler
import com.dengchong.core.tools.requestPermission
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private lateinit var previewCamera: RecordingPreviewCamera
    private lateinit var previewScheduler: RecordingPreviewScheduler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestPermission()
        setContentView(R.layout.activity_main)
        previewCamera = RecordingPreviewCamera()
        previewScheduler = RecordingPreviewScheduler(previewCamera, sv_preview)
    }
}
