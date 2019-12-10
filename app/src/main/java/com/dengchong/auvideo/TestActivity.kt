package com.dengchong.auvideo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.WindowManager
import com.dengchong.core.camera_preview.RecordPreviewCamera
import com.dengchong.core.camera_preview.RecordPreviewScheduler
import kotlinx.android.synthetic.main.activity_test.*
import java.io.File

class TestActivity : AppCompatActivity() {

    private lateinit var previewCamera: RecordPreviewCamera
    private lateinit var previewScheduler: RecordPreviewScheduler
    private var recording: Boolean = false
    private val path =
        Environment.getExternalStorageDirectory().absolutePath + File.separator + "Auvideo.h264"
    private val convetImagePath =
        Environment.getExternalStorageDirectory().absolutePath + File.separator + "Auvideo-convert"

    private val muxPath =
        Environment.getExternalStorageDirectory().absolutePath + File.separator + "Auvideo.mp4"

    private val waterPath =
        Environment.getExternalStorageDirectory().absolutePath + File.separator + "water.jpg"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_test)
        previewCamera = RecordPreviewCamera(this)
        previewScheduler = RecordPreviewScheduler(previewCamera, sv_preview, this)
        btn_recording.setOnClickListener {
            recording = !recording
            if (recording) {
                //width , height 会影响frame.data
                btn_recording.text = "stop"
                previewScheduler.startRecording(muxPath, 360, 640, 700 * 1024, 25, false, waterPath)
            } else {
                btn_recording.text = "start"
                previewScheduler.stopRecording()
            }
        }
        btn_switch.setOnClickListener {
            previewScheduler.switchCamera()
        }
    }
}
