#ifndef _XSTATUSBAR_H_
#define _XSTATUSBAR_H_

#include "xwnd.h"

#define XDEFSTATUSBARSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XStatusBar:public XWnd{
  DECLARE_DYNAMIC(XStatusBar)
  public:
    XStatusBar(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XStatusBar(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATUSBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(STATUSCLASSNAME,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATUSBARSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,STATUSCLASSNAME,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    BOOL SetText(LPCTSTR lpszText,INT nPane=0,INT nType=0){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_SETTEXT,(nPane|nType),(LPARAM)lpszText);
    }

    BOOL SetParts(INT nParts,PINT pWidths){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_SETPARTS,nParts,(LPARAM)pWidths);
    }

    INT GetParts(INT nParts,PINT pParts)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,SB_GETPARTS,nParts,(LPARAM)pParts);
    }

    BOOL GetBorders(PINT pBorders)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_GETBORDERS,0,(LPARAM)pBorders);
    }

    VOID SetMinHeight(INT nMin){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,SB_SETMINHEIGHT,nMin,0);
    }

    BOOL SetSimple(BOOL bSimple=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_SIMPLE,bSimple,0);
    }

    BOOL GetRect(INT nPane,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_GETRECT,nPane,(LPARAM)lpRect);
    }

    BOOL IsSimple()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_ISSIMPLE,0,0);
    }

    VOID SetTipText(INT nPane,LPCTSTR pszTipText){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,SB_SETTIPTEXT,nPane,(LPARAM)pszTipText);
    }

    COLORREF SetBkColor(COLORREF cr){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,SB_SETBKCOLOR,0,(LPARAM)cr);
    }

    BOOL SetIcon(INT iPart,HICON hIcon){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,SB_SETICON,iPart,(LPARAM)hIcon);
    }

    HICON GetIcon(INT iPart)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,SB_GETICON,iPart,0);
    }

    INT GetText(LPTSTR lpszText,INT nPane,PINT pType=NULL)const{
      XASSERT(::IsWindow(m_hWnd))
      LRESULT dw=::SendMessage(m_hWnd,SB_GETTEXT,(WPARAM)nPane,(LPARAM)lpszText);
      if(pType!=NULL)*pType=HIWORD(dw);
      return LOWORD(dw);
    }

    INT GetTextLength(INT nPane,PINT pType=NULL)const{
      XASSERT(::IsWindow(m_hWnd))
      LRESULT dw=::SendMessage(m_hWnd,SB_GETTEXTLENGTH,(WPARAM)nPane,0);
      if(pType!=NULL)*pType=HIWORD(dw);
      return LOWORD(dw);
    }

    VOID GetTipText(INT nPane,PCHAR pBuf,UINT ccNum)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd, SB_GETTIPTEXT,MAKEWPARAM(nPane,ccNum),(LPARAM)pBuf);
    }

    BOOL GetBorders(PINT pHorz,PINT pVert,PINT pSpacing)const{
      XASSERT(::IsWindow(m_hWnd))
      INT borders[3];
      if(::SendMessage(m_hWnd,SB_GETBORDERS,0,(LPARAM)borders)){
        if(pHorz)
          *pHorz=borders[0];
        if(pVert)
          *pVert=borders[1];
        if(pSpacing)
          *pSpacing=borders[2];
        return TRUE;
      }
      return FALSE;
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XSTATUSBAR_H_
