#ifndef _XSCROLLBAR_H_
#define _XSCROLLBAR_H_

#include "xwnd.h"

#define XDEFSCROLLBARSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XScrollBar:public XWnd{
  DECLARE_DYNAMIC(XScrollBar)
  public:
    XScrollBar(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XScrollBar(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSCROLLBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_SCROLLBAR,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSCROLLBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_SCROLLBAR,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    INT GetScrollPos()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetScrollPos(m_hWnd,SB_CTL);
    }

    INT SetScrollPos(INT nPos,BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetScrollPos(m_hWnd,SB_CTL,nPos,bRedraw);
    }

    VOID GetScrollRange(LPINT lpMinPos,LPINT lpMaxPos)const{
      XASSERT(::IsWindow(m_hWnd))
      ::GetScrollRange(m_hWnd,SB_CTL,lpMinPos,lpMaxPos);
    }

    VOID SetScrollRange(INT nMinPos,INT nMaxPos,BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      ::SetScrollRange(m_hWnd,SB_CTL,nMinPos,nMaxPos,bRedraw);
    }

    VOID ShowScrollBar(BOOL bShow){
      XASSERT(::IsWindow(m_hWnd))
      ::ShowScrollBar(m_hWnd,SB_CTL,bShow);
    }

    BOOL EnableScrollBar(UINT nArrowFlags){
      XASSERT(::IsWindow(m_hWnd))
      return ::EnableScrollBar(m_hWnd,SB_CTL,nArrowFlags);
    }

    BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo,BOOL bRedraw=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return ::SetScrollInfo(m_hWnd,SB_CTL,lpScrollInfo,bRedraw);
    }

    BOOL GetScrollInfo(LPSCROLLINFO lpsi,UINT nMask=SIF_ALL){
      XASSERT(::IsWindow(m_hWnd)&&lpsi!=NULL)
      lpsi->cbSize=sizeof(*lpsi);
      lpsi->fMask=nMask;
      return ::GetScrollInfo(m_hWnd,SB_CTL,lpsi);
    }

    INT GetScrollLimit(){
      XASSERT(::IsWindow(m_hWnd))
      return XWnd::GetScrollLimit(SB_CTL);
    }

  #if(_WIN32_WINNT >= 0x0501)
    BOOL GetScrollBarInfo(PSCROLLBARINFO pScrollInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SBM_GETSCROLLBARINFO,0,(LPARAM)pScrollInfo);
    }
  #endif

  DECLARE_EVENT_TABLE()
};

#endif // _XSCROLLBAR_H_
