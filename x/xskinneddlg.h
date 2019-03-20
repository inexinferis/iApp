#ifndef _XSKINNEDDLG_H_
#define _XSKINNEDDLG_H_

#include "xdialog.h"
#include "xskinnedwnd.h"
#include "xskinnedmsgbox.h"

#define XDEFAULTSKINNEDDLGCLASSNAME _T("XSkinnedDlgClass")
#define XDEFAULTSKINNEDSTYLE WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SIZEBOX
#define XDEFAULTSKINNEDSTYLEEX 0

class XSkinnedDlg:public XDialog,XSkinnedWnd{
  DECLARE_DYNAMIC(XSkinnedDlg)
  public:
    XSkinnedDlg(HWND hWnd=NULL):XDialog(hWnd),XSkinnedWnd(this){
      m_SMB=new XSkinnedMsgBox();
    }

    virtual ~XSkinnedDlg(){
      delete m_SMB;
    }

    virtual HWND CreateEx(DWORD dwExStyle,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTSKINNEDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL){
      return XDialog::CreateEx(dwExStyle,XDEFAULTSKINNEDDLGCLASSNAME,lpWindowName,dwStyle,rect,hWndParent,lpMenuName,lParam);
    }

    virtual INT_PTR CreateDlgBoxEx(DWORD dwExStyle,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTSKINNEDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL){
      return XDialog::CreateDlgBoxEx(dwExStyle,XDEFAULTSKINNEDDLGCLASSNAME,lpWindowName,dwStyle,rect,hWndParent,lpMenuName,lParam);
    }

    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    BOOL OnSetText(LPSTR swText);
    BOOL OnNCCreate(LPCREATESTRUCT cs);
    VOID OnNCPaint(HRGN hRgn);
    BOOL OnNCActivate(BOOL bActivate,HRGN hRgn);
    INT  OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams);
    LRESULT  OnNCHitTest(POINTS pt);
    VOID OnNCLButtonDown(UINT iHittest,POINTS pt);
    VOID OnNCLButtonUp(UINT iHittest,POINTS pt);
    VOID OnNCLButtonDbClk(UINT iHittest,POINTS pt);
    VOID OnNCRButtonDown(UINT iHittest,POINTS pt);
    VOID OnNCMouseMove(UINT iHittest,POINTS pt);
    VOID OnNCDestroy();
    VOID OnInitMenuPopup(HMENU hMenu,DWORD info);

    INT MessageBox(LPCTSTR lpText,LPCTSTR lpCaption=NULL,UINT uType=0,UINT uTimeout=INFINITE);

  protected:
    XSkinnedMsgBox *m_SMB;

  DECLARE_EVENT_TABLE()
};

#endif //_XSKINNEDDLG_H_

