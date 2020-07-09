package com.example.btapp;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.youtube.player.YouTubeBaseActivity;
import com.google.android.youtube.player.YouTubeInitializationResult;
import com.google.android.youtube.player.YouTubePlayer;
import com.google.android.youtube.player.YouTubePlayerView;

import static com.example.btapp.Drawer_Activity.LONG_TOAST;
import static com.example.btapp.Drawer_Activity.SHORT_TOAST;


public class Video extends YouTubeBaseActivity{

    private static final int RECOVERY_REQUEST = 1;
    YouTubePlayerView mYoutubePlayerView;
    YouTubePlayer.OnInitializedListener monInitializedListener;
    //Button btnPlay;
    private MyPlayerStateChangeListener playerStateChangeListener;
    private MyPlaybackEventListener playbackEventListener;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.youtube_video);
        //btnPlay = (Button) findViewById(R.id.start_yt);
        mYoutubePlayerView = (YouTubePlayerView) findViewById(R.id.Youtube_display);
        playerStateChangeListener = new MyPlayerStateChangeListener();
        playbackEventListener = new MyPlaybackEventListener();
        monInitializedListener = new YouTubePlayer.OnInitializedListener() {
            @Override
            public void onInitializationSuccess(YouTubePlayer.Provider provider, YouTubePlayer youTubePlayer, boolean b) {
                Log.d("Video", "onNavigationItemSelected: Done initializing");
                youTubePlayer.setPlayerStateChangeListener(playerStateChangeListener);
                youTubePlayer.setPlaybackEventListener(playbackEventListener);
                String video_url = "y7e-GC6oGhg";
                youTubePlayer.loadVideo(video_url);
            }

            @Override
            public void onInitializationFailure(YouTubePlayer.Provider provider, YouTubeInitializationResult youTubeInitializationResult) {
                Log.d("Video", "onNavigationItemSelected: Fail initializing");
                showToast("Video initialization failed",4,SHORT_TOAST);
            }
        };

/*        btnPlay.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });*/

        mYoutubePlayerView.initialize(YoutubeConfig.getApiKey(), monInitializedListener);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == RECOVERY_REQUEST) {
            // Retry initialization if user performed a recovery action
            getYouTubePlayerProvider().initialize(YoutubeConfig.getApiKey(), monInitializedListener);
        }
    }

    protected YouTubePlayer.Provider getYouTubePlayerProvider() {
        return mYoutubePlayerView;
    }

    private final class MyPlaybackEventListener implements YouTubePlayer.PlaybackEventListener {

        @Override
        public void onPlaying() {
            // Called when playback starts, either due to user action or call to play().
            showToast("Streaming RFCAR camera",6,SHORT_TOAST);
        }

        @Override
        public void onPaused() {
            // Called when playback is paused, either due to user action or call to pause().
            showToast("Stream Paused",6,SHORT_TOAST);
        }

        @Override
        public void onStopped() {
            // Called when playback stops for a reason other than being paused.
        }

        @Override
        public void onBuffering(boolean b) {
            // Called when buffering starts or ends.
        }

        @Override
        public void onSeekTo(int i) {
            // Called when a jump in playback position occurs, either
            // due to user scrubbing or call to seekRelativeMillis() or seekToMillis()
        }
    }

    private final class MyPlayerStateChangeListener implements YouTubePlayer.PlayerStateChangeListener {

        @Override
        public void onLoading() {
            // Called when the player is loading a video
            // At this point, it's not ready to accept commands affecting playback such as play() or pause()
        }

        @Override
        public void onLoaded(String s) {
            // Called when a video is done loading.
            // Playback methods such as play(), pause() or seekToMillis(int) may be called after this callback.
        }

        @Override
        public void onAdStarted() {
            // Called when playback of an advertisement starts.
        }

        @Override
        public void onVideoStarted() {
            // Called when playback of the video starts.
        }

        @Override
        public void onVideoEnded() {
            // Called when the video reaches its end.
        }

        @Override
        public void onError(YouTubePlayer.ErrorReason errorReason) {
            // Called when an error occurs.
        }
    }

    /*******************************************************************************************************
     NAME:			    showToast
     INPUTS:		    text: String, type: int, duration: int
     AUX VARIABLES:	    inflater: LayoutInflater, layout: View, toastText: TextView
     OUTPUTS:		    -
     DESCRIPTION:		generates and shows a custom toast
     ********************************************************************************************************/
    public void showToast(String text, int type, int duration) {
        LayoutInflater inflater = getLayoutInflater();
        View layout;
        switch (type){
            case 1:
                layout = inflater.inflate(R.layout.custom_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            case 2:
                layout = inflater.inflate(R.layout.bt_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            case 3:
                layout = inflater.inflate(R.layout.ok_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            case 4:
                layout = inflater.inflate(R.layout.error_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            case 5:
                layout = inflater.inflate(R.layout.warning_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            case 6:
                layout = inflater.inflate(R.layout.stream_toast, (ViewGroup) findViewById(R.id.toast_root));
                break;
            default:
                return;
        }

        TextView toastText = layout.findViewById(R.id.toast_text);
        toastText.setText(text);

        Toast toast = new Toast(getApplicationContext());
        if (type == 2){
            toast.setGravity(Gravity.BOTTOM, 0, 50);
        }
        else {
            toast.setGravity(Gravity.BOTTOM, 0, 170);
        }

        if(duration == LONG_TOAST)
            toast.setDuration(Toast.LENGTH_LONG);
        if(duration == SHORT_TOAST)
            toast.setDuration(Toast.LENGTH_SHORT);

        toast.setView(layout);

        toast.show();
    }
}
