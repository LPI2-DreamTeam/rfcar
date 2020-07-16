package com.example.btapp;

import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.youtube.player.YouTubeBaseActivity;
import com.google.android.youtube.player.YouTubeInitializationResult;
import com.google.android.youtube.player.YouTubePlayer;
import com.google.android.youtube.player.YouTubePlayerView;

import java.math.BigDecimal;

import static android.content.ContentValues.TAG;
import static com.example.btapp.Drawer_Activity.LONG_TOAST;
import static com.example.btapp.Drawer_Activity.SHORT_TOAST;


public class Video extends YouTubeBaseActivity implements SensorEventListener {

    TextView tilt, speed;
    private SensorManager sensorManager;
    Sensor accelerometer;
    Sensor rot_sensor;
    int rot = 0;
    boolean isFullScreen = false;
    private static final int RECOVERY_REQUEST = 1;
    YouTubePlayerView mYoutubePlayerView;
    YouTubePlayer.OnInitializedListener monInitializedListener;
    ImageButton return_btn2;
    private MyPlayerStateChangeListener playerStateChangeListener;
    private MyPlaybackEventListener playbackEventListener;
    String tilt_s, speed_s;
    private long lastSensorUpdateTime = 0;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.youtube_video);
        return_btn2 = findViewById(R.id.return_btn2);
        return_btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), Drawer_Activity.class);
                startActivity(intent);
            }
        });

        tilt = findViewById(R.id.tilt_ref);
        speed = findViewById(R.id.speed_ref);

        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        rot_sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);

        sensorManager.registerListener(Video.this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(Video.this, rot_sensor, 10000);

        mYoutubePlayerView = findViewById(R.id.Youtube_display);
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

                YouTubePlayer fullScreenPlayer = youTubePlayer;
                fullScreenPlayer.setOnFullscreenListener(new YouTubePlayer.OnFullscreenListener() {
                    @Override
                    public void onFullscreen(boolean b) {
                        isFullScreen = b;
                        showToast("Warning: Low battery",5,LONG_TOAST);
                    }
                });
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

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor mysensor = event.sensor;
        if (mysensor.getType() == Sensor.TYPE_ACCELEROMETER) {

            final float alpha = (float) 0.8;
            float gravityX = 0;
            float gravityY = 0;
            float gravityZ = 0;
            float linear_accelerationX = 0;
            float linear_accelerationY = 0;
            float linear_accelerationZ = 0;

            float[] g = new float[3];
            g = event.values.clone();
            double norm_Of_g = Math.sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2]);

            // Normalize the accelerometer vector
            g[0] = (float) (g[0] / norm_Of_g);
            g[1] = (float) (g[1] / norm_Of_g);
            g[2] = (float) (g[2] / norm_Of_g);

            float incl = ((int) Math.round(Math.toDegrees(Math.acos(g[2]))));
            rot = (int) Math.round(Math.toDegrees(Math.atan2(g[0], g[1])));

            // Isolate the force of gravity with the low-pass filter.
            gravityX = alpha * gravityX + (1 - alpha) * event.values[0];
            gravityY = alpha * gravityY + (1 - alpha) * event.values[1];
            gravityZ = alpha * gravityZ + (1 - alpha) * event.values[2];

            // Remove the gravity contribution with the high-pass filter.
            linear_accelerationX = event.values[0] - gravityX;
            linear_accelerationY = event.values[1] - gravityY;
            linear_accelerationZ = event.values[2] - gravityZ;

            Log.d(TAG, "onSensorChanged: X: " + linear_accelerationX + " Y: " + linear_accelerationY + " Z: " + linear_accelerationZ);

            //x_val.setText(String.format("X: %s", linear_accelerationX));
            //y_val.setText(String.format("Y: %s", linear_accelerationY));
            //z_val.setText(String.format("Z: %s", linear_accelerationZ));
            //inclination.setText(String.format("Inclination: %s", incl));
            //rotation.setText(String.format("Rotation: %s", rot));

            //long currTime = System.currentTimeMillis();

                /*if ((currTime - lastSensorUpdateTime) > 50) {
                    lastSensorUpdateTime = currTime;

                    sensorX = linear_accelerationX;
                    sensorY = linear_accelerationY;
                    sensorZ = linear_accelerationZ;
                }*/
        }

        if (mysensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
            float[] g = event.values.clone();
            boolean isInInitialPos = false;
            boolean thresholdReached = false;

            //Normalise
            double norm = Math.sqrt(g[0] * g[0] + g[1] * g[1] + g[2] * g[2] + g[3] * g[3]);
            g[0] /= norm;
            g[1] /= norm;
            g[2] /= norm;
            g[3] /= norm;

            //Set values to commonly known quaternion letter representatives
            double x = g[0];
            double y = g[1];
            double z = g[2];
            double w = g[3];

            //Calculate Pitch in degrees
            double sinP = 2.0 * (w * x + y * z);
            double cosP = 1.0 - 2.0 * (x * x + y * y);
            int pitch_ = (int) Math.round(Math.atan2(sinP, cosP) * (180 / Math.PI)) + 30;

            int roll_ = 0;
            //Calculate Tilt in degrees
            double sinT = 2.0 * (w * y - z * x);
            if (Math.abs(sinT) >= 1)
                roll_ = (int) (Math.copySign(Math.PI / 2, sinT) * (180 / Math.PI)) + 90;
            else
                roll_ = (int) (Math.asin(sinT) * (180 / Math.PI)) + 90;

            //Calculate Azimuth in degrees (0 to 360; 0 = North, 90 = East, 180 = South, 270 = West)
            double sinA = 2.0 * (w * z + x * y);
            double cosA = 1.0 - 2.0 * (y * y + z * z);
            int azimuth_ = (int) (Math.atan2(sinA, cosA) * (180 / Math.PI));

            int rollThreshold = 60;
            //int pitchThreshold = 40;
            //int rollInitialVal = 5;
            //int pitchInitialVal = 0;
            int max_tension = 100;
            float voltage_ = 0;
            float wheel_tilt = 0;
            int max_wheel_tilt = 45;
            int right_w = 64;
            int left_w = 56;
            int init_tilt = 94;
            int temp_diff = Math.abs(rot - init_tilt);

            if (roll_ > 0)
                voltage_ = (float) (Math.abs(roll_) * max_tension) / rollThreshold;
            if (voltage_ > max_tension)
                voltage_ = max_tension;
            else if (Math.abs(pitch_) > 70)
                voltage_ = 0;

            if (rot > init_tilt) {
                wheel_tilt = (float) (temp_diff * 100) / left_w;
                if (wheel_tilt > 100)
                    wheel_tilt = 100;
            } else if (rot == init_tilt)
                wheel_tilt = 0;
            else {
                wheel_tilt = (float) (temp_diff * 100) / right_w;
                if (wheel_tilt > 100)
                    wheel_tilt = -100;
                else
                    wheel_tilt = -wheel_tilt;

            }
            voltage_ = round(voltage_,1);
            wheel_tilt = round(wheel_tilt,1);

            speed_s = String.format("Speed(%%Max): %s", voltage_);
            tilt_s = String.format("Wheel tilt(%%Max): %s", wheel_tilt);

            speed.setText(String.format("Speed(%%Max): %s", voltage_));
            tilt.setText(String.format("Wheel tilt(%%Max): %s", wheel_tilt));

            if(isFullScreen){
                long currTime = System.currentTimeMillis();

                if((currTime - lastSensorUpdateTime) > 7500){
                    lastSensorUpdateTime = currTime;
                    showToast(speed_s + "\n" + tilt_s,7,LONG_TOAST);
                }
            }

        }
    }


     //Round to certain number of decimals
    public static float round(float d, int decimalPlace) {
        BigDecimal bd = new BigDecimal(Float.toString(d));
        bd = bd.setScale(decimalPlace, BigDecimal.ROUND_HALF_UP);
        return bd.floatValue();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // ignore orientation/keyboard change
        super.onConfigurationChanged(newConfig);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {}

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
            case 7:
                layout = inflater.inflate(R.layout.sensor_toast, (ViewGroup) findViewById(R.id.toast_root));
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
        if (type == 7){
            toast.setGravity(Gravity.TOP, -575, 0);
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
