package com.example.tutorialspoint;

import android.content.Context;
import android.os.Bundle;
import android.app.Activity;

import android.telephony.SmsManager;
import android.content.SharedPreferences;
import android.content.Intent;
import android.app.PendingIntent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String PREF_NAME = "Name";
    SharedPreferences settings;
    Button off0;
    Button on0;
    Button on2;
    Button off2;
    Button reset2;
    Button reset0;
    Button resetall;
    Button offall;
    Button onall;
    Button arduino;
    Button s1;
    Button s2;
    Button s3;
    Button sall;
    Button save;

    Button s4;

    Button s5;

    Button s1r;
    Button s2r;
    Button s3r;
    Button s4r;
    Button s5r;


    EditText txtphoneNo;

    String phoneNo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        getActionBar().hide();

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        off0 = (Button) findViewById(R.id.off0);
        on0 = (Button) findViewById(R.id.on0);
        off2 = (Button) findViewById(R.id.off2);
        on2 = (Button) findViewById(R.id.on2);
        onall = (Button) findViewById(R.id.onall);
        offall = (Button) findViewById(R.id.offall);
        reset0 = (Button) findViewById(R.id.reset0);
        reset2 = (Button) findViewById(R.id.reset2);
        resetall = (Button) findViewById(R.id.resetall);
        s1 = (Button) findViewById(R.id.s1);
        s2 = (Button) findViewById(R.id.s2);
        s3 = (Button) findViewById(R.id.s3);
        s4 = (Button) findViewById(R.id.s4);
        s5 = (Button) findViewById(R.id.s5);
        save = (Button) findViewById(R.id.save);
        arduino = (Button) findViewById(R.id.arduino);
        s1r = (Button) findViewById(R.id.s1r);
        s2r = (Button) findViewById(R.id.s2r);
        s3r = (Button) findViewById(R.id.s3r);
        s4r = (Button) findViewById(R.id.s4r);
        s5r = (Button) findViewById(R.id.s5r);


        txtphoneNo = (EditText) findViewById(R.id.editTextPhone);


        settings = getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);

        phoneNo = settings.getString(PREF_NAME, "не определено");

        txtphoneNo.setText(phoneNo);


        off0.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("0off");

            }
        });
        on0.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("0on");

            }
        });
        on2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("2on");

            }
        });
        off2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("2off");

            }
        });
        reset2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("2reset");

            }
        });
        reset0.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("0reset");

            }
        });
        resetall.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("allreset");

            }
        });
        offall.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("alloff");

            }
        });
        onall.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("allon");

            }
        });
        s1.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s1");

            }
        });
        s2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s2");

            }
        });
        s3.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s3");

            }
        });
        s4.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s4");

            }
        });
        s5.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s5");

            }
        });
        s1r.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s1reset");

            }
        });
        s2r.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s2reset");

            }
        });
        s3r.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s3reset");

            }
        });
        s4r.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s4reset");

            }
        });
        s5r.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("s5reset");

            }
        });
        arduino.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                sendSMSMessage("arduino");

            }
        });
        save.setOnClickListener(new View.OnClickListener() {
            public void onClick(View view) {
                SaveMe();
                Toast.makeText(getApplicationContext(), "Save successfully!",
                        Toast.LENGTH_LONG).show();
            }
        });
    }

    protected void sendSMSMessage(String message) {
        phoneNo = txtphoneNo.getText().toString();
        SaveMe();
        Intent intent = new Intent(getApplicationContext(), MainActivity.class);
        PendingIntent pi = PendingIntent.getActivity(getApplicationContext(), 0, intent, PendingIntent.FLAG_IMMUTABLE);

        //Get the SmsManager instance and call the sendTextMessage method to send message
        SmsManager sms = SmsManager.getDefault();
        sms.sendTextMessage(phoneNo, null, message, pi, null);

        Toast.makeText(getApplicationContext(), "Message Sent & Save successfully!",
                Toast.LENGTH_LONG).show();
    }
    public void SaveMe(){

        SharedPreferences.Editor prefEditor = settings.edit();
        prefEditor.putString(PREF_NAME, phoneNo);
        prefEditor.commit();
    }
}
