package com.example.btapp;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Base64;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Objects;

//***** TEMPLATE *****/
/*******************************************************************************************************
 NAME:			    FunctionName
 INPUTS:		    variable1: Type1 (Keyword)
 AUX VARIABLES:	    aux1: Type1
 OUTPUTS:		    output1: Type1, output2: Type2
 DESCRIPTION:		short Function description
 ********************************************************************************************************/

/*******************************************************************************************************
 // Class: //

 NAME:			        Drawer_Activity
 INHERITS FROM:		    AppCompatActivity
 IMPLEMENTS:           NavigationView.OnNavigationItemSelectedListener

 MEMBER VARIABLES:   message_l: ArrayList<Msg> (static)
                     send_btn: Button (none)
                     RCVD: int (static final)
                     SENT: int (static final)
                     Rec_view_msg_list: RecyclerView (none)
                     adapter: MsgAdapter (none)
                     typed_msg: String (none)
                     chatbox: EditText (none)
                     chatHandler: ChatHandler (static)
                     btAdapter: BluetoothAdapter (none)
                     newDevList: ListView (none)s
                     LONG_TOAST: int (static final)
                     SHORT_TOAST: int (static final)
                     MESSAGE_READ: int (static final)
                     MESSAGE_WRITE: int (static final)
                     MESSAGE_TOAST: int (static final)
                     MESSAGE_DEVICE_OBJECT: int (static final)
                     TAG: String (static final)
                     DEVICE_OBJECT: String (static final)
                     connectedBTDevice: BluetoothDevice (none)
                     connected_device_name: String (none)
                     chat_handler_created: boolean (none)
                     seed: String (static final)

 PURPOSE:           Chat interface and drawer
 *********************************************************************************************************/
public class Drawer_Activity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {

        public ArrayList<Msg> message_l;//static
        private Button send_btn;
        private ImageButton stream_button;
        private ImageButton control_btn;
        private static final int RCVD = 1;
        private static final int SENT = 2;
        private RecyclerView Rec_view_msg_list;
        public MsgAdapter adapter;//static
        private String typed_msg = "";
        public EditText chatbox;
        public static ChatHandler chatHandler;//static
        private BluetoothAdapter btAdapter;
        ListView newDevList;
        public static final int LONG_TOAST = 1;
        public static final int SHORT_TOAST = 2;
        public static final int MESSAGE_READ = 2;
        public static final int MESSAGE_WRITE = 3;
        public static final int MESSAGE_TOAST = 4;
        public static final int MESSAGE_DEVICE_OBJECT = 5;
        private static final String TAG ="log";
        public static final String DEVICE_OBJECT = "device_name";
        private BluetoothDevice connectedBTDevice;
        private String connected_device_name;
        private boolean chat_handler_created;
        private final static String seed = "C&F)J@NcRfUjXn2r4u7x!A%D*G-KaPdS"; //256-bits key

        private static final String TAG_A = "Drawer_Activity";
        private TextView wifi_read_box;
        private TextView wifi_connection_status;
        private Button send_wifi_msg;
        private WifiConnectionManager wifiConnectionManager;

        WifiManager wifiManager;
        WifiP2pManager mManager;
        WifiP2pManager.Channel mChannel;

        BroadcastReceiver mReceiver;
        IntentFilter mIntentFilter;

        /*******************************************************************************************************
         NAME:			    initMsgList
         INPUTS:		    -
         AUX VARIABLES:	    list:  ArrayList<Msg>
         OUTPUTS:		    list:  ArrayList<Msg>
         DESCRIPTION:		puts in the reclyclerView some initial messages
         ********************************************************************************************************/
        private ArrayList<Msg> initMsgList(){
            ArrayList<Msg> list = new ArrayList<>();
            list.add(new Msg("Welcome to BT Chat", "", "",RCVD));
            return list;
        }

        /*******************************************************************************************************
         * Drawer Activity handler -> ensures the data swap between this activity and the ChatHandler one
         *******************************************************************************************************/
        private Handler handler = new Handler(new Handler.Callback() {

            @Override
            public boolean handleMessage(Message msg) {
                switch (msg.what) {
                    case MESSAGE_WRITE:
                        showToast("Message Sent", 3, SHORT_TOAST);
                        byte[] writeBuf_write = (byte[]) msg.obj;
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.obj = " + msg.obj);
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.obj to string = " + msg.obj.toString());
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.arg1 = " + msg.arg1);
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.arg2 = " + msg.arg2);
                        String writeMessage_write = new String(writeBuf_write);


                        /**************
                         * DECRYPTION *
                         **************/
                        ///////////////////////////////////////////////////////////////////////////////////////////////

                        //String dec_msg = decrypt(writeMessage_write,seed);

                        ///////////////////////////////////////////////////////////////////////////////////////////////

                        int hour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
                        int minutes = Calendar.getInstance().get(Calendar.MINUTE);

                        Log.d(TAG, "Drawer_Activity Switch-case: message write = " + writeMessage_write);
                        //Drawer_Activity.message_l.add(new Msg(writeMessage_write, "", hour + ":" + minutes, SENT));
                        //Drawer_Activity.adapter.notifyDataSetChanged();
                        break;

                    case MESSAGE_READ:
                        showToast("Message Received", 3, SHORT_TOAST);
                        byte[] writeBuf_read = (byte[]) msg.obj;
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.obj = " + msg.obj);
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.obj to string = " + msg.obj.toString());
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.arg1 = " + msg.arg1);
                        Log.d(TAG, "Drawer_Activity Switch-case: msg.arg2 = " + msg.arg2);

                        //msg.arg1 -> message lenght
                        String writeMessage_read = new String(writeBuf_read,0,msg.arg1);

                        /**************
                         * DECRYPTION *
                         **************/
                        ///////////////////////////////////////////////////////////////////////////////////////////////

                        //String dec_msg2 = decrypt(writeMessage_read,seed);

                        ///////////////////////////////////////////////////////////////////////////////////////////////

                        int hour2= Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
                        int minutes2 = Calendar.getInstance().get(Calendar.MINUTE);

                        Log.d(TAG, "Drawer_Activity Switch-case: message read = " + writeMessage_read);
                        //Drawer_Activity.message_l.add(new Msg(writeMessage_read, connected_device_name, hour2 + ":" + minutes2, RCVD));
                        //Drawer_Activity.adapter.notifyDataSetChanged();
                        Rec_view_msg_list.smoothScrollToPosition(adapter.getItemCount());
                        break;

                    case MESSAGE_TOAST:

                        showToast(msg.getData().getString("toast"), 5, SHORT_TOAST);

                        break;

                    case MESSAGE_DEVICE_OBJECT:
                        connectedBTDevice = msg.getData().getParcelable(DEVICE_OBJECT);
                        connected_device_name = connectedBTDevice.getName();
                        showToast("Connected to " + connectedBTDevice.getName(), 3, SHORT_TOAST);
                        break;
                }
                return false;
            }
        });


    /****************
     * WIFI HANDLER *
     ****************/

    static final int MESSAGE_READ_WIFI = 1;

    Handler wifiHandler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what){
                case MESSAGE_READ_WIFI:
                    byte[] readBuffer = (byte[]) msg.obj;
                    String Msg = new String(readBuffer,0,msg.arg1);
                    wifi_read_box.setText(Msg);
                    break;

            }
            return true;
        }
    });

    /***************************
     * WIFI CONNECTION MANAGER *
     ***************************/

    private class WifiConnectionManager extends Thread{
        private Socket wifiSocket;
        private InputStream wifiInputStream;
        private OutputStream wifiOutputStream;

        //Constructor
        public WifiConnectionManager(Socket paramSocket){
            wifiSocket = paramSocket;

            try {
                wifiInputStream = wifiSocket.getInputStream();
                wifiOutputStream = wifiSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "WifiConnectionManager: Couldn't get Input/Output Stream");
            }
        }

        @Override
        public void run() {
            super.run();
            byte[] wifiBuff = new byte[1024];
            int num_bytes_read;

            while(wifiSocket != null){
                try {
                    num_bytes_read = wifiInputStream.read(wifiBuff); //-1 returned if there's no more data

                    if(num_bytes_read>0){ //message valid
                        wifiHandler.obtainMessage(MESSAGE_READ_WIFI,num_bytes_read,-1,wifiBuff).sendToTarget();
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                    Log.e(TAG, "run: Failed Reading InputStream into wifi buffer");
                }
            }
        }

        public void sendWifiMessage(byte[] msgBytes){
            try {
                wifiOutputStream.write(msgBytes);
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "SendWifiMessage: Couldn't send message over Wifi");
            }
        }
    }

        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_drawer_);

            wifi_read_box = (TextView) findViewById(R.id.wifi_msg_read);
            wifi_connection_status = (TextView) findViewById(R.id.connection_status);
            send_wifi_msg = (Button) findViewById(R.id.wifi_msg_btn);
            send_wifi_msg.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    String msg2send = "Hello World! - Android";
                    wifiConnectionManager.sendWifiMessage(msg2send.getBytes());
                }
            });

            wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
            mManager = (WifiP2pManager) getSystemService(Context.WIFI_P2P_SERVICE);
            mChannel = mManager.initialize(this,getMainLooper(),null);

            mReceiver = new WifiDirectBroadcastReceiver(mManager,mChannel,this);
            mIntentFilter = new IntentFilter();
            mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
            mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
            mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
            mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);

            //////

            btAdapter = BluetoothAdapter.getDefaultAdapter();
            newDevList = (ListView) findViewById(R.id.new_devices_list_view);
            Toolbar toolbar = findViewById(R.id.toolbar);
            DrawerLayout drawer = findViewById(R.id.drawer_layout);
            NavigationView navigationView = findViewById(R.id.nav_view);

            setSupportActionBar(toolbar);
            ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
            drawer.addDrawerListener(toggle);
            toggle.syncState();
            navigationView.setNavigationItemSelectedListener(this);
            Objects.requireNonNull(getSupportActionBar()).setDisplayShowTitleEnabled(false);

            int permissionCheck = this.checkSelfPermission("Manifest.permission.ACCESS_FINE_LOCATION") + this.checkSelfPermission("Manifest.permission.ACCESS_COARSE_LOCATION");
            if (permissionCheck != 0) {
                this.requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION, Manifest.permission.ACCESS_COARSE_LOCATION}, 1001); //Any number
            }

            chat_handler_created = false;


            /*****************************************
             * STREAM IMAGE BUTTON ON CLICK LISTENER *
             *****************************************/
            stream_button = findViewById(R.id.stream_btn);
            stream_button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(Drawer_Activity.this, Video.class);
                    startActivity(intent);
                    showToast("RFCAR Control Mode",3,SHORT_TOAST);
                }
            });

            /******************************************
             * CONTROL IMAGE BUTTON ON CLICK LISTENER *
             ******************************************/
            control_btn = findViewById(R.id.control_btn);
            control_btn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent(Drawer_Activity.this, Video.class);
                    startActivity(intent);
                    showToast("RFCAR Control Mode",3,SHORT_TOAST);
                }
            });

        }

        /**************************
         * DRAWER ON BACK PRESSED *
         **************************/
        @Override
        public void onBackPressed() {
            DrawerLayout drawer = findViewById(R.id.drawer_layout);
            if (drawer.isDrawerOpen(GravityCompat.START)) {
                drawer.closeDrawer(GravityCompat.START);
            } else {
                super.onBackPressed();
            }
        }

        /*********************************
         * DRAWER ITEM ON CLICK LISTENER *
         *********************************/
        @Override
        public boolean onNavigationItemSelected(MenuItem item) {
            // Handle navigation view item clicks here.
            int id = item.getItemId();

            /**************************
             * ON ENABLE WIFI PRESSED *
             **************************/
            if(id == R.id.wifi_enable){

                //Closes the drawer
                DrawerLayout drawer = findViewById(R.id.drawer_layout);
                drawer.closeDrawer(GravityCompat.START);

                if(wifiManager.isWifiEnabled()){
                    showToast("Warning: Wifi is already enabled",5,SHORT_TOAST);
                }
                else{
                    wifiManager.setWifiEnabled(true);
                    showToast("Acknowledge: Wifi is now enabled",3,SHORT_TOAST);
                }
            }

            /************************
             * ON VISIBILTY PRESSED *
             ************************/
            if (id == R.id.nav_home) {
                Intent discoverableIntent = new Intent(
                        BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                discoverableIntent.putExtra(
                        BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);

                DrawerLayout drawer = findViewById(R.id.drawer_layout);
                drawer.closeDrawer(GravityCompat.START);

                int request = 0;
                startActivityForResult(discoverableIntent, request);

                IntentFilter intentFilter = new IntentFilter(btAdapter.ACTION_SCAN_MODE_CHANGED);
                registerReceiver(change_bt_state_broadcastReceiver,intentFilter);

            }

            /*******************************
             * ON CONNECTION SETUP PRESSED *
             *******************************/
            else if (id == R.id.nav_scan) {

                //Closes the drawer
                DrawerLayout drawer = findViewById(R.id.drawer_layout);
                drawer.closeDrawer(GravityCompat.START);

                open_scan_devices_dialog();

            }

            /************************
             * ON LISTENING PRESSED *
             ************************/
            else if (id == R.id.nav_paired) {
                chatHandler = new ChatHandler(getApplicationContext(), handler);

                //Closes the drawer
                DrawerLayout drawer = findViewById(R.id.drawer_layout);
                drawer.closeDrawer(GravityCompat.START);

                showToast("Acknowledge: Device is now listening",3,LONG_TOAST);
                chat_handler_created = true;
            }

            return true;
        }

        /*******************************************************************************************************
         NAME:			    sendMessage
         INPUTS:		    message: String
         AUX VARIABLES:	    enc_msg:  String, msg_bytes: bytes[]
         OUTPUTS:		    -
         DESCRIPTION:		if there´s a connection established and the message lenght is greater than
         zero, this method encrypts the message and sends it to the other device
         Otherwise, closes the keyboard and shows a toast to inform the user that
         there was no connection made.
         ********************************************************************************************************/
 /*       void sendMessage(String message) throws Exception {

            if(!chat_handler_created){
                CloseKeyboard();
                showToast("No connection established", 4, SHORT_TOAST);
                return;
            }

            else if (chatHandler.getState() != ChatHandler.CONNECTED_STATE) {
                CloseKeyboard();
                showToast("No connection established", 4, SHORT_TOAST);
                return;
            }

            if (message.length() > 0) {*/

                /**************
                 * ENCRYPTION *
                 **************/
                ///////////////////////////////////////////////////////////////////////////////////////////////

                //String enc_msg = encrypt(message,seed);

                ///////////////////////////////////////////////////////////////////////////////////////////////

               /* byte[] msg_bytes = enc_msg.getBytes(Charset.defaultCharset());

                Log.d(TAG, "Drawer_Activity: message = " + message);
                chatHandler.write(msg_bytes);
                Log.d(TAG, "Drawer_Activity: encrypted message sent = " + Arrays.toString(msg_bytes));
            }
        }*/

        /*******************************************************************************************************
         NAME:			    open_scan_devices_dialog
         INPUTS:		    -
         AUX VARIABLES:	    my_dialog:  My_dialog
         OUTPUTS:		    -
         DESCRIPTION:		creates and shows a custom fragment from the My_dialog class
         ********************************************************************************************************/
        public void open_scan_devices_dialog(){
            My_dialog my_dialog = new My_dialog();
            my_dialog.show(getSupportFragmentManager(), "New Devices Dialog");
        }

        /*******************************************************************************************************
         NAME:			    encrypt
         INPUTS:		    s: String, key: String
         AUX VARIABLES:	    -
         OUTPUTS:		    (implicit): String
         DESCRIPTION:		encrypts a message
         ********************************************************************************************************/
        public String encrypt(String s, String key) {
            return base64Encode(xorWithKey(s.getBytes(), key.getBytes()));
        }

        /*******************************************************************************************************
         NAME:			    decrypt
         INPUTS:		    s: String, key: String
         AUX VARIABLES:	    -
         OUTPUTS:		    (implicit): String
         DESCRIPTION:		decrypts a message
         ********************************************************************************************************/
        public String decrypt(String s, String key) {
            return new String(xorWithKey(base64Decode(s), key.getBytes()));
        }

        /*******************************************************************************************************
         NAME:			    xorWithKey
         INPUTS:		    message: byte[], key: byte[]
         AUX VARIABLES:	    out: byte[], i: int
         OUTPUTS:		    out: byte[]
         DESCRIPTION:		performs a xor using the message and the key
         ********************************************************************************************************/
        private byte[] xorWithKey(byte[] message, byte[] key) {
            byte[] out = new byte[message.length];
            for (int i = 0; i < message.length; i++) {
                out[i] = (byte) (message[i] ^ key[i%key.length]);
            }
            return out;
        }

        /*******************************************************************************************************
         NAME:			    base64Decode
         INPUTS:		    s: String
         AUX VARIABLES:	    -
         OUTPUTS:		    (implicit): byte[]
         DESCRIPTION:		decodes a message
         ********************************************************************************************************/
        private byte[] base64Decode(String s) {
            try { return Base64.decode(s, Base64.DEFAULT);
            } catch (IllegalArgumentException e) {
                throw new RuntimeException(e);
            }
        }

        /*******************************************************************************************************
         NAME:			    base64Encode
         INPUTS:		    bytes: byte[]
         AUX VARIABLES:	    -
         OUTPUTS:		    (implicit): String
         DESCRIPTION:		encodes a message
         ********************************************************************************************************/
        private String base64Encode(byte[] bytes) {
            return Base64.encodeToString(bytes,Base64.DEFAULT).replaceAll("\\s", "");
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

        /******************************
         * VISIBILITY PRESSED RESULTS *
         ******************************/
        @Override
        protected void onActivityResult(int code, int result, Intent intent)
        {

            if(btAdapter.getScanMode()==BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE)
                showToast("Device visible for 5 minutes", 3, SHORT_TOAST);

            if (result == Activity.RESULT_CANCELED)
                showToast("Warning: Device may be invisible", 5, SHORT_TOAST);

        }

        /**************************************
         * BLUETOOTH STATE BROADCAST RECEIVER *
         **************************************/
        private final BroadcastReceiver change_bt_state_broadcastReceiver = new BroadcastReceiver() {

            @Override
            public void onReceive(Context context, Intent intent) {
                final String action = intent.getAction();

                assert action != null;
                if (action.equals(BluetoothAdapter.ACTION_SCAN_MODE_CHANGED)) {

                    int mode = intent.getIntExtra(BluetoothAdapter.EXTRA_SCAN_MODE, BluetoothAdapter.ERROR);

                    switch (mode) {
                        //Device is in Discoverable Mode
                        case BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE:
                            Log.d(TAG, "change_bt_state_broadcastReceiver: Discoverability Enabled.");
                            break;
                        //Device not in discoverable mode
                        case BluetoothAdapter.SCAN_MODE_CONNECTABLE:
                            Log.d(TAG, "change_bt_state_broadcastReceiver: Discoverability Disabled. Able to receive connections.");
                            break;
                        case BluetoothAdapter.SCAN_MODE_NONE:
                            Log.d(TAG, "change_bt_state_broadcastReceiver: Discoverability Disabled. Not able to receive connections.");
                            break;
                        case BluetoothAdapter.STATE_CONNECTING:
                            Log.d(TAG, "change_bt_state_broadcastReceiver: Connecting....");
                            break;
                        case BluetoothAdapter.STATE_CONNECTED:
                            Log.d(TAG, "change_bt_state_broadcastReceiver: Connected.");
                            break;
                    }

                }
            }
        };

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mReceiver, mIntentFilter);
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mReceiver);
    }

        /**************************************
         * GENERIC ACTIVITY ON DESTROY METHOD *
         **************************************/
        @Override
        protected void onDestroy()
        {
            Log.d(TAG, "onDestroy: called.");
            //unregisterReceiver(change_bt_state_broadcastReceiver);
            super.onDestroy();
        }
    }
