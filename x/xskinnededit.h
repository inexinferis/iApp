#ifndef _XSKINNEDEDIT_H_
#define _XSKINNEDEDIT_H_

#include "xedit.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

#define XEDIT_MARGIN    4

class XXXX{
};

class XSkinnedEdit:public XEdit,XXXX{
  DECLARE_DYNAMIC(XSkinnedEdit)
  public:
    XSkinnedEdit(HWND hwnd=NULL);
    virtual ~XSkinnedEdit();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFEDITSTYLE,LPVOID lpParam=NULL){
      // save original style
      m_dwStyle=dwStyle;
      // clear flags...
      dwStyle&=~(WS_BORDER|WS_DLGFRAME);
      return XEdit::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFEDITSTYLE,LPVOID lpParam=NULL){
      // save original style
      m_dwStyle=dwStyle;
      // clear flags...
      dwStyle&=~(WS_BORDER|WS_DLGFRAME);
      return XEdit::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    INT OnCreate(LPCREATESTRUCT pcs);
    INT OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams);
    VOID OnNCPaint(HRGN hRgn);
    VOID OnSetFocus(HWND hWndLost);
    VOID OnKillFocus(HWND hWndRcv);
    HBRUSH OnCtlColorEdit(HDC hDC,HWND hCtrl);

  private:
    DWORD m_dwStyle;
    BOOL m_bActive;
    XBrush m_hBrush;
    XFont m_hFont;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDEDIT_H_
