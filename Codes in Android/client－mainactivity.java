package com.zhihuishu.ourandroid;


import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity
{

    Button cButton;
    EditText ipText;
    EditText portText;
    EditText msgText;
    TextView replyText;

    String ipAddress = "";
    Integer portNumber = 0;

    String userInput = "", replyMsg = "";
    Socket clientSocket = null;

    PrintWriter out;
    BufferedReader In;


    boolean clicked = false;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ipText = (EditText)findViewById(R.id.ipEditText);
        portText = (EditText)findViewById(R.id.portEditText);
        cButton = (Button)findViewById(R.id.connectButton);
        msgText = (EditText) findViewById(R.id.msgEditText);
        replyText = (TextView) findViewById(R.id.replyTextView);


    }



    public void connectToServer(View view) throws IOException
    {

        ipAddress = ipText.getText().toString();
        portNumber = Integer.parseInt(portText.getText().toString());



        new Thread(new Runnable(){
            public void run()
            {
                try
                {

                    clientSocket = new Socket(ipAddress,portNumber);


                }
                catch(Exception e){
                    Toast.makeText(getApplicationContext(),"Problem : Socket",Toast.LENGTH_LONG).show();
                }
            }
        }).start();



    }

    public void sendToServer(View view) throws IOException
    {

        Doit();     //once the method is over the chat is closed.

        In.close();
        clientSocket.close();
    }
    public void Doit()
    {
        try
        {

            userInput = msgText.getText().toString();
            replyText.setText(userInput);


            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader In = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));


            //userInput += "\n";
            out.write(userInput);
            // out.flush();
            replyMsg = In.readLine();
            Toast.makeText(getApplicationContext(), "Here", Toast.LENGTH_LONG).show();
            replyText.setText(replyMsg);



        }

        catch(Exception e)
        {
            Toast.makeText(getApplicationContext(),"Problem : Data",Toast.LENGTH_LONG).show();
        }

    }



}
