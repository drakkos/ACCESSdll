#include <jni.h>
#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include "AccessDLL.h"
#include <string.h>
#include <sstream>

using namespace std;

static FILTERKEYS fk2 = { sizeof (FILTERKEYS), 0 };

string getCurrentApplication() {
    HWND win;
    TCHAR text[MAX_PATH];
    DWORD process;
    DWORD tid;
    int last;
    HANDLE pid;
    string str = "";

    // Here we get the current handle for the foreground window.
    win = GetForegroundWindow();

    // Next, we need to find the process to which that window
    // belongs.
    tid = GetWindowThreadProcessId (win, &process);

    pid = OpenProcess (PROCESS_ALL_ACCESS, FALSE, process);
//    cout << process << endl;
    // Next, we need to associate that PID with the .exe that
    // spawned it.
    GetModuleFileNameEx (pid, NULL, text, MAX_PATH);

    for (int i = 0; i < MAX_PATH; i++) {
        if ((text[i] < '1' || text[i] > 'z') && text[i] != ' ') {
            last = i;
            return str;
        }
        str = str + text[i];
    }

    return str;
}

jstring convertToString (JNIEnv *env, string arg) {
    return env->NewStringUTF (arg.data());
}

JNIEXPORT jstring JNICALL Java_accessframework_WindowsXpContext_getApplication
  (JNIEnv *env, jclass obj)
{

    return convertToString (env, getCurrentApplication());
}


string getKey (HKEY__* code, const char * subkey, const char * entry, const char * type) {
    HKEY hKey = 0;
    int buf[255];// = {0};
    char buf2[255];
    char aKey[1024], aValue[1024];
    DWORD dwType = 0;
    DWORD dwBufSize = sizeof (buf);
    ostringstream str;

    if (strcmp (type, "REG_SZ") == 0) {
        dwType = REG_SZ;
    }
    else if (strcmp (type, "REG_EXPAND_SZ") == 0) {
        dwType = REG_EXPAND_SZ;
    }
    else if (strcmp (type, "REG_DWORD") == 0) {
        dwType = REG_DWORD;
    }

//    cout << subkey << "/" << entry << endl;

    strcpy (aKey, subkey);
    strcpy (aValue, entry);

    dwType = RegOpenKeyEx (code, aKey, 0, KEY_ALL_ACCESS, &hKey);

//    cout << "Error: " << dwType << endl;
    
    if (dwType == ERROR_SUCCESS) {
//        cout << "Opened Key" << endl;
        if (strcmp (type, "REG_SZ") == 0) {
            RegQueryValueEx (hKey, entry, 0, &dwType, (LPBYTE)buf2, &dwBufSize);
            str << buf2;
        }
        else if (strcmp (type, "REG_EXPAND_SZ") == 0) {
            RegQueryValueEx (hKey, entry, 0, &dwType, (LPBYTE)buf2, &dwBufSize);
            str << buf2;
        }
        else {
            RegQueryValueEx (hKey, entry, 0, &dwType, (LPBYTE)buf, &dwBufSize);
            str << "0x" << buf[0];
        }

//        cout << "Read: " << str << "End  " << endl;

        RegCloseKey(hKey);
        return str.str();
    }

    RegCloseKey(hKey);
    return "No key found";
}

void setStringKey (HKEY__* code, const char * subkey, const char * entry, const char * type, const char *str) {
    HKEY hKey = 0;
    int buf[255] ={0};
    char aKey[1024], aValue[1024];
    DWORD dwType = 0;
    DWORD dwDisp;
    DWORD error;
    DWORD dwBufSize = sizeof (str);
    DWORD num;
    ostringstream tstr;

    if (strcmp (type, "REG_SZ") == 0) {
        cout << "REG_SZ" << endl;
        dwType = REG_SZ;
    }
    else if (strcmp (type, "REG_EXPAND_SZ") == 0) {
        cout << "REG_SZ" << endl;
        dwType = REG_EXPAND_SZ;
    }
    else {
        cout << "REG_DWORD" << endl;
        dwType = REG_DWORD;
    }

    cout << type << endl;

    strcpy (aKey, subkey);
    strcpy (aValue, entry);

    error = RegCreateKeyEx (code, aKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisp);

    cout << "Error: " << error << endl;

    cout << "Type is " << type << endl;

    if (error == ERROR_SUCCESS) {

        if (strcmp (type, "REG_DWORD") == 0) {
            cout << "Writing " << str << " as DWORD" << endl;
            num = atoi (&str[0]);

            RegSetValueEx (hKey, entry, 0, dwType, (BYTE *)&num, sizeof (DWORD));
        }
        else {
            cout << "Writing " << str << " as SZ" << endl;
            RegSetValueEx (hKey, entry, 0, dwType, (LPBYTE)str, strlen(str));
        }
    }

    RegCloseKey(hKey);
}

JNIEXPORT jstring JNICALL Java_accessframework_WindowsXpContext_getSystemKey
  (JNIEnv *env, jclass obj, jstring key, jstring entry, jstring type) {
    return convertToString (env, getKey (HKEY_LOCAL_MACHINE,
            env->GetStringUTFChars(key, false),
            env->GetStringUTFChars(entry, false),
            env->GetStringUTFChars(type, false)));
}

JNIEXPORT jstring JNICALL Java_accessframework_WindowsXpContext_getUserKey
  (JNIEnv *env, jclass obj, jstring key, jstring entry, jstring type) {
    return convertToString (env, getKey (HKEY_CURRENT_USER,
            env->GetStringUTFChars(key, false),
            env->GetStringUTFChars(entry, false),
            env->GetStringUTFChars(type, false)));
}

JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_setSystemKey
  (JNIEnv *env, jclass obj, jstring key, jstring entry, jstring type, jstring val) {
    setStringKey (HKEY_LOCAL_MACHINE,
            env->GetStringUTFChars(key, false),
            env->GetStringUTFChars(entry, false),
            env->GetStringUTFChars(type, false),
            env->GetStringUTFChars (val, false));
}

JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_setUserKey
  (JNIEnv *env, jclass obj, jstring key, jstring entry, jstring type, jstring val) {
    setStringKey (HKEY_CURRENT_USER,
            env->GetStringUTFChars(key, false),
            env->GetStringUTFChars(entry, false),
            env->GetStringUTFChars(type, false),
            env->GetStringUTFChars(val, false));
}

JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_refreshRegistry
  (JNIEnv *env, jclass obj, jint x, jint y, jint val) {

    SystemParametersInfo (x, val, NULL, y);
 //   cout << "aaaaaa" << endl;
}

JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_refreshRegistryAlt
  (JNIEnv *env, jclass obj, jint x, jint y, jint val) {

    SystemParametersInfo (x, 0, (void*)val, 0);
}


JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_refreshFilterKeys
  (JNIEnv *env, jclass obj, jint code, jint iwait, jint idelay, jint irepeat, jint ibounce) {
    FILTERKEYS fk = { sizeof (FILTERKEYS), 0 } ;
    bool result;
    
    fk2.cbSize = sizeof (FILTERKEYS);
    
    result = SystemParametersInfo (SPI_GETFILTERKEYS, sizeof (FILTERKEYS), &fk2, SPIF_SENDCHANGE);

    if (!result) {
        cout << "Error in Getting" << endl;
    }
    else {
        cout << "Success in getting" << endl;
    }
    
//    cout << "Before Wait: " << fk2.iWaitMSec << endl;
//    cout << "Before Repeat: " << fk2.iRepeatMSec << endl;
    
    fk.dwFlags = fk2.dwFlags;
    
    fk.dwFlags = FKF_FILTERKEYSON;

    if (iwait != -1) {
        fk.iWaitMSec = iwait;
    }
    else {
        fk.iWaitMSec = fk2.iWaitMSec;
    }

    if (ibounce != -1) {
        fk.iBounceMSec = ibounce;
    }
    else {
        fk.iBounceMSec = fk2.iBounceMSec;
    }

    if (idelay != -1) {
        fk.iDelayMSec = idelay;
    }
    else {
        fk.iDelayMSec = fk2.iDelayMSec;
    }

    if (irepeat != -1) {
        fk.iRepeatMSec = irepeat;
    }
    else {
        fk.iRepeatMSec = fk2.iRepeatMSec;
    }

    fk.cbSize = sizeof (FILTERKEYS);

    cout << "Flag " << fk.dwFlags << endl;
    cout << "Wait Inside " << fk.iWaitMSec << endl;
    
    result = SystemParametersInfo (SPI_SETFILTERKEYS, sizeof (FILTERKEYS), &fk, 0);

    if (!result) {
        cout << "No Success: " << GetLastError() << endl;
    }
    else {
        cout << "Success in setting" << endl;
    }

}

JNIEXPORT jint JNICALL Java_accessframework_WindowsXpContext_queryMouseThreshold1
  (JNIEnv *env, jclass obj) {
  long speed[3];

  SystemParametersInfo(SPI_GETMOUSE, NULL, speed, NULL);

  return speed[1];
}

JNIEXPORT jint JNICALL Java_accessframework_WindowsXpContext_queryMouseThreshold2
  (JNIEnv *env, jclass obj) {
  long speed[3];

  SystemParametersInfo(SPI_GETMOUSE, NULL, speed, NULL);

  return speed[2];
}

JNIEXPORT jint JNICALL Java_accessframework_WindowsXpContext_queryMouseAcceleration
  (JNIEnv *env, jclass obj) {
  long speed[3];

  SystemParametersInfo(SPI_GETMOUSE, NULL, speed, NULL);

  return speed[0];
}

JNIEXPORT void JNICALL Java_accessframework_WindowsXpContext_refreshMouse
  (JNIEnv *env, jclass obj, jint speed, jint thresh1, jint thresh2) {

  long nspeed[3], new_speed[3];

    SystemParametersInfo(SPI_GETMOUSE, NULL, nspeed, NULL);

    cout << "Before: "<< nspeed[0] << ", " << nspeed[1] << ", "<< nspeed[2] << endl;
    
    if (speed != -1) {
        new_speed[0] = speed;
    }
    else {
        new_speed[0] = nspeed[0];
    }

    if (thresh1 != -1) {
        new_speed[1] = thresh1;
    }
    else {
        new_speed[1] = nspeed[1];
    }

    if (thresh2 != -1) {
        new_speed[2] = thresh2;
    }
    else {
        new_speed[2] = nspeed[2];
    }

        cout << "To: " << new_speed[0] << ", " << new_speed[1] << ", "<< new_speed[2] << endl;

    SystemParametersInfo(SPI_SETMOUSE, 0, &new_speed, 0);
}

void ErrorExit(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    char lpMsgBuf[4096];
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw, 0,
        lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process


    cout << "Error: " << GetLastError() << ": " << lpMsgBuf << endl;
    
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(dw);
}
