#ifndef _XMDICLIENT_H_
#define _XMDICLIENT_H_

#include "xwnd.h"

#define XDEFMDICLIENTSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|MDIS_ALLCHILDSTYLES
#define XDEFMDICLIENTSTYLEEX 0

class XMDIClient:public XWnd{
  DECLARE_DYNCREATE(XMDIClient)
  public:
    XMDIClient(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XMDIClient(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFMDICLIENTSTYLE,LPCLIENTCREATESTRUCT lpCCS=NULL){
      return XWnd::Create(_T("mdiclient"),lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,(LPVOID)lpCCS);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFMDICLIENTSTYLE,LPCLIENTCREATESTRUCT lpCCS=NULL){
      return XWnd::CreateEx(dwStyleEx,_T("mdiclient"),lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,(LPVOID)lpCCS);
    }

    HWND MDICreate(LPMDICREATESTRUCT lpMDICS){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,WM_MDICREATE,0,(LPARAM)lpMDICS);
    }

    VOID MDIDestroy(HWND hWndDestroy){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDIDESTROY,(WPARAM)hWndDestroy,0);
    }

    VOID MDIActivate(HWND hWndActivate){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDIACTIVATE,(WPARAM)hWndActivate,0);
    }

    VOID MDIIconArrange(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDIICONARRANGE,0,0);
    }

    VOID MDIMaximize(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDIMAXIMIZE,(WPARAM)hWnd,0);
    }

    VOID MDIPrev(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDINEXT,0,1);
    }

    VOID MDINext(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDINEXT,0,0);
    }

    VOID MDIRestore(HWND hWnd){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDIRESTORE,(WPARAM)hWnd,0);
    }

    HMENU MDISetMenu(HMENU hFrameMenu,HMENU hWindowMenu){
      XASSERT(::IsWindow(m_hWnd))
      return (HMENU)::SendMessage(m_hWnd,WM_MDISETMENU,(WPARAM)hFrameMenu,(LPARAM)hWindowMenu);
    }

    HMENU MDIRefreshMenu(){
      XASSERT(::IsWindow(m_hWnd))
      return (HMENU)::SendMessage(m_hWnd,WM_MDIREFRESHMENU,0,0);
    }

    VOID MDITile(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDITILE,0,0);
    }

    VOID MDICascade(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDICASCADE,0,0);
    }

    VOID MDICascade(INT nType){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDICASCADE,nType,0);
    }

    VOID MDITile(INT nType){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_MDITILE,nType,0);
    }

    HWND MDIGetActive(PBOOL pbMaximized)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,WM_MDIGETACTIVE,0,(LPARAM)pbMaximized);
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XMDICLIENT_H_
