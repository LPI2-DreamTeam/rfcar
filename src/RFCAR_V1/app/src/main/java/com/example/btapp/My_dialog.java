package com.example.btapp;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatDialogFragment;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Objects;
import java.util.Set;
import java.util.UUID;

import static com.example.btapp.Drawer_Activity.LONG_TOAST;
import static com.example.btapp.Drawer_Activity.SHORT_TOAST;
import static com.example.btapp.Drawer_Activity.chatHandler;

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

 NAME:			    My_dialog
 INHERITS FROM:		AppCompatDialogFragment
 IMPLEMENTS:        AdapterView.OnItemClickListener

 MEMBER VARIABLES:  my_scanned_devices: ArrayList<BluetoothDevice> (none)
                    bt_paired_devices: Set<BluetoothDevice>  (none)
                    btAdapter: BluetoothAdapter (none)
                    bluetoothDevice: BluetoothDevice (none)
                    dev_list: ArrayList<String> (none)
                    arrayAdapter: ArrayAdapter<String> (none)
                    view: View (none)
                    pairedList: ListView (none)
                    newDevList: ListView (none)
                    receiver: BroadcastReceiver (none)
                    MY_UUID: UUID (static final)

 PURPOSE:           defines the fragment used for connection setup
 *********************************************************************************************************/
public class My_dialog extends AppCompatDialogFragment implements AdapterView.OnItemClickListener {

    private ArrayList<BluetoothDevice> my_scanned_devices = new ArrayList<>();
    private Set<BluetoothDevice> bt_paired_devices;
    private BluetoothAdapter btAdapter;
    private BluetoothDevice bluetoothDevice;
    ArrayList<String> dev_list = new ArrayList<>();
    ArrayAdapter<String> arrayAdapter;
    View view;
    ListView pairedList;
    ListView newDevList;
    BroadcastReceiver receiver;
    private static final UUID MY_UUID = UUID.fromString("8ce255c0-200a-11e0-ac64-0800200c9a66");

    /*******************************************************************************************************
     NAME:			    onCreateDialog
     INPUTS:		    savedInstanceState: Bundle

     AUX VARIABLES:	     builder:  AlertDialog.Builder
                        inflater: LayoutInflater
                        intentFilter: IntentFilter
                        btn_dev: Button
                        btn_scan: Button

     OUTPUTS:		    builder.create(): Dialog
     DESCRIPTION:		generic onCreate method for a fragment
     ********************************************************************************************************/
    @SuppressLint("InflateParams")
    @NonNull
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {

        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = Objects.requireNonNull(getActivity()).getLayoutInflater();

        view = inflater.inflate(R.layout.activity_connect_room, null);

        btAdapter = BluetoothAdapter.getDefaultAdapter();

        pairedList = view.findViewById(R.id.paired_devices_list_view);
        newDevList = view.findViewById(R.id.new_devices_list_view);

         receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    BluetoothDevice dev = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    dev_list.add(dev.getName() + " " + dev.getAddress());
                    my_scanned_devices.add(dev);
                    arrayAdapter.notifyDataSetChanged();
                }
            }
        };

        IntentFilter intentFilter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        getActivity().registerReceiver(receiver, intentFilter);
        arrayAdapter = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1, dev_list);
        newDevList.setAdapter(arrayAdapter);

        Button btn_dev = view.findViewById(R.id.btn_scan_dev);
        btn_dev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showToast("Scanning Request Sent",3, LONG_TOAST);
                dev_list.clear();
                arrayAdapter.notifyDataSetChanged();
                btAdapter.startDiscovery();
            }
        });

        Button btn_scan = view.findViewById(R.id.btn_list_dev);
        btn_scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bt_paired_devices = btAdapter.getBondedDevices();
                String[] devices_names = new String[bt_paired_devices.size()];

                        int i = 0;

                        if (bt_paired_devices.size() > 0) {
                            for (BluetoothDevice bluetoothDevice : bt_paired_devices) {
                                devices_names[i] = bluetoothDevice.getName() + " " + bluetoothDevice.getAddress();
                                i++;
                    }
                    ArrayAdapter<String> adapterArr = new ArrayAdapter<String>(getActivity().getApplicationContext(), android.R.layout.simple_list_item_1, devices_names);
                    pairedList.setAdapter(adapterArr);
                }

            }
        });

        /*******************************************************************************************************
         * OnClickListener for ian item in the paired devices list
         *******************************************************************************************************/
        pairedList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String info = ((TextView) view).getText().toString();
                String address = info.substring(info.length() - 17);
                String name = info.replaceAll(address," ");

                    ArrayList<BluetoothDevice> bt_dev_arr = new ArrayList<>();
                    for(BluetoothDevice dev : bt_paired_devices){
                        bt_dev_arr.add(dev);
                    }

                    bluetoothDevice = bt_dev_arr.get(position);

                    connectToDevice(bluetoothDevice.getAddress());
                    showToast("Connect request sent to " + name,3, SHORT_TOAST);
            }

        });

        /*******************************************************************************************************
         * OnClickListener for ian item in the new devices list
         *******************************************************************************************************/
        newDevList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> arg0, View view, int position, long id) {
                btAdapter.cancelDiscovery();
                String info = ((TextView) view).getText().toString();
                String address = info.substring(info.length() - 17);
                String name = info.replaceAll(address," ");

                if(my_scanned_devices.get(position).getBondState() == BluetoothDevice.BOND_BONDED){
                    showToast("Already Paired Device",4, SHORT_TOAST);
                    return;
                }

                boolean result = my_scanned_devices.get(position).createBond();

                if(result)
                    showToast("Pairing Request Sent",3, LONG_TOAST);
                else
                    showToast("Pairing Request Error",4, SHORT_TOAST);

            }
        });

        builder.setView(view)
                /*******************************************************************************************************
                 * Fragment OK button
                 *******************************************************************************************************/
                .setNegativeButton("ok", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                    }
                });

        return  builder.create();
    }

    /*******************************************************************************************************
     NAME:			    onDestroyView
     INPUTS:		    -
     AUX VARIABLES:	    -
     OUTPUTS:		    -
     DESCRIPTION:		generic fragment onDestroy method that is called when the fragment is destroyed
     ********************************************************************************************************/
    @Override
    public void onDestroyView() {
        super.onDestroyView();
        Objects.requireNonNull(getActivity()).unregisterReceiver(receiver);
        Log.d("Tag", "Fragment.onDestroyView() has been called.");
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
                layout = inflater.inflate(R.layout.custom_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            case 2:
                layout = inflater.inflate(R.layout.bt_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            case 3:
                layout = inflater.inflate(R.layout.ok_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            case 4:
                layout = inflater.inflate(R.layout.error_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            case 5:
                layout = inflater.inflate(R.layout.warning_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            case 6:
                layout = inflater.inflate(R.layout.stream_toast, (ViewGroup) view.findViewById(R.id.toast_root));
                break;
            default:
                return;
        }

        TextView toastText = layout.findViewById(R.id.toast_text);
        toastText.setText(text);

        Toast toast = new Toast(Objects.requireNonNull(getActivity()).getApplicationContext());
        if (type == 2){
            toast.setGravity(Gravity.BOTTOM, 0, 50);
        }
        else if (type == 3 || type == 4 || type == 5){
            toast.setGravity(Gravity.BOTTOM, 0, 50);
        }
        else
            toast.setGravity(Gravity.CENTER, 0, 50);

        if(duration == LONG_TOAST)
            toast.setDuration(Toast.LENGTH_LONG);
        if(duration == SHORT_TOAST)
            toast.setDuration(Toast.LENGTH_SHORT);

        toast.setView(layout);

        toast.show();
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {}

    /*******************************************************************************************************
     NAME:			    connectToDevice
     INPUTS:		    deviceAddress: String
     AUX VARIABLES:	    device: BluetoothDevice
     OUTPUTS:		    -
     DESCRIPTION:		connection method between devices
     ********************************************************************************************************/
    private void connectToDevice(String deviceAddress) {
        btAdapter.cancelDiscovery();
        BluetoothDevice device = btAdapter.getRemoteDevice(deviceAddress);
        chatHandler.initializeClient(device, MY_UUID);
    }

}
