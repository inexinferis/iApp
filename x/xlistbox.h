#ifndef _XLISTBOX_H_
#define _XLISTBOX_H_

#include "xwnd.h"

#define XDEFLISTBOXSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XListBox:public XWnd{
  DECLARE_DYNAMIC(XListBox)
  public:
    XListBox(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XListBox(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTBOXSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_LISTBOX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTBOXSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_LISTBOX,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    INT GetCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETCOUNT,0,0);
    }

    INT GetCurSel()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETCURSEL,0,0);
    }

    INT SetCurSel(INT nSelect){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETCURSEL,nSelect,0);
    }

    INT GetHorizontalExtent()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETHORIZONTALEXTENT,0,0);
    }

    VOID SetHorizontalExtent(INT cxExtent){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LB_SETHORIZONTALEXTENT,cxExtent,0);
    }

    INT GetSelCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETSELCOUNT,0,0);
    }

    INT GetSelItems(INT nMaxItems,LPINT rgIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETSELITEMS,nMaxItems,(LPARAM)rgIndex);
    }

    INT GetTopIndex()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETTOPINDEX,0,0);
    }

    INT SetTopIndex(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETTOPINDEX,nIndex,0);
    }

    DWORD_PTR GetItemData(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SendMessage(m_hWnd,LB_GETITEMDATA,nIndex,0);
    }

    INT SetItemData(INT nIndex,DWORD_PTR dwItemData){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETITEMDATA,nIndex,(LPARAM)dwItemData);
    }

    LPVOID GetItemDataPtr(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (LPVOID)::SendMessage(m_hWnd,LB_GETITEMDATA,nIndex,0);
    }

    INT SetItemDataPtr(INT nIndex,LPVOID pData){
      XASSERT(::IsWindow(m_hWnd))
      return SetItemData(nIndex,(DWORD_PTR)(LPVOID)pData);
    }

    INT GetItemRect(INT nIndex,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETITEMRECT,nIndex,(LPARAM)lpRect);
    }

    INT GetSel(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETSEL,nIndex,0);
    }

    INT SetSel(INT nIndex,BOOL bSelect=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETSEL,bSelect,nIndex);
    }

    INT GetText(INT nIndex,LPTSTR lpszBuffer)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETTEXT,nIndex,(LPARAM)lpszBuffer);
    }

    INT GetTextLen(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETTEXTLEN,nIndex,0);
    }

    VOID SetColumnWidth(INT cxWidth){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LB_SETCOLUMNWIDTH,cxWidth,0);
    }

    BOOL SetTabStops(INT nTabStops,LPINT rgTabStops){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LB_SETTABSTOPS,nTabStops,(LPARAM)rgTabStops);
    }

    VOID SetTabStops(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LB_SETTABSTOPS,0,0);
    }

    BOOL SetTabStops(LPINT rgTabStops){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LB_SETTABSTOPS,1,(LPARAM)rgTabStops);
    }

    INT SetItemHeight(INT nIndex,UINT cyItemHeight){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETITEMHEIGHT,nIndex,MAKELONG(cyItemHeight,0));
    }

    INT GetItemHeight(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETITEMHEIGHT,nIndex,0);
    }

    INT FindStringExact(INT nIndexStart,LPCTSTR lpszFind)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_FINDSTRINGEXACT,nIndexStart,(LPARAM)lpszFind);
    }

    INT GetCaretIndex()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETCARETINDEX,0,0);
    }

    INT SetCaretIndex(INT nIndex,BOOL bScroll=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SETCARETINDEX,nIndex,MAKELONG(bScroll,0));
    }

    INT AddString(LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_ADDSTRING,0,(LPARAM)lpszItem);
    }

    INT DeleteString(UINT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_DELETESTRING,nIndex,0);
    }

    INT InsertString(INT nIndex,LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_INSERTSTRING,nIndex,(LPARAM)lpszItem);
    }

    VOID ResetContent(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LB_RESETCONTENT,0,0);
    }

    INT Dir(UINT attr,LPCTSTR lpszWildCard){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_DIR,attr,(LPARAM)lpszWildCard);
    }

    INT FindString(INT nStartAfter,LPCTSTR lpszItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_FINDSTRING,nStartAfter,(LPARAM)lpszItem);
    }

    INT SelectString(INT nStartAfter,LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_SELECTSTRING,nStartAfter,(LPARAM)lpszItem);
    }

    INT SelItemRange(BOOL bSelect,INT nFirstItem,INT nLastItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)(bSelect?::SendMessage(m_hWnd,LB_SELITEMRANGEEX,nFirstItem,nLastItem):
        ::SendMessage(m_hWnd,LB_SELITEMRANGEEX,nLastItem,nFirstItem));
    }

    VOID SetAnchorIndex(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LB_SETANCHORINDEX,nIndex,0);
    }

    INT GetAnchorIndex()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_GETANCHORINDEX,0,0);
    }

    LCID GetLocale()const{
      XASSERT(::IsWindow(m_hWnd))
      return (LCID)::SendMessage(m_hWnd,LB_GETLOCALE,0,0);
    }

    LCID SetLocale(LCID nNewLocale){
      XASSERT(::IsWindow(m_hWnd))
      return (LCID)::SendMessage(m_hWnd,LB_SETLOCALE,(WPARAM)nNewLocale,0);
    }

    INT InitStorage(INT nItems,UINT nBytes){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LB_INITSTORAGE,(WPARAM)nItems,nBytes);
    }

  #if(WINVER >= 0x0500)
    DWORD GetListBoxInfo()const{
      XASSERT(::IsWindow(m_hWnd))
      return ::GetListBoxInfo(m_hWnd);
    }
  #endif

    UINT ItemFromPoint(POINT pt,PBOOL bOutside=NULL)const{
      XASSERT(::IsWindow(m_hWnd))
      DWORD dw=(DWORD)::SendMessage(m_hWnd,LB_ITEMFROMPOINT,0,MAKELPARAM(pt.x,pt.y));
      if(bOutside)
        *bOutside=!!HIWORD(dw);
      return LOWORD(dw);
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XLISTBOX_H_
