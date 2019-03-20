#ifndef _XSTATIC_H_
#define _XSTATIC_H_

#include "xwnd.h"

#define XDEFSTATICSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XStatic:public XWnd{
  DECLARE_DYNAMIC(XStatic)
  public:
    XStatic(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XStatic(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATICSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_STATIC,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATICSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_STATIC,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HICON SetIcon(HICON hIcon){
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,STM_SETICON,(WPARAM)hIcon,0);
    }

    virtual HICON GetIcon()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,STM_GETICON,0,0);
    }

    virtual HBITMAP SetBitmap(HBITMAP hBitmap){
      XASSERT(::IsWindow(m_hWnd))
      return (HBITMAP)::SendMessage(m_hWnd,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);
    }

    virtual HBITMAP GetBitmap()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HBITMAP)::SendMessage(m_hWnd,STM_GETIMAGE,IMAGE_BITMAP,0);
    }

    virtual HCURSOR SetCursor(HCURSOR hCursor){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,STM_SETIMAGE,IMAGE_CURSOR,(LPARAM)hCursor);
    }

    virtual HCURSOR GetCursor(){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,STM_GETIMAGE,IMAGE_CURSOR,0);
    }

    HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile){
      XASSERT(::IsWindow(m_hWnd))
      return (HENHMETAFILE)::SendMessage(m_hWnd,STM_SETIMAGE,IMAGE_ENHMETAFILE,(LPARAM)hMetaFile);
    }

    HENHMETAFILE GetEnhMetaFile()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HENHMETAFILE)::SendMessage(m_hWnd,STM_GETIMAGE,IMAGE_ENHMETAFILE,0);
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XSTATIC_H_
