package com.example.btapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

//***** TEMPLATE *****/
/*******************************************************************************************************
 NAME:			    FunctionName
 INPUTS:		    variable1: Type1 (Keyword), function1: (TypeofInput1, TypeofInput2) -> returnType
 AUX VARIABLES:	    aux1: Type1
 OUTPUTS:		    output1: Type1, output2: Type2
 DESCRIPTION:		short Function description
 ********************************************************************************************************/

/*******************************************************************************************************
 // Class: //

 NAME:			     MainActivity
 INHERITS FROM:		 AppCompatActivity
 IMPLEMENTS:         -

 MEMBER VARIABLES:   loginButton: Button (none)
                     DeviceBTAdapter: BluetoothAdapter  (none)
                     REQUEST_ENABLE_BLUETOOTH: int (static final)
                     LONG_TOAST: int (static final)
                     SHORT_TOAST: int (static final)

 PURPOSE:           defines the initial login screen of the application
 *********************************************************************************************************/
public class MainActivity extends AppCompatActivity {

    private Button enterButton;
    private BluetoothAdapter DeviceBTAdapter;
    private static final int REQUEST_ENABLE_BLUETOOTH = 1;
    public static final int LONG_TOAST = 1;
    public static final int SHORT_TOAST = 2;

    @Override  //overwrites the his super class method
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.init);

        //Initializes DeviceBTAdapter
        DeviceBTAdapter = BluetoothAdapter.getDefaultAdapter();

        int permissionCheck = this.checkSelfPermission("Manifest.permission.ACCESS_FINE_LOCATION") + this.checkSelfPermission("Manifest.permission.ACCESS_COARSE_LOCATION");
        if (permissionCheck != 0) {
            this.requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1001); //Any number
        }

        enterButton = findViewById(R.id.enter_button);
        enterButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                enter();
            }
        });

        // checks whether Bluetooth is supported on the device or not.
        if (DeviceBTAdapter == null) {
            //A toast is a view containing a quick little message for the user.
            //When the view is shown to the user, appears as a floating view over the application.
            Toast.makeText(this, "Bluetooth is not available in your device!", Toast.LENGTH_SHORT).show();
            finish();
        }
    }

    /*******************************************************************************************************
     NAME:			    enter
     INPUTS:		    -
     AUX VARIABLES:	    intent: Intent
     OUTPUTS:		    -
     DESCRIPTION:		Defines what happens when you press the enter button
     ********************************************************************************************************/
    private void enter() {
       Intent intent = new Intent(this, Drawer_Activity.class);
       startActivity(intent);
       //showToast("Welcome to the RFCAR App",1,SHORT_TOAST);
    }

    /*********************************************************************************************************
     NAME:			    showToast
     INPUTS:		    text: String, type: int, duration: int
     AUX VARIABLES:	    inflater: LayoutInflater, layout: View, toastText: TextView
     OUTPUTS:		    -
     DESCRIPTION:		generates and shows a custom toast (diferent from other .java due to the toast offset)
     *********************************************************************************************************/
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

    /**************************************
     * GENERIC ACTIVITY ON START METHOD *
     **************************************/
    @Override //overwrites the his super class method
    protected void onStart() {
        super.onStart();

        //checks if your device has enabled Bluetooth.
        if (!DeviceBTAdapter.isEnabled()) {

            //If not, requests to turn it on.
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BLUETOOTH);
        }
        if (DeviceBTAdapter.isEnabled())
            showToast("Bluetooth Enabled",2,SHORT_TOAST);
    }
}