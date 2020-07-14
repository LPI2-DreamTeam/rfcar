public class Video extends YouTubeBaseActivity{

    private static final int RECOVERY_REQUEST = 1;
    YouTubePlayerView mYoutubePlayerView;
    YouTubePlayer.OnInitializedListener monInitializedListener;
    ImageButton return_btn2;
    private MyPlayerStateChangeListener playerStateChangeListener;
    private MyPlaybackEventListener playbackEventListener;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.youtube_video);
        return_btn2 = (ImageButton) findViewById(R.id.return_btn2);
        return_btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), Drawer_Activity.class);
                startActivity(intent);
            }
        });
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