package com.dengchong.core.test;

import android.view.Surface;

public class EGLTestMgr {

    public native static void prepareEglContext(Surface surface, int width, int height);

    public native static void resetRenderSize(int width, int height);

    public native static void destroyEglContext();
}
