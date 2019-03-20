#ifndef _XMDICHILDWND_H_
#define _XMDICHILDWND_H_

#include "xmdiframewnd.h"

#define XDEFMDICHILDWNDSTYLE WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW
#define XDEFMDICHILDWNDSTYLEEX 0

#define XDEFMDICHILDWNDCLASSNAME _T("XMDIChildWndClass")

class XMDIChildWnd:public XFrameWnd{
  DECLARE_DYNCREATE(XMDIChildWnd)
  public:
    XMDIChildWnd(HWND hWnd=NULL):XFrameWnd(hWnd),m_hParentWnd(NULL){}
    virtual ~XMDIChildWnd(){}

    virtual HWND Create(HWND hParentWnd,LPCTSTR lpszWindowName,LPCTSTR lpszClassName=XDEFMDICHILDWNDCLASSNAME,
      DWORD dwStyle=XDEFMDICHILDWNDSTYLE,const RECT& rect=XDEFAULTRECT,LPCTSTR lpszMenu=NULL,DWORD dwExStyle=XDEFMDICHILDWNDSTYLEEX,LPVOID lpParam=NULL);

    HWND LoadFrame(HWND hParentWnd,UINT uResID,DWORD dwStyle=XDEFAULTFRAMEWNDSTYLE,LPVOID lpParam=NULL);

    VOID MDIDestroy(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(::GetParent(m_hWnd),WM_MDIDESTROY,(WPARAM)m_hWnd,0);
    }

    VOID MDIActivate(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(::GetParent(m_hWnd),WM_MDIACTIVATE,(WPARAM)m_hWnd,0);
    }

    VOID MDIMaximize(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(::GetParent(m_hWnd),WM_MDIMAXIMIZE,(WPARAM)m_hWnd,0);
    }

    VOID MDIRestore(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(::GetParent(m_hWnd),WM_MDIRESTORE,(WPARAM)m_hWnd,0);
    }

    HWND MDIGetFrame(){
      XASSERT(::IsWindow(m_hWnd))
      HWND hWndMDIClient=::GetParent(m_hWnd);
      XASSERT(hWndMDIClient!=NULL)
      HWND hMDIFrame=::GetParent(hWndMDIClient);
      XASSERT(hMDIFrame!=NULL)
      return hMDIFrame;
    }

    BOOL DestroyWindow();

    virtual VOID RecalcLayout();

  protected:

    virtual BOOL CreateClient(LPCREATESTRUCT lpcs);
    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(LPMSG lpMsg);

    virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

  private:

    HWND m_hParentWnd;

  DECLARE_EVENT_TABLE()
};

#endif // _XMDICHILDWND_H_
