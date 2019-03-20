#ifndef _XSKYNNEDBUTTON_H_
#define _XSKYNNEDBUTTON_H_

#include "xbutton.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

class XSkinnedButton:public XButton{
  DECLARE_DYNAMIC(XSkinnedButton)
  public:
    XSkinnedButton(HWND hwnd=NULL);
    virtual ~XSkinnedButton();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFBUTTONSTYLE,LPVOID lpParam=NULL){
      // the button should not be owner draw
      XASSERT((dwStyle&SS_TYPEMASK)!=BS_OWNERDRAW)
      // save original style
      m_dwStyle=dwStyle;
      // clear flags, set owner draw...
      dwStyle=(dwStyle&~SS_TYPEMASK)|BS_OWNERDRAW;
      return XButton::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFBUTTONSTYLE,LPVOID lpParam=NULL){
      // the button should not be owner draw
      XASSERT((dwStyle&SS_TYPEMASK)!=BS_OWNERDRAW)
      // save original style
      m_dwStyle=dwStyle;
      // clear flags, set owner draw...
      dwStyle=(dwStyle&~SS_TYPEMASK)|BS_OWNERDRAW;
      return XButton::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    VOID OnMouseMove(DWORD dwVKState,POINTS pt);
    VOID OnMouseHover(DWORD dwVKState,POINTS pt);
    VOID OnMouseLeave();
    BOOL OnDrawItem(UINT uCtrlID,PDRAWITEMSTRUCT pDIS);
    VOID OnClick(WPARAM wParam,LPARAM lParam);
    INT OnSetCheck(DWORD dwChecked);
    INT OnGetCheck();
    INT OnEraseBkgnd(HDC hDC);
    LRESULT OnNCHitTest(POINTS pt);

  private:
    DWORD m_dwStyle,m_dwState;
    BOOL m_bMouseOver;
    XFont m_hFont;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKYNNEDBUTTON_H_
