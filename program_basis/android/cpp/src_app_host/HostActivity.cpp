#include "cqjnihelper.hh"
#include "cqwindow.h"

static jclass clazz() {
    static jclass clazz = nullptr;
    if (clazz == nullptr) {
        cqJNIFindClass(&clazz, cqJNIGetEnv(), "src/app/host/HostActivity");
    }
    return clazz;
}

static int64_t create_window() {
    static jmethodID methodID = nullptr;

    cqJNIStaticMethod method(clazz(), &methodID, "create_window");
    return method.callInt64();
}

static void show_window(int64_t wid) {
    static jmethodID methodID = nullptr;

    cqJNIStaticMethod method(clazz(), &methodID, "show_window");
    method.push(wid);
    return method.callVoid();
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_installInterfaces
/**/(JNIEnv *, jobject)
{
    _cq_interfaces interfaces;
    interfaces.create_window = create_window;
    interfaces.show_window = show_window;
    _cq_install_interfaces(&interfaces);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowLoad
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_load(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowAppear
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_appear(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowDisappear
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_disappear(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowUnload
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_unload(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowScale
/**/(JNIEnv *, jobject, jlong wid, jfloat scale)
{
    _cq_notify_window_scale(wid, scale);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowOrigin
/**/(JNIEnv *, jobject, jlong wid, jfloat x, jfloat y)
{
    _cq_notify_window_origin(wid, x, y);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowSize
/**/(JNIEnv *, jobject, jlong wid, jfloat width, jfloat height)
{
    _cq_notify_window_size(wid, width, height);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowGLDraw
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_gl_draw(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowUpdate
/**/(JNIEnv *, jobject, jlong wid)
{
    _cq_notify_window_update(wid);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowTouchBegan
/**/(JNIEnv *, jobject, jlong wid, jfloat x, jfloat y)
{
    _cq_notify_window_touch_began(wid, x, y);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowTouchMoved
/**/(JNIEnv *, jobject, jlong wid, jfloat x, jfloat y)
{
    _cq_notify_window_touch_moved(wid, x, y);
}

extern "C" JNIEXPORT void JNICALL Java_src_app_host_HostActivity_notifyWindowTouchEnded
/**/(JNIEnv *, jobject, jlong wid, jfloat x, jfloat y)
{
    _cq_notify_window_touch_ended(wid, x, y);
}