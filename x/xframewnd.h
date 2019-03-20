#ifndef _XFRAMEWND_H_
#define _XFRAMEWND_H_

#include "xwnd.h"

#define XDEFAULTFRAMEWNDCLASSNAME _T("XFrameWndClass")
#define XDEFAULTFRAMEWNDSTYLE WS_OVERLAPPEDWINDOW
#define XDEFAULTFRAMEWNDSTYLEEX 0//WS_EX_APPWINDOW

class XFrameWnd:public XWnd{
  DECLARE_DYNCREATE(XFrameWnd)
  public:
    XFrameWnd(HWND hWnd=NULL):XWnd(hWnd){}
    virtual ~XFrameWnd(){}

    virtual HWND Create(LPCTSTR lpszWindowName,LPCTSTR lpszClassName=XDEFAULTFRAMEWNDCLASSNAME,DWORD dwStyle=XDEFAULTFRAMEWNDSTYLE,
      const RECT& rect=XDEFAULTRECT,HWND hParentWnd=NULL,LPCTSTR lpszMenu=NULL,DWORD dwExStyle=XDEFAULTFRAMEWNDSTYLEEX,LPVOID lpParam=NULL);
    virtual HWND LoadFrame(UINT uResID,DWORD dwStyle=XDEFAULTFRAMEWNDSTYLE,HWND hParentWnd=NULL,LPVOID lpParam=NULL);

    BOOL LoadAccelTable(HINSTANCE hInstance,LPCTSTR lpszResourceName);

    virtual VOID ActivateFrame(INT nCmdShow=-1);
    virtual VOID RecalcLayout();

  protected:

    //internal helper
    virtual BOOL CreateClient(LPCREATESTRUCT lpcs);
    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(LPMSG lpMsg);

    virtual LRESULT OnCommand(WPARAM wParam,LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

    //events
    INT OnCreate(LPCREATESTRUCT pcs);
    VOID OnSize(UINT nType,SIZES sSize);

  protected:
    HACCEL m_hAccelTable;

  DECLARE_EVENT_TABLE()
};

#endif //_XFRAMEWND_H_
