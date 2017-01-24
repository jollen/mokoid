package org.jollen;

import android.media.*;
import android.media.MediaPlayer.OnPreparedListener;
import android.content.Context;
import android.util.Log;
import android.content.res.AssetFileDescriptor;
import java.io.IOException;

public final class MyMediaPlayer extends MediaPlayer {
    private static final String TAG = "MyMediaPlayer";
    private static MyMediaPlayer mp;
    private static Context app;

    public MyMediaPlayer() {
        super();   
    }

    public static MyMediaPlayer create(Context context, int resid) {
        int s = AudioSystem.newAudioSessionId();

	app =context;

        if (mp != null) {
	    return mp;
	}

        return create(context, resid, null, s > 0 ? s : 0);
    }

    public static MyMediaPlayer create(Context context, int resid,
            AudioAttributes audioAttributes, int audioSessionId) {
        try {
            AssetFileDescriptor afd = context.getResources().openRawResourceFd(resid);
            if (afd == null) return null;

            mp = new MyMediaPlayer();

            final AudioAttributes aa = audioAttributes != null ? audioAttributes :
                new AudioAttributes.Builder().build();
            mp.setAudioAttributes(aa);
            mp.setAudioSessionId(audioSessionId);

            mp.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
            afd.close();
            mp.prepareAsync();
            return mp;
        } catch (IOException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        } catch (IllegalArgumentException ex) {
            Log.d(TAG, "create failed:", ex);
           // fall through
        } catch (SecurityException ex) {
            Log.d(TAG, "create failed:", ex);
            // fall through
        }
        return null;
    }

    @Override
    public void start() throws IllegalStateException {
	Log.i(TAG, "start playback...");
        super.start();
    }

}
