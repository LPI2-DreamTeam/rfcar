package com.example.btapp;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.NetworkInfo;
import android.net.wifi.p2p.WifiP2pManager;

public class WifiDirectBroadcastReceiver extends BroadcastReceiver {

    private WifiP2pManager mManager;
    private WifiP2pManager.Channel mChannel;
    private WifiActivity wifiActivity;

    public WifiDirectBroadcastReceiver(WifiP2pManager mManager, WifiP2pManager.Channel mChannel, WifiActivity wifiActivity ){
        this.mManager = mManager;
        this.mChannel = mChannel;
        this.wifiActivity = wifiActivity;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)){
            int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);

            if(state == WifiP2pManager.WIFI_P2P_STATE_ENABLED){
                //WIFI IS ON
                wifiActivity.showToast("Acknowledge: Wifi is enabled", 3, Drawer_Activity.SHORT_TOAST);
            }
            else{
                //WIFI IS OFF
                wifiActivity.showToast("Warning: Wifi is disabled", 4, Drawer_Activity.SHORT_TOAST);
            }
        }
        else if(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)){
            if(mManager!=null){
                mManager.requestPeers(mChannel, wifiActivity.peerListListener);
            }
        }
        else if(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)){
            if(mManager == null){
                return;
            }
            NetworkInfo networkInfo = intent.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);

            if(networkInfo.isConnected()){
                mManager.requestConnectionInfo(mChannel, wifiActivity.connectionInfoListener);
            }
            else{
                wifiActivity.connection_status2.setText(R.string.dev_disc);
            }
        }
        else if(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION.equals(action)){
            //
        }
    }
}
