package com.example.btapp;

import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.ArrayList;

//***** TEMPLATE *****/
/*******************************************************************************************************
 NAME:			    FunctionName
 INPUTS:		    variable1: Type1 (Keyword)
 AUX VARIABLES:	    aux1: Type1
 OUTPUTS:		    output1: Type1, output2: Type2
 DESCRIPTION:		short Function description
********************************************************************************************************/

class Msg {
    String text;
    String bt_nickname;
    String time;
    int type;

    Msg(String txt, String nick, String time, int type){
        this.text = txt;
        this.bt_nickname = nick;
        this.time = time;
        this.type = type;
    }
}

/*******************************************************************************************************
 // Class: //

 NAME:			    MsgAdapter
 INHERITS FROM:		RecyclerView.Adapter<RecyclerView.ViewHolder>
 IMPLEMENTS:        -

 MEMBER VARIABLES:  RCVD: int (final)
                    SENT: int (final)
                    msg_list: ArrayList<Msg>

 PURPOSE:           giving the message added to the RecyclerView a layout
*********************************************************************************************************/
public class MsgAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private static final int RCVD = 1;
    private static final int SENT = 2;
    private ArrayList<Msg> msg_list;

    /*******************************************************************************************************
     * @param msg_list Initialization list for the RecyclerView
     *******************************************************************************************************/
    //Constructor
    MsgAdapter(ArrayList<Msg> msg_list) {
        this.msg_list = msg_list;
    }

   /*********************************************************************************************************
     // Class: //

     NAME:			    MsgAdapter
     INHERITS FROM:		RecyclerView.Adapter<RecyclerView.ViewHolder>

     VARIABLES:         view: View (final)
                        text: TextView (final)
                        user: TextView (final)
                        time: TextView (final)
   **********************************************************************************************************/
    public static class ViewHolder extends RecyclerView.ViewHolder{

        final View view;
        final TextView text;
        final TextView user;
        final TextView time;

        ViewHolder(View view){
            super(view);
            this.view = view;
            text = view.findViewById(R.id.text_message_body);
            user = view.findViewById(R.id.text_message_name);
            time = view.findViewById(R.id.text_message_time);
        }

    }

    /*******************************************************************************************************
     NAME:			    onCreateViewHolder
     INPUTS:		    parent: ViewGroup (none), viewType: int (none)
     AUX VARIABLES:	    v: View
     OUTPUTS:		    (implicit): ViewHolder
     DESCRIPTION:		inflates a message layout based on its type and returns a new ViewHolder
     ********************************************************************************************************/
    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View v;

        switch(viewType){
            case RCVD:
                v = (View) LayoutInflater.from(parent.getContext()).inflate(R.layout.msg_received, parent, false);
                return new ViewHolder(v);
            case SENT:
                v = (View) LayoutInflater.from(parent.getContext()).inflate(R.layout.msg_sent, parent, false);
                return new ViewHolder(v);
        }
        return null;
    }

    /*******************************************************************************************************
     NAME:			    onBindViewHolder
     INPUTS:		    holder: RecyclerView.ViewHolder (none), position: int (none)
     AUX VARIABLES:	    msg: Msg
     OUTPUTS:		    -
     DESCRIPTION:		decides what nickname will appear above the message based on the message type
     ********************************************************************************************************/
    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        Msg msg = msg_list.get(position);
        switch (msg.type){
            case RCVD:
                ((ViewHolder) holder).user.setText(msg.bt_nickname);
                break;
            case SENT:
                ((ViewHolder) holder).user.setText("");
                break;
            default:
                break;
        }

        ((ViewHolder) holder).text.setText(msg.text);
        ((ViewHolder) holder).time.setText(msg.time);
    }

    /*******************************************************************************************************
     NAME:			    getItemCount
     INPUTS:		    -
     AUX VARIABLES:	    -
     OUTPUTS:		    msg_list.size(): int / otherwise 0
     DESCRIPTION:		returns the size of the message list
     ********************************************************************************************************/
    @Override
    public int getItemCount() {
        if (msg_list != null)
            return msg_list.size();
        else {
            return 0;
        }
    }

    /*******************************************************************************************************
     NAME:			    getItemViewType
     INPUTS:		    type: int (final)
     AUX VARIABLES:	    message: Msg
     OUTPUTS:		    message.type: int / otherwise -1
     DESCRIPTION:		returns the type of a message in the message list
     ********************************************************************************************************/
    @Override
    public int getItemViewType(final int type) {
        Msg message = msg_list.get(type);
        if(msg_list != null){
            return message.type;
        }
        return -1;
    }

}
