package com.example.btapp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Configuration;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;

import static com.example.btapp.Drawer_Activity.LONG_TOAST;
import static com.example.btapp.Drawer_Activity.SHORT_TOAST;

public class WifiActivity extends AppCompatActivity{

    private static final String TAG = "WifiActivity";
    ImageButton return_button;
    TextView connection_status2;

    WifiManager wifiManager;
    WifiP2pManager mManager;
    WifiP2pManager.Channel mChannel;

    BroadcastReceiver mReceiver;
    IntentFilter mIntentFilter;

    List<WifiP2pDevice> peers = new ArrayList<WifiP2pDevice>();
    String[] deviceNames;
    WifiP2pDevice[] deviceBuffer;
    ListView wifi_devices_discovered;

    ServerClass serverClass;
    ClientClass clientClass;
    static WifiConnectionManager wifiConnectionManager;
    static boolean can_send_rec_msg = false;

    private Button send_wifi_msg;
    private TextView wifi_read_box;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.wifi_devices_discovered);

        wifi_read_box = (TextView) findViewById(R.id.wifi_msg_read);

        send_wifi_msg = (Button) findViewById(R.id.wifi_msg_btn);
        send_wifi_msg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(can_send_rec_msg) {
                    String msg2send = "Hello World! - Android";
                    wifiConnectionManager.sendWifiMessage(msg2send.getBytes());
                }
                else {
                    showToast("Error: No connection established",4,SHORT_TOAST);
                }
            }
        });

        return_button = (ImageButton) findViewById(R.id.return_btn);
        return_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), Drawer_Activity.class);
                startActivity(intent);
            }
        });

        wifi_devices_discovered = (ListView) findViewById(R.id.wifi_list_view);
        wifi_devices_discovered.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                final WifiP2pDevice device = deviceBuffer[position];
                WifiP2pConfig config = new WifiP2pConfig();
                config.deviceAddress = device.deviceAddress;

                mManager.connect(mChannel, config, new WifiP2pManager.ActionListener() {
                    @Override
                    public void onSuccess() {
                        showToast("Acknowledge: Connected to " + device.deviceName,3,LONG_TOAST);
                        connection_status2.setText(MessageFormat.format("Connected to {0}", device.deviceName));
                    }

                    @Override
                    public void onFailure(int reason) {
                        showToast("Error: Unable to connect to " + device.deviceName,4,LONG_TOAST);
                        connection_status2.setText("Connection Fail");
                    }
                });
            }
        });


        connection_status2 = (TextView) findViewById(R.id.connection_status2);

        wifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
        mManager = (WifiP2pManager) getSystemService(Context.WIFI_P2P_SERVICE);
        mChannel = mManager.initialize(this,getMainLooper(),null);

        mReceiver = new WifiDirectBroadcastReceiver(mManager,mChannel,this);
        mIntentFilter = new IntentFilter();
        mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
        mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
        mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
        mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);

        mManager.discoverPeers(mChannel, new WifiP2pManager.ActionListener() {
            @Override
            public void onSuccess() {
                showToast("Acknowledge: Wifi discovery started",3,LONG_TOAST);
                connection_status2.setText(R.string.disc_start);
            }

            @Override
            public void onFailure(int reason) {
                showToast("Error: Wifi discovery failed, please enable android location",4,LONG_TOAST);
                connection_status2.setText(R.string.disc_fail);
            }
        });

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        // ignore orientation/keyboard change
        super.onConfigurationChanged(newConfig);
    }

    WifiP2pManager.PeerListListener peerListListener = new WifiP2pManager.PeerListListener() {
        @Override
        public void onPeersAvailable(WifiP2pDeviceList peerList) {
            if(!peerList.getDeviceList().equals(peers)){
                peers.clear();
                peers.addAll(peerList.getDeviceList());

                deviceNames = new String[peerList.getDeviceList().size()];
                deviceBuffer = new WifiP2pDevice[peerList.getDeviceList().size()];

                int i = 0;
                for(WifiP2pDevice device : peerList.getDeviceList()){
                    deviceNames[i] = device.deviceName;
                    deviceBuffer[i] = device;
                    i++;
                }

                ArrayAdapter<String> adapter = new ArrayAdapter<String>(getApplicationContext(), android.R.layout.simple_list_item_1,deviceNames);
                wifi_devices_discovered.setAdapter(adapter);
            }

            if(peers.size() == 0){
                showToast("Warning: No devices found", 5, SHORT_TOAST);
            }
        }
    };

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

    class WifiConnectionManager extends Thread{
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

    public class ServerClass extends Thread{
        Socket socket;
        ServerSocket serverSocket;

        @Override
        public void run() {
            try {
                serverSocket = new ServerSocket(8888);
                socket = serverSocket.accept();
                //Communication
                wifiConnectionManager = new WifiConnectionManager(socket);
                wifiConnectionManager.start();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "run: Server Socket creation failed");
            }

        }
    }

    public class ClientClass extends Thread{
        Socket socket;
        String hostAddress;

        public ClientClass(InetAddress hostAddress){
            this.hostAddress = hostAddress.getHostAddress();
            socket = new Socket();

        }

        @Override
        public void run() {
            try {
                socket.connect(new InetSocketAddress(this.hostAddress,8888),500);
                //Communication
                wifiConnectionManager = new WifiConnectionManager(socket);
                wifiConnectionManager.start();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "run: Socket Connection Failed - Client");
            }
        }
    }

    WifiP2pManager.ConnectionInfoListener connectionInfoListener = new WifiP2pManager.ConnectionInfoListener() {
        @Override
        public void onConnectionInfoAvailable(WifiP2pInfo info) {
            final InetAddress groupOwnerAddress = info.groupOwnerAddress;

            if(info.groupFormed && info.isGroupOwner){
                connection_status2.setText(R.string.host);
                serverClass = new ServerClass();
                serverClass.start();
                can_send_rec_msg = true;
            }
            else if(info.groupFormed){
                connection_status2.setText(R.string.client);
                clientClass = new ClientClass(groupOwnerAddress);
                clientClass.start();
                can_send_rec_msg = true;
            }
        }
    };

    @Override
    protected void onStart() {
        super.onStart();

    }

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

}
