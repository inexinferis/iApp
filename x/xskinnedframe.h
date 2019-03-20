#ifndef _XSKINNEDFRAME_H_
#define _XSKINNEDFRAME_H_

#include "xframewnd.h"
#include "xskinnedwnd.h"
#include "xskinnedmsgbox.h"

#define XDEFAULTSKINNEDWNDCLASSNAME _T("XSkinnedFrameClass")
#define XDEFAULTSKINNEDSTYLE WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SIZEBOX
#define XDEFAULTSKINNEDSTYLEEX 0

class XSkinnedFrame:public XFrameWnd,XSkinnedWnd{
  DECLARE_DYNAMIC(XSkinnedFrame)
  public:
    XSkinnedFrame(HWND hWnd=NULL):XFrameWnd(hWnd),XSkinnedWnd(this){
      m_SMB=new XSkinnedMsgBox();
    }

    virtual ~XSkinnedFrame(){
      delete m_SMB;
    }

    virtual HWND Create(LPCTSTR lpszWindowName,LPCTSTR lpszClassName=XDEFAULTSKINNEDWNDCLASSNAME,DWORD dwStyle=XDEFAULTSKINNEDSTYLE,
      const RECT& rect=XDEFAULTRECT,HWND hParentWnd=NULL,LPCTSTR lpszMenu=NULL,DWORD dwExStyle=XDEFAULTSKINNEDSTYLEEX,LPVOID lpParam=NULL){
      return XFrameWnd::Create(lpszWindowName,lpszClassName,dwStyle,rect,hParentWnd,lpszMenu,dwExStyle,lpParam);
    }

    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(LPMSG lpMsg);

    BOOL OnSetText(LPSTR swText);
    HICON OnSetIcon(BOOL bBig,HICON hIcon);
    VOID OnPrint(HDC hDC,UINT dwFlag);
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
    BOOL OnEraseBkgnd(HDC hDC);

    VOID OnInitMenuPopup(HMENU hMenu,DWORD info);

    INT MessageBox(LPCTSTR lpText,LPCTSTR lpCaption=NULL,UINT uType=0,UINT uTimeout=INFINITE);

  protected:
    XSkinnedMsgBox *m_SMB;

  DECLARE_EVENT_TABLE()
};

#endif //_XSKINNEDFRAME_H_
