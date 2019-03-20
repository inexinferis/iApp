#include "main.h"

/**************************************************************\
  Connecting Dlg
\**************************************************************/

BEGIN_EVENT_TABLE(ConnectingDlg,XSkinnedDlg)
  EVENT_CREATE(ConnectingDlg::OnCreate)
  EVENT_BN_CLICKED(IDCANCEL,ConnectingDlg::OnCancel)
  EVENT_DESTROY(ConnectingDlg::OnDestroy)
END_EVENT_TABLE()

INT ConnectingDlg::OnCreate(LPCREATESTRUCT /*pcs*/){
  //set icon...
  SetIcon(LoadIcon(XGetInstanceHandle(),MAKEINTRESOURCE(IDI_IAPPICON)),FALSE);
  //center wnd
  CenterWindow();
  //create static
  sStatus.Create(*this,STR_CONNECTING,50,XRect(10,10,250,30),XDEFSTATICSTYLE|SS_VCENTER);
  bCancel.Create(*this,STR_CANCEL,IDCANCEL,XRect(240,40,120,24),XDEFSTATICSTYLE|WS_TABSTOP|SS_VCENTER);
  pApp->con.SetStatusCallbackEx(this,LPXINTERNET_STATUS_CALLBACK(LPXWND_INTERNET_STATUS_CALLBACK(&ConnectingDlg::OnStatusCallback)));
  return 0;
}

BOOL ConnectingDlg::OnStatusCallback(HINTERNET /*hInternet*/,DWORD_PTR /*dwContext*/,DWORD dwInternetStatus,
  LPVOID /*lpvStatusInformation*/,DWORD /*dwStatusInformationLength*/){
  static DWORD dwLastStatus=0;
  switch (dwInternetStatus){
    case INTERNET_STATUS_RESOLVING_NAME:case INTERNET_STATUS_NAME_RESOLVED:
      if(dwLastStatus!=1){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_RESOLVING);
        dwLastStatus=1;
      }
		break;
    case INTERNET_STATUS_CONNECTING_TO_SERVER:case INTERNET_STATUS_CONNECTED_TO_SERVER:
      if(dwLastStatus!=2){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_CONNECTING);
        dwLastStatus=2;
      }
		break;
		case INTERNET_STATUS_SENDING_REQUEST:case INTERNET_STATUS_REQUEST_SENT:
		  if(dwLastStatus!=3){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_SENDINGREQUEST);
        dwLastStatus=3;
      }
		break;
    case INTERNET_STATUS_RECEIVING_RESPONSE:case INTERNET_STATUS_RESPONSE_RECEIVED:
      if(dwLastStatus!=4){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_RECEIVINGRESPONSE);
        dwLastStatus=4;
      }
		break;
    case INTERNET_STATUS_CLOSING_CONNECTION:case INTERNET_STATUS_CONNECTION_CLOSED:
      if(dwLastStatus!=5){
        sStatus.SetWindowTextEx(XGetInstanceHandle(),STR_CLOSINGCONNECTION);
        dwLastStatus=5;
      }
		break;
    default:break;
	}
  return TRUE;
}

VOID ConnectingDlg::OnCancel(WPARAM /*wParam*/,LPARAM /*lParam*/){
  pApp->con.Disconnect();
}

VOID ConnectingDlg::OnDestroy(){
  pApp->con.RemoveStatusCallbackEx();
}
