#ifndef _XSKINNEDCOMBOBOX_H_
#define _XSKINNEDCOMBOBOX_H_

#include "xcombobox.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

#ifndef SS_VCENTER
  #define SS_VCENTER 0x20
#endif

class XSkinnedComboBox:public XComboBox{
  DECLARE_DYNAMIC(XSkinnedComboBox)
  public:
    XSkinnedComboBox(HWND hwnd=NULL);
    virtual ~XSkinnedComboBox();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXSTYLE,LPVOID lpParam=NULL){
      return XComboBox::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXSTYLE,LPVOID lpParam=NULL){
      return XComboBox::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
    }

    INT OnCreate(LPCREATESTRUCT pcs);

    HBRUSH OnCtlColorEdit(HDC hDC,HWND hCtrl);
    HBRUSH OnCtlColorListBox(HDC hDC,HWND hCtrl);

    VOID OnSetFocus(HWND hWndLost);
    VOID OnKillFocus(HWND hWndRcv);
    VOID OnNCPaint(HRGN hRgn);
    VOID OnPaint();

  private:
    XFont m_hFont;
    XBrush m_hBrush;
    DWORD m_dwStyle;
    BOOL m_bActive;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDCOMBOBOX_H_
