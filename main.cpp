#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <winbase.h>
#include <winbase.h>
#include <malloc.h>
#include <winuser.h>


#include "resource.h"

BOOL COM_exists( int port);
BOOL Open_COM(const char* portName);
BOOL WriteABuffer(HWND hwnd, char * lpBuf, DWORD dwToWrite);

HINSTANCE hInst;
HANDLE hComm;
DCB dcb;

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hEditOut;
    HWND COM_Combo;
    HWND BAUD_Combo;
    HWND BYTE_Combo;
    HWND PARITY_Combo;
    HWND BIT_Combo;
    HWND FLOW_Combo;

    char portName[8];

    switch(uMsg)
    {
        case WM_INITDIALOG:
        {

        hEditOut  = GetDlgItem ( hwndDlg, IDC_EDIT_SEND );

        COM_Combo = GetDlgItem ( hwndDlg, IDC_COM_PORT );
        int i;

        for (i = 1; i < 256; ++i)
        {
        if (COM_exists( i))
        {
            char COM[8];
            sprintf(COM, "COM%d", i);
            SendMessage( COM_Combo, CB_ADDSTRING, 0, ( LPARAM ) COM );

        }
       }
     }

            SendMessage ( COM_Combo, CB_SETCURSEL, 0, 0 );



        BAUD_Combo = GetDlgItem ( hwndDlg, IDC_BAUD_RATE );

        // And add some information to it
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "110" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "300" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "600" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "1200" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "4800" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "9600" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "14400" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "19200" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "38400" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "56000" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "57600" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "115200" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "128000" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "256000" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "345600" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "460800" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "512000" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "691200" );
        SendMessage ( BAUD_Combo, CB_ADDSTRING, 0, ( LPARAM ) "921600" );


        SendMessage ( BAUD_Combo, CB_SETCURSEL, 0, 0 );


        BYTE_Combo  = GetDlgItem ( hwndDlg, IDC_BYTE_SIZE );

        SendMessage ( BYTE_Combo, CB_ADDSTRING, 0, ( LPARAM ) "5" );
        SendMessage ( BYTE_Combo, CB_ADDSTRING, 0, ( LPARAM ) "6" );
        SendMessage ( BYTE_Combo, CB_ADDSTRING, 0, ( LPARAM ) "7" );
        SendMessage ( BYTE_Combo, CB_ADDSTRING, 0, ( LPARAM ) "8" );

        SendMessage ( BYTE_Combo, CB_SETCURSEL, 0, 0 );


        PARITY_Combo  = GetDlgItem ( hwndDlg, IDC_PARITY );

        SendMessage ( PARITY_Combo, CB_ADDSTRING, 0, ( LPARAM ) "None" );
        SendMessage ( PARITY_Combo, CB_ADDSTRING, 0, ( LPARAM ) "Odd" );
        SendMessage ( PARITY_Combo, CB_ADDSTRING, 0, ( LPARAM ) "Even" );
        SendMessage ( PARITY_Combo, CB_ADDSTRING, 0, ( LPARAM ) "Mark" );
        SendMessage ( PARITY_Combo, CB_ADDSTRING, 0, ( LPARAM ) "Space" );

        SendMessage ( PARITY_Combo, CB_SETCURSEL, 0, 0 );


        BIT_Combo  = GetDlgItem ( hwndDlg, IDC_STOP_BIT );

        SendMessage ( BIT_Combo, CB_ADDSTRING, 0, ( LPARAM ) "1" );
        SendMessage ( BIT_Combo, CB_ADDSTRING, 0, ( LPARAM ) "2" );
        SendMessage ( BIT_Combo, CB_ADDSTRING, 0, ( LPARAM ) "1.5" );

        SendMessage ( BIT_Combo, CB_SETCURSEL, 0, 0 );


        FLOW_Combo  = GetDlgItem ( hwndDlg, IDC_FLOW_CTRL );

        SendMessage ( FLOW_Combo, CB_ADDSTRING, 0, ( LPARAM ) "None" );
        SendMessage ( FLOW_Combo, CB_ADDSTRING, 0, ( LPARAM ) "XOnXOff" );
        SendMessage ( FLOW_Combo, CB_ADDSTRING, 0, ( LPARAM ) "RequestToSend" );
        SendMessage ( FLOW_Combo, CB_ADDSTRING, 0, ( LPARAM ) "RequestToSendXOnXOff" );

        SendMessage ( FLOW_Combo, CB_SETCURSEL, 0, 0 );



     return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_BTN_SEND:
                {
                    //portName
                    if(Open_COM("COM2"))
                    {


                    //FillMemory(&dcb, sizeof(dcb), 0);
                    //dcb.DCBlength = sizeof(dcb);
                   /* if (!BuildCommDCB("19200,n,8,1", &dcb))
                    {
                      // Couldn't build the DCB. Usually a problem
                      // with the communications specification string.
                      MessageBox(hwndDlg, "Couldn't build the DCB.", "ERROR", MB_ICONERROR);
                      //return FALSE;
                    }
                    else
                    {*/
                     dcb.DCBlength = sizeof(dcb);

                     if (!GetCommState(hComm, &dcb))     // get current DCB
                     {
                         MessageBox(hwndDlg, "Couldn't GetCommState.", "ERROR", MB_ICONERROR);
                     // Error in GetCommState
                     }
                    else
                    {

                      // Update DCB rate.
                      dcb.BaudRate = CBR_19200;
                      dcb.ByteSize = 8;
                      /*dcb.fBinary = TRUE;
                      dcb.fErrorChar = FALSE;
                      dcb.fNull = FALSE;
                      dcb.fAbortOnError = FALSE;*/

                      dcb.Parity = NOPARITY;
                      //dcb.fParity = FALSE;
                      dcb.StopBits = ONESTOPBIT;

                     /* dcb.fRtsControl = RTS_CONTROL_DISABLE;
                      dcb.fOutxCtsFlow = FALSE;
                      dcb.fDtrControl = DTR_CONTROL_DISABLE;
                      dcb.fOutxDsrFlow = FALSE;
                      dcb.fDsrSensitivity = FALSE;
                      dcb.fInX = FALSE;
                      dcb.fOutX = FALSE;*/

                     if (!SetCommState(hComm, &dcb))
                     {
                      MessageBox(hwndDlg, "Couldn't SetCommState.", "ERROR", MB_ICONERROR);
                     }
                     else
                     {

                            int iChars = GetWindowTextLength(GetDlgItem(hwndDlg, IDC_EDIT_SEND))+1; // Room for '\0'
                            char* szBuffer;
                            szBuffer = (char*) malloc (sizeof(char)*iChars);

                            GetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_SEND), szBuffer, iChars);

                            if(WriteABuffer(hwndDlg, szBuffer, iChars-1))
                            {

                            }
                            else
                            {
                             MessageBox(hwndDlg, "Couldn't WriteABuffer.", "ERROR", MB_ICONERROR);
                            }
                             free(szBuffer);


                    }
                    }
                     // DCB is ready for use.
                  //  }
                     Sleep(1000);
                     CloseHandle(hComm);
                    }
                    else
                    {
                            MessageBox(hwndDlg, "Couldn't Open_COM.", "ERROR", MB_ICONERROR);
                    }

                }
                 //EndDialog(hwndDlg, 0);
                return TRUE;

                case IDC_BTN_CLR:
                    MessageBox(hwndDlg, "You clicked \"Test\" button!", "Information", MB_ICONINFORMATION);
                    return TRUE;
            }
    }

    return FALSE;
}

BOOL COM_exists( int port)
{
    char buffer[7];
    COMMCONFIG CommConfig;
    DWORD size;

    if (! (1 <= port && port <= 255))
    {
        return FALSE;
    }

    snprintf( buffer, sizeof buffer, "COM%d", port);
    size = sizeof CommConfig;

    // COM port exists if GetDefaultCommConfig returns TRUE
    // or changes <size> to indicate COMMCONFIG buffer too small.
    return (GetDefaultCommConfig( buffer, &CommConfig, &size)
                                                    || size > sizeof CommConfig);
}


BOOL Open_COM(const char* portName)
{
    bool ret;
             hComm = CreateFile(portName,  // Specify port device: default "COM1"
             GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
             0,                                  // the devide isn't shared.
             NULL,                               // the object gets a default security.
             OPEN_EXISTING,                      // Specify which action to take on file.
             0,                                  // default.
             NULL);
    if (hComm == INVALID_HANDLE_VALUE)
        ret = false;
    else
        ret = true;
   // error opening port; abort
    return ret;
}

BOOL WriteABuffer(HWND hwnd, char * lpBuf, DWORD dwToWrite)
{
   OVERLAPPED osWrite = {0};
   DWORD dwWritten;
   BOOL fRes;




  // Create this writes OVERLAPPED structure hEvent.
   osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (osWrite.hEvent == NULL)
      // Error creating overlapped event handle.
      return FALSE;

   // Issue write.
   if (!WriteFile(hComm, lpBuf, dwToWrite, &dwWritten, &osWrite)) {
      if (GetLastError() != ERROR_IO_PENDING) {
         // WriteFile failed, but it isn't delayed. Report error and abort.
         fRes = FALSE;
      }
      else {
         // Write is pending.
         if (!GetOverlappedResult(hComm, &osWrite, &dwWritten, TRUE))
            fRes = FALSE;
         else
            // Write operation completed successfully.
            fRes = TRUE;
      }
   }
   else
      // WriteFile completed immediately.
      fRes = TRUE;

   CloseHandle(osWrite.hEvent);
   return fRes;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;

    // The user interface is a modal dialog box
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DialogProc);
}



