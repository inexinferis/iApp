#ifndef _XDIALOG_H_
#define _XDIALOG_H_

#include "xwnd.h"

#define XDEFAULTDIALOGCLASSNAME _T("XDialogWndClass")
#define XDEFAULTDIALOGSTYLE WS_OVERLAPPEDWINDOW
#define XDEFAULTDIALOGSTYLEEX WS_EX_APPWINDOW

class XDialog:public XWnd{
  DECLARE_DYNAMIC(XDialog)
  public:
    XDialog(HWND hWnd=NULL):XWnd(hWnd){}
    virtual ~XDialog(){}

    virtual HWND Create(HWND hWndParent,LPCTSTR lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL){
      return XWnd::CreateDlg(hWndParent,lpTemplate,dwInitParam,hInstance);
    }

    virtual HWND CreateIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL){
      return XWnd::CreateDlgIndirect(hWndParent,lpTemplate,dwInitParam,hInstance);
    }

    virtual HWND CreateEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL){
      return XWnd::CreateDlgEx(dwExStyle,lpClassName,lpWindowName,dwStyle,rect,hWndParent,lpMenuName,lParam);
    }

    virtual INT_PTR CreateDlgBox(HWND hWndParent,LPCTSTR lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL){
      return XWnd::CreateDlgBox(hWndParent,lpTemplate,dwInitParam,hInstance);
    }

    virtual INT_PTR CreateDlgBoxIndirect(HWND hWndParent,LPCDLGTEMPLATE lpTemplate,LPARAM dwInitParam=0,HINSTANCE hInstance=NULL){
      return XWnd::CreateDlgBoxIndirect(hWndParent,lpTemplate,dwInitParam,hInstance);
    }

    virtual INT_PTR CreateDlgBoxEx(DWORD dwExStyle,LPCTSTR lpClassName,LPCTSTR lpWindowName,DWORD dwStyle=XDEFAULTWNDSTYLE,CONST RECT& rect=XDEFAULTRECT,
      HWND hWndParent=NULL,LPCTSTR lpMenuName=NULL,LPVOID lParam=NULL){
      return XWnd::CreateDlgBoxEx(dwExStyle,lpClassName,lpWindowName,dwStyle,rect,hWndParent,lpMenuName,lParam);
    }

    DWORD GetDefId(){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,DM_GETDEFID,0,0);
    }

    VOID SetDefId(DWORD uDefId){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,DM_SETDEFID,uDefId,0);
    }

    VOID Reposition(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,DM_REPOSITION,0,0);
    }

    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs){
      return XWnd::PreCreateWindow(wcs);
    }

    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx){
      return XWnd::PreRegisterClass(wcx);
    }

    virtual BOOL PreTranslateMessage(MSG* pMsg){
      return XWnd::PreTranslateInput(pMsg);
    }

    virtual BOOL EndDialog(INT_PTR nResult=0);

    HBRUSH OnCtlColorDlg(HDC hDC,HWND hDlg);

  DECLARE_EVENT_TABLE()
};

#endif //_XDIALOG_H_
