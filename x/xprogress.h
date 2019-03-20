#ifndef _XPROGRESS_H_
#define _XPROGRESS_H_

#include "xwnd.h"

#define XDEFPROGRESSSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XProgress:public XWnd{
  DECLARE_DYNAMIC(XProgress)
  public:
    XProgress(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XProgress(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFPROGRESSSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(PROGRESS_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFPROGRESSSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,PROGRESS_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    COLORREF SetBkColor(COLORREF clrNew){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,PBM_SETBKCOLOR,0,(LPARAM)clrNew);
    }

    VOID SetRange(SHORT nLower,SHORT nUpper){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,PBM_SETRANGE,0,MAKELPARAM(nLower,nUpper));
    }

    VOID SetRange32(INT nLower,INT nUpper){
      XASSERT(::IsWindow(m_hWnd))
     ::SendMessage(m_hWnd,PBM_SETRANGE32,nLower,(LPARAM)nUpper);
    }

    VOID GetRange(INT &nLower,INT &nUpper)const{
      XASSERT(::IsWindow(m_hWnd))
      PBRANGE range;
      ::SendMessage(m_hWnd,PBM_GETRANGE,(WPARAM)FALSE,(LPARAM)&range);
      nLower=range.iLow;
      nUpper=range.iHigh;
      return;
    }

    INT GetPos()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_GETPOS,0,0);
    }

    INT OffsetPos(INT nPos){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_DELTAPOS,nPos,0);
    }

    INT SetStep(INT nStep){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_SETSTEP,nStep,0);
    }

    INT StepIt(){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_STEPIT,0,0);
    }

    INT SetPos(INT nPos){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_SETPOS,nPos,0);
    }

  #if _WIN32_IE >= 0x0400
    COLORREF SetBarColor(COLORREF clrBar){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,PBM_SETBARCOLOR,0,clrBar);
    }
  #endif

  #if _WIN32_WINNT >= 0x0501
    BOOL SetMarquee(BOOL fMarqueeMode,INT nInterval){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,PBM_SETMARQUEE,(WPARAM)fMarqueeMode,(LPARAM)nInterval);
    }
  #endif

  #if _WIN32_WINNT >= 0x0600
    INT GetStep()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_GETSTEP,0,0);
    }

    COLORREF GetBkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,PBM_GETBKCOLOR,0,0);
    }

    COLORREF GetBarColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,PBM_GETBARCOLOR,0,0);
    }

    INT SetState(INT iState){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_SETSTATE,(WPARAM)iState,0);
    }

    INT GetState()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,PBM_GETSTATE,0,0);
    }
  #endif

  DECLARE_EVENT_TABLE()
};

#endif // _XPROGRESS_H_
