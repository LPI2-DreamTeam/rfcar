public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final String TAG = "MainActivity";

    private SensorManager sensorManager;
    Sensor rot_sensor;

    TextView x_val, y_val, z_val, inclination, rotation, pitch, roll, azimuth, voltage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pitch = (TextView) findViewById(R.id.pitch);
        roll = (TextView) findViewById(R.id.roll);
        azimuth = (TextView) findViewById(R.id.azimuth);
        voltage = (TextView) findViewById(R.id.voltage_layout);

        Log.d(TAG, "onCreate: Initializing Sensor Services");
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        rot_sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        sensorManager.registerListener(MainActivity.this, rot_sensor, 10000);
    }

        @Override
        public void onAccuracyChanged (Sensor sensor,int i){}

        @Override
        public void onSensorChanged (SensorEvent event){
            Sensor mysensor = event.sensor;

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
                int pitchThreshold = 40;
                int rollInitialVal = 5;
                int pitchInitialVal = 0;
                int max_tension = 6;
                float voltage_ = 0;

                if(roll_ > 0)
                voltage_ = (float) (Math.abs(roll_) * max_tension) / rollThreshold;
                if(voltage_ > 6)
                    voltage_ = 6;
                else if(Math.abs(pitch_) > 70)
                    voltage_ = 0;

                Log.d(TAG, "onSensorChanged: Voltage_: " + String.format("%s", voltage_));

                    pitch.setText(String.format("Pitch: %s", pitch_));
                    roll.setText(String.format("Roll: %s", roll_));
                    azimuth.setText(String.format("Azimuth: %s", azimuth_));
                    linear_vel.setText(String.format("Voltage Applied: %s", voltage_));
            }
        }