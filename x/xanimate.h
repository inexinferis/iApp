#ifndef _XANIMATE_H_
#define _XANIMATE_H_

#include "xwnd.h"

#define XDEFANIMATESTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XAnimate:public XWnd{
  DECLARE_DYNAMIC(XAnimate)
  public:
    XAnimate(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XAnimate(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFANIMATESTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(ANIMATE_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFANIMATESTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,ANIMATE_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    BOOL Open(LPCTSTR lpszName,HINSTANCE hInstance=NULL){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_OPEN,(WPARAM)hInstance,(LPARAM)lpszName);
    }

    BOOL Open(UINT nID,HINSTANCE hInstance=NULL){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_OPEN,(WPARAM)hInstance,MAKELPARAM(nID,0));
    }

    BOOL Play(UINT nFrom,UINT nTo,UINT nRep){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_PLAY,nRep,MAKELPARAM(nFrom,nTo));
    }

    BOOL Stop(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_STOP,0,0);
    }

    BOOL Close(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_OPEN,0,0);
    }

    BOOL Seek(UINT nTo){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_PLAY,0,MAKELPARAM(nTo,nTo));
    }

    #if _WIN32_WINNT >= 0x600
    BOOL IsPlaying()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,ACM_ISPLAYING,0,0);
    }
    #endif

  DECLARE_EVENT_TABLE()
};

#endif //_XANIMATE_H_
