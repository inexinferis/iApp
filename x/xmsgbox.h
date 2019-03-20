#ifndef _XMSGBOX_H_
#define _XMSGBOX_H_

#include "xdialog.h"
#include "xstatic.h"
#include "xbutton.h"
#include "xgdi.h"

#define MAX_RES_STRING           1024

#define XMSGBOX_MAXBTNS          4   // number of buttons
#define XMSGBOX_MAXBTNSTR        32  // buffer for button strings
#define XMSGBOX_BTNSTRRSRC       799 // string resources offset

#define XMSGBOX_DU_OUTERMARGIN   7   // margin of sections
#define XMSGBOX_DU_INNERMARGIN   10  // margin of controls
#define XMSGBOX_DU_BUTTONGAP     4   // space between buttons
#define XMSGBOX_DU_BTNHEIGHT     14  // button height
#define XMSGBOX_DU_BTNWIDTH      50  // button width

#define XMSGBOX_CLASSNAME        _T("XMsgBoxClass")
#define WM_XMSGBOX_TIMEOUT       WM_USER+0x0102 //A Default Notify MSGBOX TimeOut

class XMsgBox:public XDialog{
  public:
    XMsgBox();
    virtual ~XMsgBox();

    INT MessageBox(HWND hWnd,LPCTSTR lpText,LPCTSTR lpCaption=NULL,UINT uType=0,UINT uTimeout=INFINITE);

  protected:

    BOOL OnInitDialog(WPARAM wParam,LPARAM lParam);
    INT OnHelp(WPARAM wParam,LPARAM lParam);
    INT OnOption(DWORD dwParam,HWND hWnd);
    INT OnClose();
    VOID OnTimeOut(TIMERPROC lpTimerFunc);

  private:
    HMODULE m_hUser32;
    HICON m_hIcon;HFONT m_hFont;
    LPCTSTR m_lpCaption,m_lpText;
    UINT m_uType,m_uNumButtons,m_uTimeout;
    BOOL m_bTimeOutSetted;

  protected:
    // Overloaded for Skinning...
    virtual XStatic *GetStaticIcon(){
      return &m_StcIcon;
    }

    virtual XStatic *GetStaticText(){
      return &m_StcText;
    }

    virtual XButton *GetButton(UINT i){
      if(i<XMSGBOX_MAXBTNS)
        return &m_Buttons[i];
      return &m_Buttons[0];
    }

    XStatic m_StcIcon,m_StcText;
    XButton m_Buttons[XMSGBOX_MAXBTNS];

  DECLARE_EVENT_TABLE()
};

#endif //_XMSGBOX_H_
