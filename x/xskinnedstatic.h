#ifndef _XSKINNEDSTATIC_H_
#define _XSKINNEDSTATIC_H_

#include "xstatic.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

#ifndef SS_VCENTER
  #define SS_VCENTER 0x20
#endif

class XSkinnedStatic:public XStatic{
  DECLARE_DYNAMIC(XSkinnedStatic)
  public:
    XSkinnedStatic(HWND hwnd=NULL);
    virtual ~XSkinnedStatic();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATICSTYLE,LPVOID lpParam=NULL){
      // the button should not be owner draw
      XASSERT((dwStyle&SS_TYPEMASK)!=SS_OWNERDRAW)
      // save original style
      m_dwStyle=dwStyle;m_hClient=(XWnd*)lpParam;
      // clear flags, set owner draw...
      dwStyle=(dwStyle&~SS_TYPEMASK)|SS_OWNERDRAW;
      return XStatic::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFSTATICSTYLE,LPVOID lpParam=NULL){
      // the button should not be owner draw
      XASSERT((dwStyle&SS_TYPEMASK)!=SS_OWNERDRAW)
      // save original style
      m_dwStyle=dwStyle;m_hClient=(XWnd*)lpParam;
      // clear flags, set owner draw...
      dwStyle=(dwStyle&~SS_TYPEMASK)|SS_OWNERDRAW;
      return XStatic::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    virtual HBITMAP SetBitmap(HBITMAP hBitmap){
      XASSERT(::IsWindow(m_hWnd))
      HBITMAP hOldBmp=m_hBmp;
      m_hBmp=hBitmap;
      return hOldBmp;
    }

    virtual HBITMAP GetBitmap()const{
      XASSERT(::IsWindow(m_hWnd))
      return m_hBmp;
    }

    virtual HICON SetIcon(HICON hIcon){
      XASSERT(::IsWindow(m_hWnd))
      HICON hOldIcon=m_hIcon;
      m_hIcon=hIcon;
      return hOldIcon;
    }

    virtual HICON GetIcon()const{
      XASSERT(::IsWindow(m_hWnd))
      return m_hIcon;
    }

    VOID SetFontSize(UINT fs);

    BOOL OnSetText(LPSTR swText);
    BOOL OnDrawItem(UINT uCtrlID,PDRAWITEMSTRUCT pDIS);
    VOID OnClicked(DWORD dwParams,HWND hWnd);

  private:
    DWORD m_dwStyle;
    HICON m_hIcon;
    HBITMAP m_hBmp;
    XFont m_hFont;
    XWnd *m_hClient;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDSTATIC_H_
