package src.app.application;

import android.app.Application;

import src.library.foundation.L;

public abstract class CQAppDelegate {

    private static Application sApp;

    private static synchronized Application storedApp(char intent, Application app) {
        switch (intent) {
            case '+': return sApp = app;
            case '?': return sApp;
            default : return null;
        }
    }

    static void initApp(Application app) {
        if (app != null) {
            L.i("init app context");
            storedApp('+', app);
        } else {
            L.e("try set null app context");
        }
    }

    void init() {
        L.i("init app delegate %s", L.string(this));
        onCreate();
    }

    protected abstract void onCreate();

    public static Application getApp() {
        Application app = storedApp('?', null);
        if (app == null) {
            L.e("app context didn't set");
        }
        return app;
    }
}
