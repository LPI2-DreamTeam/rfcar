TextView x_val, y_val, z_val;
private float sensorX, sensorY, sensorZ;

...

 Log.d(TAG, "onCreate: Initializing Sensor Services");
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(MainActivity.this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
        Log.d(TAG, "onCreate: Registered accelerometer listener");
		
...

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