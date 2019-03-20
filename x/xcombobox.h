#ifndef _XCOMBOBOX_H_
#define _XCOMBOBOX_H_

#include "xwnd.h"

#define XDEFCOMBOBOXSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XComboBox:public XWnd{
  DECLARE_DYNAMIC(XComboBox)
  public:
    XComboBox(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XComboBox(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_COMBOBOX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFCOMBOBOXSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_COMBOBOX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    INT GetCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETCOUNT,0,0);
    }

    INT GetCurSel()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETCURSEL,0,0);
    }

    INT SetCurSel(INT nSelect){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETCURSEL,nSelect,0);
    }

    DWORD GetEditSel()const{
      XASSERT(::IsWindow(m_hWnd))
      return DWORD(::SendMessage(m_hWnd,CB_GETEDITSEL,0,0));
    }

    BOOL LimitText(INT nMaxChars){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_LIMITTEXT,nMaxChars,0);
    }

    BOOL SetEditSel(INT nStartChar,INT nEndChar){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_SETEDITSEL,0,MAKELONG(nStartChar,nEndChar));
    }

    DWORD_PTR GetItemData(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SendMessage(m_hWnd,CB_GETITEMDATA,nIndex,0);
    }

    INT SetItemData(INT nIndex,DWORD_PTR dwItemData){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETITEMDATA,nIndex,(LPARAM)dwItemData);
    }

    PVOID GetItemDataPtr(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (LPVOID)GetItemData(nIndex);
    }

    INT SetItemDataPtr(INT nIndex,PVOID pData){
      XASSERT(::IsWindow(m_hWnd))
      return SetItemData(nIndex,(DWORD_PTR)(LPVOID)pData);
    }

    INT GetLBText(INT nIndex,LPTSTR lpszText)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETLBTEXT,nIndex,(LPARAM)lpszText);
    }

    INT GetLBTextLen(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETLBTEXTLEN,nIndex,0);
    }

    VOID ShowDropDown(BOOL bShowIt){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,CB_SHOWDROPDOWN,bShowIt,0);
    }

    INT AddString(LPCTSTR lpszString){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_ADDSTRING,0,(LPARAM)lpszString);
    }

    INT DeleteString(UINT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_DELETESTRING,nIndex,0);
    }

    INT InsertString(INT nIndex,LPCTSTR lpszString){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_INSERTSTRING,nIndex,(LPARAM)lpszString);
    }

    VOID ResetContent(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,CB_RESETCONTENT,0,0);
    }

    INT Dir(UINT attr,LPCTSTR lpszWildCard){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_DIR,attr,(LPARAM)lpszWildCard);
    }

    INT FindString(INT nStartAfter,LPCTSTR lpszString)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_FINDSTRING,nStartAfter,(LPARAM)lpszString);
    }

    INT SelectString(INT nStartAfter,LPCTSTR lpszString){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SELECTSTRING,nStartAfter,(LPARAM)lpszString);
    }

    VOID Clear(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_CLEAR,0,0);
    }

    VOID Copy(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_COPY,0,0);
    }

    VOID Cut(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_CUT,0,0);
    }

    VOID Paste(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_PASTE,0,0);
    }

    INT SetItemHeight(INT nIndex,UINT cyItemHeight){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETITEMHEIGHT,nIndex,MAKELONG(cyItemHeight,0));
    }

    INT GetItemHeight(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETITEMHEIGHT,nIndex,0L);
    }

    INT FindStringExact(INT nIndexStart,LPCTSTR lpszFind)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_FINDSTRINGEXACT,nIndexStart,(LPARAM)lpszFind);
    }

    INT SetExtendedUI(BOOL bExtended){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETEXTENDEDUI,bExtended,0L);
    }

    BOOL GetExtendedUI()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_GETEXTENDEDUI,0,0L);
    }

    VOID GetDroppedControlRect(LPRECT lprect)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,CB_GETDROPPEDCONTROLRECT,0,(LPARAM)lprect);
    }

    BOOL GetDroppedState()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_GETDROPPEDSTATE,0,0L);
    }

    LCID GetLocale()const{
      XASSERT(::IsWindow(m_hWnd))
      return (LCID)::SendMessage(m_hWnd,CB_GETLOCALE,0,0);
    }

    LCID SetLocale(LCID nNewLocale){
      XASSERT(::IsWindow(m_hWnd))
      return (LCID)::SendMessage(m_hWnd,CB_SETLOCALE,(WPARAM)nNewLocale,0);
    }

    INT GetTopIndex()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETTOPINDEX,0,0);
    }

    INT SetTopIndex(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETTOPINDEX,nIndex,0);
    }

    INT InitStorage(INT nItems,UINT nBytes){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_INITSTORAGE,(WPARAM)nItems,nBytes);
    }

    VOID SetHorizontalExtent(UINT nExtent){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,CB_SETHORIZONTALEXTENT,nExtent,0);
    }

    UINT GetHorizontalExtent()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,CB_GETHORIZONTALEXTENT,0,0);
    }

    INT SetDroppedWidth(UINT nWidth){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_SETDROPPEDWIDTH,nWidth,0);
    }

    INT GetDroppedWidth()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETDROPPEDWIDTH,0,0);
    }

  #if(WINVER >= 0x0500)

    BOOL GetComboBoxInfo(PCOMBOBOXINFO pcbi)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetComboBoxInfo(m_hWnd, pcbi);
    }

  #endif

  #if _WIN32_WINNT >= 0x501

    BOOL SetMinVisibleItems(INT iMinVisible){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_SETMINVISIBLE,iMinVisible,0);
    }

    INT GetMinVisible()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,CB_GETMINVISIBLE,0,0);
    }

  #endif

  #if _WIN32_WINNT >= 0x600

    #ifdef CB_SETCUEBANNER

    BOOL SetCueBanner(LPCTSTR lpszText){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,CB_SETCUEBANNER,0,(LPARAM)lpszText);
    }

    BOOL GetCueBanner(LPWSTR lpszText,INT cchText)const{
      XASSERT(::IsWindow(m_hWnd)&&(lpszText!=NULL))
      if(cchText==0||lpszText==NULL)
        return FALSE;
      lpszText[0]=0;
      return (BOOL)::SendMessage(m_hWnd,CB_GETCUEBANNER,(WPARAM)lpszText,(LPARAM)cchText);
    }

    #endif

  #endif

  DECLARE_EVENT_TABLE()
};

#endif //_XCOMBOBOX_H_
