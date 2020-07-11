public class ChatHandler {
    private static final String TAG ="log";
    private static final String APP_NAME = "RFCAR App";

    // Unique UUID for this application
    private static final UUID MY_UUID = UUID.fromString("8ce255c0-200a-11e0-ac64-0800200c9a66");

    private final BluetoothAdapter bluetoothAdapter;
    private final Handler handler;
    private AcceptThread  acceptThread;
    private ConnectThread  connectThread;
    private IOThread ioThread;
    private int state;
    private Context mContext;
    private UUID deviceUUID;
    private BluetoothDevice bluetoothDevice;
    byte[] buffer;

    private static final int LISTENING_STATE = 1;
    private static final int CONNECTING_STATE = 2;
    private static final int NO_STATE = 4;
    static final int CONNECTED_STATE = 3;

    //Constructor
    ChatHandler(Context context, Handler handler) {
        mContext = context;
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        state = NO_STATE;
        this.handler = handler;
        start();
    }

    // initiate connection to remote device
    synchronized void initializeClient(BluetoothDevice device, UUID uuid) {
        // Cancel any thread
        if (state == CONNECTING_STATE) {
            if (connectThread != null) {
                connectThread.cancel();
                connectThread = null;
            }
        }

        // Cancel running thread
        if (ioThread != null) {
            ioThread.cancel();
            ioThread = null;
        }

        // Start the thread to connect with the given device
        connectThread = new ConnectThread (device, uuid);
        connectThread.start();
        setState(CONNECTING_STATE);
    }

    synchronized int getState() {
        return state;
    }

    private synchronized void connected(BluetoothSocket socket, BluetoothDevice device) {

        Log.d(TAG, "connected: Starting connected thread");

        //Cancelling all kinds of threads that may be running to start a new one

        // Stopping the thread if it is in connecting state
        if (connectThread != null) {
            connectThread.cancel();
            Log.d(TAG, "connected: Cancelled connectThread");
            connectThread = null;
        }

        // Cancel running thread that refers to the connected state
        if (ioThread != null) {
            ioThread.cancel();
            Log.d(TAG, "connected: Cancelled ioThread");
            ioThread = null;
        }

        //Stopping the scanning thread
        if (acceptThread != null) {
            acceptThread.cancel();
            Log.d(TAG, "connected: Cancelled acceptThread");
            acceptThread = null;
        }

        // Start the thread to manage the connection and perform transmissions
        ioThread = new IOThread(socket);
        ioThread.start();

        Log.d(TAG,"IOthread: ioThread.start() called");

        Message msg = handler.obtainMessage(Drawer_Activity.MESSAGE_DEVICE_OBJECT);
        Bundle bundle = new Bundle();
        bundle.putParcelable(Drawer_Activity.DEVICE_OBJECT, device);
        msg.setData(bundle);
        handler.sendMessage(msg);

        setState(CONNECTED_STATE);
    }

    void write(byte[] out) {
        IOThread r;
        synchronized (this) {
            if (state != CONNECTED_STATE) {
                Log.d(TAG, "IOthread: Write: state != CONNECTED_STATE");
                return;
            }
            r = ioThread;
        }
        r.write(out);
    }

    // runs while scanning for connections
    private class AcceptThread  extends Thread {
        private final BluetoothServerSocket serverSocket;

        //Constructor
        AcceptThread() {
            BluetoothServerSocket bluetoothServerSocket = null;
            try {
                bluetoothServerSocket = bluetoothAdapter.listenUsingInsecureRfcommWithServiceRecord(APP_NAME, MY_UUID);
                Log.d(TAG, "AcceptThread: Setting up the Server using: " + MY_UUID);
            } catch (IOException ex) {
                ex.printStackTrace();
                Log.e(TAG, "AcceptThread: IOException " + ex.getMessage());
            }
            serverSocket = bluetoothServerSocket;
        }

        public void run() {


            Log.d(TAG, "run: AcceptThread Running.");
            BluetoothSocket bluetoothSocket = null;
            while (state != CONNECTED_STATE) {
                try {
                    Log.d(TAG, "Running: RFCOM server socket started...");
                    bluetoothSocket = serverSocket.accept();
                    int d = Log.d(TAG, "AcceptThread: accepted the request");
                } catch (IOException e) {
                    Log.e(TAG, "AcceptThread: IOException" + e.getMessage() );
                    break;
                }

                // Only one thread can execute at a time.
                // sync_object is a reference to an object
                // whose lock associates with the monitor.
                // The code is said to be synchronized on
                // the monitor object
                //synchronized(sync_object)
                //{
                //   // Access shared variables and other
                //   // shared resources
                //}

                // If a connection was accepted
                if (bluetoothSocket != null) {
                    synchronized (ChatHandler.this) {
                        switch (state) {
                            case LISTENING_STATE:
                            case CONNECTING_STATE:
                                // start the connected thread.
                                Log.i(TAG, "Call to connected method");
                                connected(bluetoothSocket, bluetoothSocket.getRemoteDevice());
                                Log.i(TAG, "END initializeClientAcceptThread ");
                                break;
                            case NO_STATE:
                            case CONNECTED_STATE:
                                // Either not ready or already connected. Terminate
                                // new socket.
                                try {
                                    bluetoothSocket.close();
                                } catch (IOException e) {}
                                break;
                        }
                    }
                }
            }
        }

        void cancel() {
            try {
                serverSocket.close();
                Log.i(TAG, "END mAcceptThread ");
            } catch (IOException e) {
                Log.e(TAG, "cancel: Close of AcceptThread ServerSocket failed. " + e.getMessage() );
            }
        }
    }

    // runs while attempting to pair with a device
    private class ConnectThread  extends Thread {
        private BluetoothSocket bluetoothSocket;

        //Constructor
        ConnectThread(BluetoothDevice device, UUID uuid) {
            Log.d(TAG, "ConnectThread: started.");
            bluetoothDevice = device;
            deviceUUID = uuid;
        }

        public void run() {
            setName("PairingThread");
            Log.i(TAG, "Running ConnectThread ");

            BluetoothSocket temp = null;
            try {
                Log.d(TAG, "ConnectThread: Trying to create InsecureRfcommSocket using UUID: "+ MY_UUID );
                temp = bluetoothDevice.createRfcommSocketToServiceRecord(MY_UUID);

            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "ConnectThread: Could not create InsecureRfcommSocket " + e.getMessage());
            }

            bluetoothSocket = temp;

            //Cancel discovery
            bluetoothAdapter.cancelDiscovery();

            // Make a connection to the BluetoothSocket
            try {
                bluetoothSocket.connect();
            } catch (IOException e) {
                try {
                    bluetoothSocket.close();
                    Log.d(TAG, "run: Closed Socket");
                } catch (IOException exception) {
                    Log.e(TAG, "ConnectThread: run: Unable to close connection in socket " + exception.getMessage());
                }

                //connectionFailed
                Log.d(TAG, "run: ConnectThread: Could not connect to UUID: " + MY_UUID );
                Message msg = handler.obtainMessage(Drawer_Activity.MESSAGE_TOAST);
                Bundle bundle = new Bundle();
                bundle.putString("Toast", "Unable to connect to the device");
                msg.setData(bundle);
                handler.sendMessage(msg);
            }

            // Reset the ConnectThread
            synchronized (ChatHandler.this) {
                connectThread = null;
            }

            // Start the connected thread
            connected(bluetoothSocket, bluetoothDevice);
        }

        void cancel() {
            try {
                Log.d(TAG, "cancel: Closing Client Socket");
                bluetoothSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "cancel: close() of mmSocket in Connectthread failed. " + e.getMessage());
            }
        }
    }

    // runs during a connection with a remote device
    private class IOThread extends Thread {
        private final BluetoothSocket bluetoothSocket;
        private final InputStream inputStream;
        private final OutputStream outputStream;

        IOThread(BluetoothSocket socket) {
            Log.d(TAG, "IOThread: Starting.");

            bluetoothSocket = socket;
            InputStream auxIn = null;
            OutputStream auxOut = null;

            try {
                auxIn = socket.getInputStream();
                auxOut = socket.getOutputStream();
            } catch (IOException ex) {
                ex.printStackTrace();
            }
            inputStream = auxIn;
            outputStream = auxOut;
        }

        public void run() {
            buffer = new byte[1024]; // buffer for the stream
            int bytes;

            //Listening to the InputStream
            while (true) {
                try {
                    // Reading from the InputStream

                    bytes = inputStream.read(buffer);

                    String incomingMessage = new String(buffer, 0, bytes);
                    Log.d(TAG, "InputStream: " + incomingMessage);
                    // Send the obtained bytes to the UI Activity
                    handler.obtainMessage(Drawer_Activity.MESSAGE_READ, bytes, -1,
                            buffer).sendToTarget();
                    Log.d(TAG, "InputStream: " + "Msg Received :" + incomingMessage);


                } catch (IOException e) {
                    Log.e(TAG, "write: Error reading Input Stream. " + e.getMessage() );
                    Message msg = handler.obtainMessage(Drawer_Activity.MESSAGE_TOAST);
                    Bundle bundle = new Bundle();
                    bundle.putString("Toast", "Device connection was lost");
                    msg.setData(bundle);
                    handler.sendMessage(msg);

                    // Start the service over to restart listening mode
                    ChatHandler.this.start();
                }
            }
        }

        // writing to OutputStream
        void write(byte[] buffer) {
            try {
                Log.d(TAG, "write: Writing to outputstream: " + Arrays.toString(buffer));
                outputStream.write(buffer);
                Log.d(TAG, "InputStream: " + "Msg Sent :" + Arrays.toString(buffer));
                handler.obtainMessage(Drawer_Activity.MESSAGE_WRITE, -1, -1, buffer).sendToTarget();
            } catch (IOException e) { Log.e(TAG, "write: Error writing to output stream. " + e.getMessage() );}
        }

        void cancel() {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "cancel: Error closing the socket " + e.getMessage() );
            }
        }
    }

    private synchronized void start() {
        // Cancel any thread
        if (connectThread != null) {
            connectThread.cancel();
            connectThread = null;
        }

        // Cancel any running thread
        if (ioThread != null) {
            ioThread.cancel();
            ioThread = null;
        }

        setState(LISTENING_STATE);
        if (acceptThread == null) {
            acceptThread = new AcceptThread ();
            acceptThread.start();
        }
    }

    private synchronized void setState(int state) {
        this.state = state;
    }

    public synchronized void stop() {
        if (connectThread != null) {
            connectThread.cancel();
            connectThread = null;
        }

        if (ioThread != null) {
            ioThread.cancel();
            ioThread = null;
        }

        if (acceptThread != null) {
            acceptThread.cancel();
            acceptThread = null;
        }
        setState(NO_STATE);
    }
}