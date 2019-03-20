#ifndef _XTREEVIEW_H_
#define _XTREEVIEW_H_

#include "xwnd.h"

#define XDEFTREEVIEWSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XTreeView: public XWnd{
  DECLARE_DYNAMIC(XTreeView)
  public:
    XTreeView(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XTreeView(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTREEVIEWSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_TREEVIEW,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTREEVIEWSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_TREEVIEW,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct){
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_INSERTITEM,0,(LPARAM)lpInsertStruct);
    }

    HTREEITEM InsertItem(LPCTSTR lpszItem,INT nImage,INT nSelectedImage=0,HTREEITEM hParent=NULL,HTREEITEM hInsertAfter=NULL,LPARAM lParam=0){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM,lpszItem,nImage,nSelectedImage,0,0,lParam,hParent,hInsertAfter);
    }

    HTREEITEM InsertItem(LPCTSTR lpszItem,HTREEITEM hParent=NULL,HTREEITEM hInsertAfter=NULL,LPARAM lParam=0){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(TVIF_TEXT|TVIF_PARAM,lpszItem,0,0,0,0,lParam,hParent,hInsertAfter);
    }

    BOOL DeleteItem(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_DELETEITEM,0,(LPARAM)hItem);
    }

    BOOL DeleteAllItems(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_DELETEITEM,0,(LPARAM)TVI_ROOT);
    }

    UINT DeleteChildrenItems(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      UINT nCount=0;
      HTREEITEM hChild=(HTREEITEM)GetChildItem(hItem);
      while(hChild!=NULL){
        HTREEITEM hNextItem=(HTREEITEM)GetNextSiblingItem(hChild);
        DeleteItem(hChild);
        hChild=hNextItem;
        nCount++;
      }
      return nCount;
    }

    BOOL Expand(HTREEITEM hItem,UINT nCode=TVE_EXPAND){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_EXPAND,nCode,(LPARAM)hItem);
    }

    UINT GetCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_GETCOUNT,0,0);
    }

    UINT GetIndent()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_GETINDENT,0,0);
    }

    VOID SetIndent(UINT nIndent){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TVM_SETINDENT,nIndent,0);
    }

    HIMAGELIST GetImageList(INT nImageList)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TVM_GETIMAGELIST,(WPARAM)nImageList,0);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList,INT nImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TVM_SETIMAGELIST,(WPARAM)nImageList,(LPARAM)hImageList);
    }

    VOID RemoveImageList(INT nImageList){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TVM_SETIMAGELIST,(WPARAM)nImageList,0);
    }

    HIMAGELIST CreateDragImage(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,TVM_CREATEDRAGIMAGE,0,(LPARAM)hItem);
    }

    UINT SetScrollTime(UINT uScrollTime){
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_SETSCROLLTIME,(WPARAM) uScrollTime,0);
    }

    //Not Defined in Mingw WinAPI 3.17
    #ifdef TVM_SETBORDER
    INT SetBorder(DWORD dwFlags,SHORT xBorder,SHORT yBorder){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,TVM_SETBORDER,dwFlags,MAKELPARAM(xBorder,yBorder));
    }
    #endif

    HTREEITEM GetNextItem(HTREEITEM hItem,UINT nCode)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,nCode,(LPARAM)hItem);
    }

    BOOL GetISearchString(LPTSTR lpsz){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_GETISEARCHSTRING,0,(LPARAM)lpsz);
    }

    BOOL GetUnicodeFormat()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_GETUNICODEFORMAT,0,0);
    }

    BOOL SetUnicodeFormat(BOOL fUnicode){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SETUNICODEFORMAT,fUnicode,0);
    }

    HTREEITEM GetChildItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hItem);
    }

    HTREEITEM GetNextSiblingItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)hItem);
    }

    HTREEITEM GetPrevSiblingItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_PREVIOUS,(LPARAM)hItem);
    }

    HTREEITEM GetParentItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_PARENT,(LPARAM)hItem);
    }

    HTREEITEM GetFirstVisibleItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_FIRSTVISIBLE,0);
    }

    HTREEITEM GetNextVisibleItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_NEXTVISIBLE,(LPARAM)hItem);
    }

    HTREEITEM GetPrevVisibleItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_PREVIOUSVISIBLE,(LPARAM)hItem);
    }

    HTREEITEM GetLastVisibleItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_LASTVISIBLE,0);
    }

    HTREEITEM GetSelectedItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_CARET,0);
    }

    UINT GetScrollTime()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_GETSCROLLTIME,0,0);
    }

    HTREEITEM GetDropHilightItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_DROPHILITE,0);
    }

    HTREEITEM GetRootItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_ROOT,0);
    }

    BOOL Select(HTREEITEM hItem,UINT nCode){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SELECTITEM,nCode,(LPARAM)hItem);
    }

    BOOL SelectItem(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SELECTITEM,TVGN_CARET,(LPARAM)hItem);
    }

    BOOL SelectDropTarget(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SELECTITEM,TVGN_DROPHILITE,(LPARAM)hItem);
    }

    BOOL SelectSetFirstVisible(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SELECTITEM,TVGN_FIRSTVISIBLE,(LPARAM)hItem);
    }

    BOOL GetItem(TVITEM* pItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)pItem);
    }

    BOOL SetItem(TVITEM* pItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SETITEM,0,(LPARAM)pItem);
    }

    BOOL SetItemText(HTREEITEM hItem,LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return SetItem(hItem,TVIF_TEXT,lpszItem,0,0,0,0,0);
    }

    BOOL SetItemImage(HTREEITEM hItem,INT nImage,INT nSelectedImage){
      XASSERT(::IsWindow(m_hWnd))
      return SetItem(hItem,TVIF_IMAGE|TVIF_SELECTEDIMAGE,NULL,nImage,nSelectedImage,0,0,0);
    }

    BOOL SetItemState(HTREEITEM hItem,UINT nState,UINT nStateMask){
      XASSERT(::IsWindow(m_hWnd))
      return SetItem(hItem,TVIF_STATE,NULL,0,0,nState,nStateMask,0);
    }

    BOOL SetItemData(HTREEITEM hItem,DWORD_PTR dwData){
      XASSERT(::IsWindow(m_hWnd))
      return SetItem(hItem,TVIF_PARAM,NULL,0,0,0,0,(LPARAM)dwData);
    }

    HWND EditLabel(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TVM_EDITLABEL,0,(LPARAM)hItem);
    }

    HWND GetEditControl()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TVM_GETEDITCONTROL,0,0);
    }

    HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_HITTEST,0,(LPARAM)pHitTestInfo);
    }

    UINT GetVisibleCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_GETVISIBLECOUNT,0,0);
    }

    BOOL SortChildren(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SORTCHILDREN,0,(LPARAM)hItem);
    }

    BOOL EnsureVisible(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_ENSUREVISIBLE,0,(LPARAM)hItem);
    }

    BOOL SortChildrenCB(LPTVSORTCB pSort){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SORTCHILDRENCB,0,(LPARAM)pSort);
    }

    HWND GetToolTips()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TVM_GETTOOLTIPS,0,0);
    }

    HWND SetToolTips(HWND hWndTip){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,TVM_SETTOOLTIPS,(WPARAM)hWndTip,0);
    }

    COLORREF GetBkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_GETBKCOLOR,0,0);
    }

    COLORREF SetBkColor(COLORREF clr){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_SETBKCOLOR,0,(LPARAM)clr);
    }

    SHORT GetItemHeight()const{
      XASSERT(::IsWindow(m_hWnd))
      return (SHORT)::SendMessage(m_hWnd,TVM_GETITEMHEIGHT,0,0);
    }

    SHORT SetItemHeight(SHORT cyHeight){
      XASSERT(::IsWindow(m_hWnd))
      return (SHORT)::SendMessage(m_hWnd,TVM_SETITEMHEIGHT,cyHeight,0);
    }

    COLORREF GetTextColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_GETTEXTCOLOR,0,0);
    }

    COLORREF SetTextColor(COLORREF clr){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_SETTEXTCOLOR,0,(LPARAM)clr);
    }

    BOOL SetInsertMark(HTREEITEM hItem,BOOL fAfter){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SETINSERTMARK,fAfter,(LPARAM)hItem);
    }

    COLORREF GetInsertMarkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_GETINSERTMARKCOLOR,0,0);
    }

    COLORREF SetInsertMarkColor(COLORREF clrNew){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_SETINSERTMARKCOLOR,0,(LPARAM)clrNew);
    }

    BOOL EndEditLabelNow(BOOL fCancelWithoutSave){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_ENDEDITLABELNOW,(WPARAM)fCancelWithoutSave,0);
    }

  #if _WIN32_IE >= 0x0500

    COLORREF GetLineColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_GETLINECOLOR,0,0);
    }

    COLORREF SetLineColor(COLORREF clrNew=CLR_DEFAULT){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,TVM_SETLINECOLOR,0,(LPARAM)clrNew);
    }

  #endif

  #if _WIN32_IE >= 0x0600

    HTREEITEM GetNextItem(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_GETNEXTITEM,TVGN_NEXTSELECTED,(LPARAM)hItem);
    }

  #endif

  #if _WIN32_WINNT >= 0x0501

    UINT MapItemToAccId(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_MAPHTREEITEMTOACCID,(WPARAM)hItem,0);
    }

    HTREEITEM MapAccIdToItem(UINT uAccId)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_MAPACCIDTOHTREEITEM,(WPARAM)uAccId,0);
    }

  #endif

  #if _WIN32_WINNT >= 0x0600

    UINT GetSelectedCount(){
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,TVM_GETSELECTEDCOUNT,0,0);
    }

    VOID ShowInfoTip(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,TVM_SHOWINFOTIP,0,(LPARAM)hItem);
    }

    BOOL SetItemStateEx(HTREEITEM hItem,UINT uStateEx){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)SetItemEx(hItem,TVIF_STATEEX,0,0,0,0,0,0,uStateEx,0,0);
    }

    BOOL SetItemExpandedImageIndex(HTREEITEM hItem,INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)SetItemEx(hItem,TVIF_EXPANDEDIMAGE,0,0,0,0,0,0,TVIF_EXPANDEDIMAGE,0,nIndex);
    }

    DWORD GetExtendedStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TVM_GETEXTENDEDSTYLE,(WPARAM)0,0);
    }

    DWORD SetExtendedStyle(DWORD dwExMask,DWORD dwExStyles){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TVM_SETEXTENDEDSTYLE,(WPARAM)dwExMask,(LPARAM)dwExStyles);
    }

    LRESULT SetHot(HTREEITEM hItem){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,TVM_SETHOT,0,(LPARAM)hItem);
    }

    BOOL SetAutoscrollInfo(UINT uPixelsPerSec,UINT uUpdateTime){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,TVM_SETAUTOSCROLLINFO,(WPARAM)uPixelsPerSec,(LPARAM)uUpdateTime);
    }

  #endif

    BOOL GetItemRect(HTREEITEM hItem,LPRECT lpRect,BOOL bTextOnly)const{
      XASSERT(::IsWindow(m_hWnd))
      *(HTREEITEM*)lpRect=hItem;
      return (BOOL)::SendMessage(m_hWnd,TVM_GETITEMRECT,(WPARAM)bTextOnly,(LPARAM)lpRect);
    }

    UINT GetItemText(HTREEITEM hItem,LPTSTR lpszText,UINT cchTextMax)const{
      TVITEM _tvi;_tvi.hItem=hItem;_tvi.mask=TVIF_TEXT;_tvi.cchTextMax=cchTextMax;_tvi.pszText=lpszText;
      return (UINT)::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&_tvi);
    }

    BOOL GetItemImage(HTREEITEM hItem,INT &nImage,INT &nSelectedImage)const{
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.hItem=hItem;item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
      if(::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item)){
        nImage=item.iImage;nSelectedImage=item.iSelectedImage;
        return TRUE;
      }
      return FALSE;
    }

    UINT GetItemState(HTREEITEM hItem,UINT nStateMask)const{
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.hItem=hItem;item.mask=TVIF_STATE;item.stateMask=nStateMask;item.state=0;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return item.state;
    }

    DWORD_PTR GetItemData(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.hItem=hItem;item.mask=TVIF_PARAM;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return item.lParam;
    }

    BOOL ItemHasChildren(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.hItem=hItem;item.mask=TVIF_CHILDREN;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return item.cChildren;
    }

    BOOL SetItem(HTREEITEM hItem,UINT nMask,LPCTSTR lpszItem,INT nImage,INT nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam){
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.hItem=hItem;item.mask=nMask;item.pszText=(LPTSTR) lpszItem;item.iImage=nImage;
      item.iSelectedImage=nSelectedImage;item.state=nState;item.stateMask=nStateMask;item.lParam=lParam;
      return (BOOL)::SendMessage(m_hWnd,TVM_SETITEM,0,(LPARAM)&item);
    }

  #if (_WIN32_IE >= 0x0600)

    BOOL SetItemEx(HTREEITEM hItem,UINT nMask,LPCTSTR lpszItem,INT nImage,INT nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam,UINT uStateEx,HWND hWnd,INT iExpandedImage){
      XASSERT(::IsWindow(m_hWnd))
      TVITEMEX item;item.hItem=hItem;item.mask=nMask;item.pszText=(LPTSTR) lpszItem;item.iImage=nImage;
      item.iSelectedImage=nSelectedImage;item.state=nState;item.stateMask=nStateMask;item.lParam=lParam;
      #if NTDDI_VERSION >= 0x06010000
      item.uStateEx=uStateEx;item.hwnd=hWnd;item.iExpandedImage=iExpandedImage;
      #else
      UNREFERENCED_PARAMETER(uStateEx)
      UNREFERENCED_PARAMETER(hWnd)
      UNREFERENCED_PARAMETER(iExpandedImage)
      #endif
      return (BOOL)::SendMessage(m_hWnd,TVM_SETITEM,0,(LPARAM)&item);
    }

  #endif

    HTREEITEM InsertItem(UINT nMask,LPCTSTR lpszItem,INT nImage,INT nSelectedImage,UINT nState,UINT nStateMask,LPARAM lParam,HTREEITEM hParent,HTREEITEM hInsertAfter){
      XASSERT(::IsWindow(m_hWnd))
      TVINSERTSTRUCT tvis;tvis.hParent=hParent;tvis.hInsertAfter=hInsertAfter;tvis.item.mask=nMask;
      tvis.item.pszText=(LPTSTR)lpszItem;tvis.item.iImage=nImage;tvis.item.iSelectedImage=nSelectedImage;
      tvis.item.state=nState;tvis.item.stateMask=nStateMask;tvis.item.lParam=lParam;
      return (HTREEITEM)::SendMessage(m_hWnd,TVM_INSERTITEM,0,(LPARAM)&tvis);
    }

    HTREEITEM HitTest(POINT pt,UINT* pFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      TVHITTESTINFO hti;hti.pt=pt;
      HTREEITEM h=(HTREEITEM)::SendMessage(m_hWnd,TVM_HITTEST,0,(LPARAM)&hti);
      if(pFlags!=NULL)*pFlags=hti.flags;
      return h;
    }

    BOOL GetCheck(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.mask=TVIF_HANDLE|TVIF_STATE;item.hItem=hItem;item.stateMask=TVIS_STATEIMAGEMASK;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return ((BOOL)(item.state>>12)-1);
    }

    BOOL SetCheck(HTREEITEM hItem,BOOL fCheck){
      XASSERT(::IsWindow(m_hWnd))
      TVITEM item;item.mask=TVIF_HANDLE|TVIF_STATE;item.hItem=hItem;
      item.stateMask=TVIS_STATEIMAGEMASK;item.state=INDEXTOSTATEIMAGEMASK((fCheck?2:1));
      return (BOOL)::SendMessage(m_hWnd,TVM_SETITEM,0,(LPARAM)&item);
    }

  #if _WIN32_WINNT >= 0x0600

    BOOL GetItemPartRect(HTREEITEM hItem,TVITEMPART nPart,LPRECT lpRect){
      XASSERT(::IsWindow(m_hWnd)&&lpRect)
      TVGETITEMPARTRECTINFO info;info.hti=hItem;info.prc=lpRect;info.partID=nPart;
      return (BOOL)::SendMessage((m_hWnd),TVM_GETITEMPARTRECT,0,(LPARAM)&info);
    }

    UINT GetItemStateEx(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd)&&hItem)
      TVITEMEX item;item.hItem=hItem;item.mask=TVIF_STATEEX;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return item.uStateEx;
    }

    INT GetItemExpandedImageIndex(HTREEITEM hItem)const{
      XASSERT(::IsWindow(m_hWnd)&&hItem)
      TVITEMEX item;item.hItem=hItem;item.mask=TVIF_EXPANDEDIMAGE;item.uStateEx=TVIF_EXPANDEDIMAGE;
      ::SendMessage(m_hWnd,TVM_GETITEM,0,(LPARAM)&item);
      return item.iExpandedImage;
    }

  #endif

  DECLARE_EVENT_TABLE()
};

#endif //_XTREEVIEW_H_
