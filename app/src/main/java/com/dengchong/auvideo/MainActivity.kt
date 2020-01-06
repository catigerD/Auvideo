package com.dengchong.auvideo

import android.Manifest
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.dengchong.core.tools.requestPermission
import com.zhihu.matisse.Matisse
import com.zhihu.matisse.MimeType
import kotlinx.android.synthetic.main.activity_main.*
import com.zhihu.matisse.engine.impl.GlideEngine
import android.content.pm.ActivityInfo
import android.app.Activity
import com.dengchong.core.tools.logd
import com.zhihu.matisse.filter.Filter


private const val REQUEST_CODE_CHOOSE = 1

class MainActivity : AppCompatActivity() {

    private val permissions = arrayListOf<String>().apply {
        add(Manifest.permission.CAMERA)
        add(Manifest.permission.WRITE_EXTERNAL_STORAGE)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestPermission(permissions)
        setContentView(R.layout.activity_main)
        btn_go.setOnClickListener {
            val intent = Intent(this, TestActivity::class.java)
            startActivity(intent)
        }

        btn_import.setOnClickListener {
            Matisse.from(this@MainActivity)
                .choose(MimeType.ofAll())//照片视频全部显示
                .countable(true)//有序选择图片
                .maxSelectable(9)//最大选择数量为9
//                .addFilter(GifSizeFilter(320, 320, 5 * Filter.K * Filter.K))
//                .gridExpectedSize(resources.getDimensionPixelSize(R.dimen.grid_expected_size)) //图片显示表格的大小getResources()
                .restrictOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED)//图像选择和预览活动所需的方向。
                .thumbnailScale(0.85f)//缩放比例
                .imageEngine(GlideEngine())//加载方式
                .showPreview(true) // Default is `true`
                .forResult(REQUEST_CODE_CHOOSE)//请求码
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == REQUEST_CODE_CHOOSE && resultCode == Activity.RESULT_OK) {
            logd(Matisse.obtainResult(data).toString())
        }
    }
}
