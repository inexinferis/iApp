#ifndef _XMDIFRAMEWND_H_
#define _XMDIFRAMEWND_H_

#include "xframewnd.h"

class XMDIFrameWnd:public XFrameWnd{
  DECLARE_DYNCREATE(XMDIFrameWnd)
  public:
    XMDIFrameWnd(HWND hWnd=NULL):XFrameWnd(hWnd),m_hWndMDIClient(NULL),m_hDefaultMenu(NULL){}
    virtual ~XMDIFrameWnd(){}

    HWND LoadFrame(UINT uResID,DWORD dwStyle=XDEFAULTFRAMEWNDSTYLE,HWND hParentWnd=NULL,LPVOID lpParam=NULL);

    HWND MDICreate(LPMDICREATESTRUCT lpMDICS){
      XASSERT(::IsWindow(m_hWnd))
        return (HWND)::SendMessage(m_hWndMDIClient,WM_MDICREATE,0,(LPARAM)lpMDICS);
      return NULL;
    }

    VOID MDIDestroy(HWND hWndDestroy){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDIDESTROY,(WPARAM)hWndDestroy,0);
    }

    VOID MDIActivate(HWND hWndActivate){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDIACTIVATE,(WPARAM)hWndActivate,0);
    }

    VOID MDIIconArrange(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDIICONARRANGE,0,0);
    }

    VOID MDIMaximize(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDIMAXIMIZE,(WPARAM)hWnd,0);
    }

    VOID MDIPrev(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDINEXT,0,1);
    }

    VOID MDINext(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDINEXT,0,0);
    }

    VOID MDIRestore(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDIRESTORE,(WPARAM)hWnd,0);
    }

    HMENU MDISetMenu(HMENU hFrameMenu,HMENU hWindowMenu){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        return (HMENU)::SendMessage(m_hWndMDIClient,WM_MDISETMENU,(WPARAM)hFrameMenu,(LPARAM)hWindowMenu);
      return NULL;
    }

    HMENU MDIRefreshMenu(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        return (HMENU)::SendMessage(m_hWndMDIClient,WM_MDIREFRESHMENU,0,0);
      return NULL;
    }

    VOID MDITile(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDITILE,0,0);
    }

    VOID MDICascade(){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDICASCADE,0,0);
    }

    VOID MDICascade(INT nType){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDICASCADE,nType,0);
    }

    VOID MDITile(INT nType){
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        ::SendMessage(m_hWndMDIClient,WM_MDITILE,nType,0);
    }

    HWND MDIGetActive(PBOOL pbMaximized=NULL)const{
      XASSERT(::IsWindow(m_hWnd))
      if(::IsWindow(m_hWndMDIClient))
        return (HWND)::SendMessage(m_hWndMDIClient,WM_MDIGETACTIVE,0,(LPARAM)pbMaximized);
      return NULL;
    }

    HWND GetActiveFrame(){
      HWND hActiveChild=MDIGetActive();
      if(hActiveChild==NULL)
        return m_hWnd;
      return hActiveChild;
    }

    HMENU GetWindowMenuPopup(HMENU hMenuBar);

    //internal helper
    virtual VOID RecalcLayout();

  protected:

    virtual BOOL CreateClient(LPCREATESTRUCT lpcs);
    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(LPMSG lpMsg);

    virtual LRESULT OnCommand(WPARAM wParam,LPARAM lParam);
    virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

  public:
    HWND m_hWndMDIClient;
    HMENU m_hDefaultMenu;

  DECLARE_EVENT_TABLE()
};

#endif // _XMDIFRAMEWND_H_
