<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
xmlns:tools="http://schemas.android.com/tools"
android:layout_width="match_parent"
android:layout_height="match_parent"
    android:orientation="vertical"
tools:context="com.zhihuishu.ourandroid.MainActivity" >



    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="250dp"
        android:orientation="vertical">
        <LinearLayout
            android:layout_width="match_parent"
            android:layout_height="100dp"
            android:orientation="horizontal">
        <TextView
            android:layout_gravity="left"
            android:layout_height="wrap_content"
            android:layout_width="wrap_content"
            android:text="ADDRESS"/>

        <EditText
            android:id="@+id/ipEditText"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"

            />
            </LinearLayout>
            <LinearLayout
                android:layout_width="match_parent"
                android:layout_height="100dp"
                android:orientation="horizontal">

                <TextView
                    android:layout_gravity="left"
                    android:layout_height="wrap_content"
                    android:layout_width="wrap_content"
                    android:text="PORT"/>
                <EditText
            android:id="@+id/portEditText"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"

             />
                </LinearLayout>

    </LinearLayout>

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical">
        <Button
            android:id="@+id/connectButton"
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:text="LOGIN"
            android:onClick="connectToServer"/>
        <EditText
            android:layout_width="match_parent"
            android:layout_height="wrap_content"
            android:id="@+id/msgEditText"/>
        <TextView
            android:id="@+id/replyTextView"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"/>
    </LinearLayout>


</LinearLayout>
