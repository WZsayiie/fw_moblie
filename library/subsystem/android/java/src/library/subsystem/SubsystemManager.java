package src.library.subsystem;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;

import src.library.foundation.ContextFinder;
import src.library.foundation.L;

@SuppressWarnings("WeakerAccess") /* this is a library */
public class SubsystemManager {

    @SuppressLint("StaticFieldLeak")
    private static SubsystemManager sInstance;

    public static SubsystemManager get() {
        if (sInstance == null) {
            sInstance = new SubsystemManager();
        }
        return sInstance;
    }

    private Activity mSubsystemActivity;

    public void startSubsystem() {
        if (mSubsystemActivity != null) {
            L.i("subsystem was already running");
            return;
        }

        Activity rootActivity = ContextFinder.get().findCurrentActivity();
        if (rootActivity == null) {
            L.e("not found current activity context");
            return;
        }

        Intent intent = new Intent(rootActivity, SubsystemActivity.class);
        rootActivity.startActivity(intent);
    }

    //the manager need record subsystem activity's instance,
    //otherwise stopSubsystem() can't work.
    void setSubsystemActivityInstance(Activity activity) {
        mSubsystemActivity = activity;
    }

    public void stopSubsystem() {
        if (mSubsystemActivity == null) {
            L.i("there was no any subsystem");
            return;
        }

        mSubsystemActivity.finish();
        mSubsystemActivity = null;
    }
}
