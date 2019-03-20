#ifndef _XCOMBOBOXEX_H_
#define _XCOMBOBOXEX_H_

#include "xcombobox.h"

#define XDEFCOMBOBOXEXSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XComboBoxEx:public XComboBox{
  DECLARE_DYNAMIC(XComboBoxEx)
  public:
    XComboBoxEx(HWND hwnd=NULL):XComboBox(hwnd){}
    virtual ~XComboBoxEx(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXEXSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_COMBOBOXEX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXEXSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_COMBOBOXEX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    DWORD GetExtendedStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL) ::SendMessage(m_hWnd,CBEM_GETEXTENDEDSTYLE,0,0);
    }

    DWORD SetExtendedStyle(DWORD dwExMask,DWORD dwExStyles){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD) ::SendMessage(m_hWnd,CBEM_SETEXTENDEDSTYLE,(DWORD) dwExMask,(LPARAM) dwExStyles);
    }

    BOOL HasEditChanged(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL) ::SendMessage(m_hWnd,CBEM_HASEDITCHANGED,0,0);
    }

    HWND GetEditCtrl()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,CBEM_GETEDITCONTROL,0,0);
    }

    HWND GetComboBoxCtrl()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,CBEM_GETCOMBOCONTROL,0,0);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,CBEM_SETIMAGELIST,0,(LPARAM)hImageList);
    }

    HIMAGELIST GetImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,CBEM_GETIMAGELIST,0,0);
    }

    INT Dir(UINT /*attr*/,LPCTSTR /*lpszWildCard*/){
      XASSERT(FALSE)
      return CB_ERR;
    }

    INT FindString(INT /*nIndexStart*/,LPCTSTR /*lpszFind*/)const{
      XASSERT(FALSE)
      return CB_ERR;
    }

    INT AddString(LPCTSTR /*lpszString*/){
      XASSERT(FALSE)
      return CB_ERR;
    }

    BOOL SetEditSel(INT /*nStartChar*/,INT /*nEndChar*/){
      XASSERT(FALSE)
      return FALSE;
    }

    INT InsertString(INT /*nIndex*/,LPCTSTR /*lpszString*/){
      XASSERT(FALSE)
      return CB_ERR;
    }

  DECLARE_EVENT_TABLE()
};

#endif //_XCOMBOBOXEX_H_

