public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String TAG = "MainActivity";

    private SensorManager sensorManager;
    Sensor accelerometer;

    TextView x_val, y_val, z_val;
    private float sensorX, sensorY, sensorZ;

    private CanvasView canvas;
    private int circleRadius = 30;
    private float circleX, circleY;

    private Timer timer;
    private Handler handler;
    private long lastSensorUpdateTime = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        x_val = (TextView) findViewById(R.id.xValue);
        y_val = (TextView) findViewById(R.id.yValue);
        z_val = (TextView) findViewById(R.id.zValue);

        Log.d(TAG, "onCreate: Initializing Sensor Services");
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(MainActivity.this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        Log.d(TAG, "onCreate: Registered accelerometer listener");

        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);

        int screenWidth = size.x;
        int screenHeight = size.y;

        circleX = (screenWidth >> 1) - circleRadius;
        circleY = (screenHeight >> 1) - circleRadius;

        canvas = new CanvasView(MainActivity.this);
        setContentView(canvas);

        handler = new Handler(){
            public void handleMessage(Message message){
                canvas.invalidate();
            }
        };

        timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {

                if(sensorX > 0)
                    circleX -= 10;
                else
                    circleX += 10;

                if(sensorY > 0)
                    circleY += 10;
                else
                    circleY -= 10;

                handler.sendEmptyMessage(0);
            }
        }, 0, 50);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i){}

    @Override
    public void onSensorChanged(SensorEvent event) {
        Sensor mysensor = event.sensor;
        if(mysensor.getType() == Sensor.TYPE_ACCELEROMETER) {

            final float alpha = (float) 0.8;
            float gravityX = 0;
            float gravityY = 0;
            float gravityZ = 0;
            float linear_accelerationX = 0;
            float linear_accelerationY = 0;
            float linear_accelerationZ = 0;

            // Isolate the force of gravity with the low-pass filter.
            gravityX = alpha * gravityX + (1 - alpha) * event.values[0];
            gravityY = alpha * gravityY + (1 - alpha) * event.values[1];
            gravityZ = alpha * gravityZ + (1 - alpha) * event.values[2];

            // Remove the gravity contribution with the high-pass filter.
            linear_accelerationX = event.values[0] - gravityX;
            linear_accelerationY = event.values[1] - gravityY;
            linear_accelerationZ = event.values[2] - gravityZ;

            Log.d(TAG, "onSensorChanged: X: " + linear_accelerationX + " Y: " + linear_accelerationY + " Z: " + linear_accelerationZ);

            x_val.setText(String.format("X: %s", linear_accelerationX));
            y_val.setText(String.format("Y: %s", linear_accelerationY));
            z_val.setText(String.format("Z: %s", linear_accelerationZ));

            long currTime = System.currentTimeMillis();

            if((currTime - lastSensorUpdateTime) > 50){
                lastSensorUpdateTime = currTime;

                sensorX = linear_accelerationX;
                sensorY = linear_accelerationY;
                sensorZ = linear_accelerationZ;
            }
        }
    }

    //Canvas
    private class CanvasView extends View {
        private Paint pen;
        public CanvasView(Context context){
            super(context);
            setFocusable(true);

            pen = new Paint();
        }

        public void onDraw(Canvas screen){
            pen.setStyle(Paint.Style.FILL_AND_STROKE);
            pen.setAntiAlias(true);
            pen.setTextSize(30f);

            int color = ContextCompat.getColor(MainActivity.this, R.color.Orange);
            pen.setColor(color);

            screen.drawCircle(circleX, circleY, circleRadius, pen);
        }

    }
}