package com.dengchong.core.camera_preview;

import android.view.Surface;

public class RecordingPreviewSchedule {

    native void initEGLContext(Surface surface, int width, int height);

    native void notifyFrameAvailable();

    native void resetSurfaceSize(int width, int height);

    native void destroySurface();

    native void startRecording(String filePath, int width, int height, int bitRate, int frameRate, boolean hwEncoding);

    native void stopRecording();
}
