#ifndef _CONNECTINGDLG_H_
#define _CONNECTINGDLG_H_

#include "main.h"

class ConnectingDlg: public XSkinnedDlg {
  public:
    ConnectingDlg(IApp *app):pApp(app){}
    ~ConnectingDlg(){}
    //eventos...
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnCancel(WPARAM wParam,LPARAM lParam);
    VOID OnDestroy();

    virtual BOOL OnStatusCallback(HINTERNET hInternet,DWORD_PTR dwContext,DWORD dwInternetStatus,
      LPVOID lpvStatusInformation,DWORD dwStatusInformationLength);

  private:
    IApp *pApp;
    XSkinnedStatic sStatus;
    XSkinnedButton bCancel;

  DECLARE_EVENT_TABLE()
};

#endif //_CONNECTINGDLG_H_
