#ifndef _XTABCONTROL_H_
#define _XTABCONTROL_H_

#include "xwnd.h"

#define XDEFTABCONTROLSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XTabCtrl: public XWnd{
  DECLARE_DYNAMIC(XTabCtrl)
  public:
    XTabCtrl(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XTabCtrl(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTABCONTROLSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_TABCONTROL,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTABCONTROLSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_TABCONTROL,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    DWORD GetItemState(INT nItem,DWORD dwMask)const{
      XASSERT(::IsWindow(m_hWnd))
      TCITEM item;item.mask=TCIF_STATE;item.dwStateMask=dwMask;
      ::SendMessage(m_hWnd,TCM_GETITEM,nItem,(LPARAM)&item);
      return item.dwState;
    }

    BOOL SetItemState(INT nItem,DWORD dwMask,DWORD dwState){
      XASSERT(::IsWindow(m_hWnd))
      TCITEM item;item.mask=TCIF_STATE;item.dwState=dwState;item.dwStateMask = dwMask;
      return (BOOL)::SendMessage(m_hWnd,TCM_SETITEM,nItem,(LPARAM)&item);
    }

    LONG InsertItem(UINT nMask,INT nItem,LPCTSTR lpszItem,INT nImage,LPARAM lParam){
      XASSERT(::IsWindow(m_hWnd))
      TCITEM item;item.mask=nMask;item.iImage=nImage;item.lParam=lParam;item.pszText=(LPTSTR)lpszItem;
      return (LONG)::SendMessage(m_hWnd,TCM_INSERTITEM,nItem,(LPARAM) &item);
    }

    LONG InsertItem(UINT nMask,INT nItem,LPCTSTR lpszItem,INT nImage,LPARAM lParam,DWORD dwState,DWORD dwStateMask){
      XASSERT(::IsWindow(m_hWnd))
      TCITEM item;item.mask=nMask;item.iImage=nImage;item.lParam=lParam;
      item.pszText=(LPTSTR)lpszItem;item.dwState=dwState;item.dwStateMask=dwStateMask;
      return (LONG)::SendMessage(m_hWnd,TCM_INSERTITEM,nItem,(LPARAM) &item);
    }

    HIMAGELIST GetImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TCM_GETIMAGELIST,0,0);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TCM_SETIMAGELIST,0,(LPARAM)hImageList);
    }

    INT GetItemCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_GETITEMCOUNT,0,0);
    }

    BOOL GetItem(INT nItem,TCITEM* pTabCtrlItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_GETITEM,nItem,(LPARAM)pTabCtrlItem);
    }

    BOOL SetItem(INT nItem,TCITEM* pTabCtrlItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_SETITEM,nItem,(LPARAM)pTabCtrlItem);
    }

    BOOL SetItemExtra(INT nBytes){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_SETITEMEXTRA,(WPARAM) nBytes,0);
    }

    VOID SetCurFocus(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_SETCURFOCUS,(WPARAM) nItem,0);
    }

    LONG InsertItem(INT nItem,TCITEM* pTabCtrlItem){
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,TCM_INSERTITEM,nItem,(LPARAM)pTabCtrlItem);
    }

    LONG InsertItem(INT nItem,LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(TCIF_TEXT,nItem,lpszItem,0,0);
    }

    LONG InsertItem(INT nItem,LPCTSTR lpszItem,INT nImage){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(TCIF_TEXT|TCIF_IMAGE,nItem,lpszItem,nImage,0);
    }

    BOOL DeleteItem(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_DELETEITEM,nItem,0);
    }

    BOOL DeleteAllItems(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_DELETEALLITEMS,0,0);
    }

    BOOL GetItemRect(INT nItem,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_GETITEMRECT,nItem,(LPARAM)lpRect);
    }

    INT GetCurSel()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_GETCURSEL,0,0);
    }

    INT SetCurSel(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_SETCURSEL,nItem,0);
    }

    INT HitTest(TCHITTESTINFO* pHitTestInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_HITTEST,0,(LPARAM) pHitTestInfo);
    }

    VOID AdjustRect(BOOL bLarger,LPRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_ADJUSTRECT,bLarger,(LPARAM)lpRect);
    }

    DWORD SetItemSize(SIZE size){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TCM_SETITEMSIZE,0,MAKELPARAM(size.cx,size.cy));
    }

    VOID RemoveImage(INT nImage){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_REMOVEIMAGE,nImage,0);
    }

    VOID SetPadding(SIZE size){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_SETPADDING,0,MAKELPARAM(size.cx,size.cy));
    }

    INT GetRowCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_GETROWCOUNT,0,0);
    }

    HWND GetToolTips()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TCM_GETTOOLTIPS,0,0);
    }

    VOID SetToolTips(HWND hWndTip){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_SETTOOLTIPS,(WPARAM)hWndTip,0);
    }

    INT GetCurFocus()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_GETCURFOCUS,0,0);
    }

    INT SetMinTabWidth(INT cx){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TCM_SETMINTABWIDTH,0,(LPARAM)cx);
    }

    VOID DeselectAll(BOOL fExcludeFocus){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TCM_DESELECTALL,fExcludeFocus,0);
    }

    BOOL HighlightItem(INT idItem,BOOL fHighlight=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TCM_HIGHLIGHTITEM,idItem,MAKELPARAM(fHighlight,0));
    }

    DWORD GetExtendedStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TCM_GETEXTENDEDSTYLE,0,0);
    }

    DWORD SetExtendedStyle(DWORD dwNewStyle,DWORD dwExMask=0){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD) ::SendMessage(m_hWnd,TCM_SETEXTENDEDSTYLE,dwExMask,dwNewStyle);
    }

  DECLARE_EVENT_TABLE()
};

#endif //_XTABCONTROL_H_
