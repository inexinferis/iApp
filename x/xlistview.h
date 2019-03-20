#ifndef _XLISTVIEW_H_
#define _XLISTVIEW_H_

#include "xwnd.h"

#define XDEFLISTVIEWSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XListView: public XWnd{
  DECLARE_DYNAMIC(XListView)
  public:
    XListView(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XListView(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTVIEWSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_LISTVIEW,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTVIEWSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_LISTVIEW,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    HIMAGELIST CreateDragImage(INT iItem,LPPOINT lpptUpLeft){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,LVM_CREATEDRAGIMAGE,iItem,(LPARAM)lpptUpLeft);
    }

    COLORREF GetBkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_GETBKCOLOR,0,0);
    }

    BOOL SetBkColor(COLORREF cr){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETBKCOLOR,0,cr);
    }

  #if (_WIN32_WINNT >= 0x500)

    BOOL GetBkImage(LPLVBKIMAGE plvbki){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETBKIMAGE,0,(LPARAM)plvbki);
    }

    BOOL SetBkImage(LPLVBKIMAGE plvbki){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETBKIMAGE,0,(LPARAM)plvbki);
    }

    BOOL SetBkImage(HBITMAP hbm,BOOL fTile=TRUE,INT xOffsetPercent=0,INT yOffsetPercent=0){
      LVBKIMAGE lv;lv.hbm=hbm;
      lv.ulFlags=LVBKIF_SOURCE_HBITMAP|(fTile?LVBKIF_STYLE_TILE:LVBKIF_STYLE_NORMAL);
      lv.xOffsetPercent=xOffsetPercent;lv.yOffsetPercent=yOffsetPercent;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETBKIMAGE,0,(LPARAM)&lv);
    }

    BOOL SetBkImage(LPTSTR pszUrl,BOOL fTile=TRUE,INT xOffsetPercent=0,INT yOffsetPercent=0){
      LVBKIMAGE lv;lv.pszImage=pszUrl;
      lv.ulFlags=LVBKIF_SOURCE_URL|(fTile?LVBKIF_STYLE_TILE:LVBKIF_STYLE_NORMAL);
      lv.xOffsetPercent=xOffsetPercent;lv.yOffsetPercent=yOffsetPercent;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETBKIMAGE,0,(LPARAM)&lv);
    }

  #endif

    INT GetSelectedItem(INT nFlags=LVNI_FOCUSED|LVNI_SELECTED){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETNEXTITEM,-1,MAKELPARAM(nFlags,0));
    }

    INT SetSelectedItem(INT nItem,INT nFlags=LVNI_FOCUSED|LVNI_SELECTED){
      XASSERT(::IsWindow(m_hWnd))
	    LV_ITEM _lvi;_lvi.stateMask=LVIS_SELECTED|LVIS_FOCUSED;_lvi.state=nFlags;
	    return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMSTATE,nItem,(LPARAM)&_lvi);
    }

    BOOL GetCheck(UINT iIndex){
      XASSERT(::IsWindow(m_hWnd))
      INT nState=(INT)::SendMessage(m_hWnd,LVM_GETITEMSTATE,(WPARAM)iIndex,LVIS_STATEIMAGEMASK);
      return ((BOOL)(nState>>12)-1);
    }

    BOOL SetCheck(INT nItem,BOOL fCheck){
      XASSERT(::IsWindow(m_hWnd))
      LVITEM lvi;lvi.stateMask=LVIS_STATEIMAGEMASK;
      lvi.state=INDEXTOSTATEIMAGEMASK((fCheck?2:1));
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMSTATE,nItem,(LPARAM)&lvi);
    }

    HIMAGELIST GetImageList(INT nImageListType)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,LVM_GETIMAGELIST,nImageListType,0);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList,INT nImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,LVM_SETIMAGELIST,nImageList,(LPARAM)hImageList);
    }

    VOID RemoveImageList(INT nImageList){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETIMAGELIST,(WPARAM)nImageList,0);
    }

    INT GetItemCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETITEMCOUNT,0,0);
    }

    BOOL GetItem(LVITEM* pItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETITEM,0,(LPARAM)pItem);
    }

    BOOL SetItem(const LVITEM* pItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)pItem);
    }

    BOOL SetItem(INT nItem,INT nSubItem,UINT nMask,LPCTSTR lpszItem,INT iImage,UINT nState,UINT nStateMask,LPARAM lParam=0,INT nIndent=0){
      XASSERT(::IsWindow(m_hWnd))
      LVITEM lvi={nMask,nItem,nSubItem,nState,nStateMask,(LPTSTR)lpszItem,0,iImage,lParam,nIndent
      #if (_WIN32_WINNT >= 0x0501)
      ,0,0,NULL
      #endif
      };
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvi);
    }

    BOOL SetItemText(INT nItem,INT nSubItem,LPTSTR lpszText){
      LV_ITEM _lvi;_lvi.iSubItem=nSubItem;_lvi.pszText=lpszText;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMTEXT,nItem,(LPARAM)&_lvi);
    }

    UINT GetItemText(INT nItem,INT nSubItem,LPTSTR lpszText,UINT cchTextMax)const{
      LV_ITEM _lvi;_lvi.iSubItem=nSubItem;_lvi.cchTextMax=cchTextMax;_lvi.pszText=lpszText;
      return (UINT)::SendMessage(m_hWnd,LVM_GETITEMTEXT,nItem,(LPARAM)&_lvi);
    }

    BOOL SetItemData(INT nItem,DWORD_PTR dwData){
      XASSERT(::IsWindow(m_hWnd))
      LVITEM lvi;lvi.iItem=nItem;lvi.mask=LVIF_PARAM;lvi.lParam=(LPARAM)dwData;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvi);
    }

    DWORD_PTR GetItemData(INT nItem) const{
      XASSERT(::IsWindow(m_hWnd))
      LVITEM lvi;lvi.iItem=nItem;lvi.mask=LVIF_PARAM;
      if(::SendMessage(m_hWnd,LVM_GETITEM,0,(LPARAM)&lvi))
        return lvi.lParam;
      return (DWORD_PTR)NULL;
    }

    INT InsertItem(const LVITEM* pItem){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_INSERTITEM,0,(LPARAM)pItem);
    }

    INT InsertItem(UINT nMask,INT nItem,LPCTSTR lpszItem,UINT nState=0,UINT nStateMask=0,INT nImage=0,LPARAM lParam=0){
      XASSERT(::IsWindow(m_hWnd))
      LVITEM item;item.mask=nMask;item.iItem=nItem;item.iSubItem=0;item.pszText=(LPTSTR)lpszItem;
      item.state=nState;item.stateMask=nStateMask;item.iImage=nImage;item.lParam=lParam;
      return (INT)::SendMessage(m_hWnd,LVM_INSERTITEM,0,(LPARAM)&item);
    }

    INT InsertItem(INT nItem,LPCTSTR lpszItem){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(LVIF_TEXT,nItem,lpszItem);
    }

    INT InsertItem(INT nItem,LPCTSTR lpszItem,INT nImage){
      XASSERT(::IsWindow(m_hWnd))
      return InsertItem(LVIF_TEXT|LVIF_IMAGE,nItem,lpszItem,0,0,nImage);
    }

    BOOL DeleteItem(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_DELETEITEM,nItem,0);
    }

    BOOL DeleteAllItems(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_DELETEALLITEMS,0,0);
    }

    UINT GetCallbackMask()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_GETCALLBACKMASK,0,0);
    }

    BOOL SetCallbackMask(UINT nMask){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETCALLBACKMASK,nMask,0);
    }

    INT GetNextItem(INT nItem,INT nFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETNEXTITEM,nItem,MAKELPARAM(nFlags,0));
    }

    INT FindItem(LVFINDINFO* pFindInfo,INT nStart)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_FINDITEM,nStart,(LPARAM)pFindInfo);
    }

    INT FindItem(UINT iStart,UINT flags,PTCHAR psz,VOID* lParam,LPPOINT pt,UINT vkDir){
      XASSERT(::IsWindow(m_hWnd))
      LV_FINDINFO lvfi;
      lvfi.flags=flags;lvfi.psz=psz;lvfi.lParam=(LPARAM)lParam;lvfi.pt=*pt;lvfi.vkDirection=vkDir;
      return (INT)::SendMessage(m_hWnd,LVM_FINDITEM,iStart,(LPARAM)&lvfi);
    }

    INT HitTest(LVHITTESTINFO* pHitTestInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_HITTEST,0,(LPARAM)pHitTestInfo);
    }

    INT HitTestEx(LVHITTESTINFO* pHitTestInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_HITTEST,(WPARAM)-1,(LPARAM)pHitTestInfo);
    }

    INT HitTest(POINT pt,PUINT pFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      LVHITTESTINFO hti;hti.pt=pt;
      INT nRes=(INT)::SendMessage(m_hWnd,LVM_HITTEST,0,(LPARAM)&hti);
      if(pFlags!=NULL)*pFlags=hti.flags;
      return nRes;
    }

    BOOL SetItemPosition(INT nItem,POINT pt){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMPOSITION32,nItem,(LPARAM)&pt);
    }

    BOOL SetItemPosition(INT nItem,INT x,INT y){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMPOSITION32,nItem,MAKELPARAM(x,y));
    }

    BOOL GetItemPosition(INT nItem,LPPOINT lpPoint)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETITEMPOSITION,nItem,(LPARAM)lpPoint);
    }

    INT GetStringWidth(LPCTSTR lpsz)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETSTRINGWIDTH,0,(LPARAM)lpsz);
    }

    BOOL EnsureVisible(INT nItem,BOOL bPartialOK){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_ENSUREVISIBLE,nItem,MAKELPARAM(bPartialOK,0));
    }

    BOOL Scroll(INT cx,INT cy){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SCROLL,cx,cy);
    }

    BOOL Scroll(SIZE size){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SCROLL,size.cx,size.cy);
    }

    BOOL RedrawItems(INT nFirst,INT nLast){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_REDRAWITEMS,nFirst,nLast);
    }

    BOOL Arrange(UINT nCode){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_ARRANGE,nCode,0);
    }

    HWND EditLabel(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,LVM_EDITLABEL,nItem,0);
    }

    HWND GetEditControl()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,LVM_GETEDITCONTROL,0,0);
    }

    BOOL GetColumn(INT nCol,LVCOLUMN* pColumn)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETCOLUMN,nCol,(LPARAM)pColumn);
    }

    BOOL SetColumn(INT nCol,const LVCOLUMN* pColumn){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMN,nCol,(LPARAM)pColumn);
    }

    INT SetColumn(UINT mask,INT iCol,INT cx,PTCHAR pszText,INT fmt=LVCFMT_CENTER,INT iImage=0,INT iOrder=0){
      LV_COLUMN col;
      col.mask=mask;col.fmt=fmt;col.cx=cx;col.pszText=pszText;col.iSubItem=iCol;
      col.iImage=iImage;col.iOrder=iOrder;
      return (INT)::SendMessage(m_hWnd,LVM_SETCOLUMN,(WPARAM)iCol,(LPARAM)&col);
    }

    INT InsertColumn(INT nCol,const LVCOLUMN* pColumn){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_INSERTCOLUMN,nCol,(LPARAM)pColumn);
    }

    INT InsertColumn(INT nCol,LPCTSTR lpszColumnHeading,INT nFormat=LVCFMT_LEFT,INT nWidth=-1,INT nSubItem=-1){
      XASSERT(::IsWindow(m_hWnd))
      LVCOLUMN column;
      column.mask=LVCF_TEXT|LVCF_FMT;
      column.pszText=(LPTSTR)lpszColumnHeading;
      column.fmt=nFormat;
      if(nWidth!=-1){
        column.mask|=LVCF_WIDTH;
        column.cx=nWidth;
      }
      if(nSubItem!=-1){
        column.mask|=LVCF_SUBITEM;
        column.iSubItem=nSubItem;
      }
      return (INT)::SendMessage(m_hWnd,LVM_INSERTCOLUMN,nCol,(LPARAM)&column);
    }

    BOOL DeleteColumn(INT nCol){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_DELETECOLUMN,nCol,0);
    }

    INT GetColumnWidth(INT nCol)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETCOLUMNWIDTH,nCol,0);
    }

    BOOL SetColumnWidth(INT nCol,INT cx){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMNWIDTH,nCol,MAKELPARAM(cx,0));
    }

    BOOL GetViewRect(LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETVIEWRECT,0,(LPARAM)lpRect);
    }

    BOOL GetItemRect(INT nItem,LPRECT lpRect,UINT nCode)const{
      XASSERT(::IsWindow(m_hWnd)&&lpRect!=NULL)
      return (BOOL)::SendMessage(m_hWnd,LVM_GETITEMRECT,nItem,(lpRect?lpRect->left=nCode,(LPARAM)lpRect:(LPARAM)NULL));
    }

    COLORREF GetTextColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_GETTEXTCOLOR,0,0);
    }

    BOOL SetTextColor(COLORREF cr){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTEXTCOLOR,0,cr);
    }

    COLORREF GetTextBkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_GETTEXTBKCOLOR,0,0);
    }

    BOOL SetTextBkColor(COLORREF cr){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTEXTBKCOLOR,0,cr);
    }

    INT GetTopIndex()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETTOPINDEX,0,0);
    }

    INT GetCountPerPage()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETCOUNTPERPAGE,0,0);
    }

    BOOL GetOrigin(LPPOINT lpPoint)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETORIGIN,0,(LPARAM)lpPoint);
    }

    BOOL Update(INT nItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_UPDATE,nItem,0);
    }

    BOOL SetItemState(INT nItem,LVITEM* pItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMSTATE,nItem,(LPARAM)pItem);
    }

    BOOL SetItemState(INT nItem,INT stateMask,INT state){
      XASSERT(::IsWindow(m_hWnd))
	    LV_ITEM _lvi;_lvi.stateMask=stateMask;_lvi.state=state;
	    return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMSTATE,nItem,(LPARAM)&_lvi);
    }

    UINT GetItemState(INT nItem,UINT nMask)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_GETITEMSTATE,nItem,nMask);
    }

    VOID SetItemCount(INT nItems){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETITEMCOUNT,nItems,0);
    }

    BOOL SetItemCountEx(INT iCount,DWORD dwFlags){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETITEMCOUNT,iCount,(LPARAM)dwFlags);
    }

    BOOL SortItems(PFNLVCOMPARE pfnCompare,DWORD_PTR dwData){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SORTITEMS,dwData,(LPARAM)pfnCompare);
    }

    UINT GetSelectedCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_GETSELECTEDCOUNT,0,0);
    }

  #if (_WIN32_WINNT >= 0x500)

    INT GetHotItem(){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETHOTITEM,0,0);
    }

    INT SetHotItem(INT iIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_SETHOTITEM,iIndex,0);
    }

    HCURSOR GetHotCursor(){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,LVM_GETHOTCURSOR,0,0);
    }

    HCURSOR SetHotCursor(HCURSOR hcur){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,LVM_SETHOTCURSOR,0,(LPARAM)hcur);
    }

    DWORD SetIconSpacing(INT cx,INT cy){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_SETICONSPACING,0,MAKELONG(cx,cy));
    }

    DWORD SetIconSpacing(SIZE sz){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_SETICONSPACING,0,MAKELONG(sz.cx,sz.cy));
    }

    INT SubItemHitTest(LPLVHITTESTINFO pInfo){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_SUBITEMHITTEST,0,(LPARAM)pInfo);
    }

    INT SubItemHitTestEx(LPLVHITTESTINFO pInfo){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_SUBITEMHITTEST,(WPARAM)-1,(LPARAM)pInfo);
    }

    BOOL SetColumnOrderArray(INT iCount,PINT lpiArray){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETCOLUMNORDERARRAY,iCount,(LPARAM)lpiArray);
    }

    BOOL GetSubItemRect(INT iItem,INT iSubItem,INT code,LPRECT prc){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETSUBITEMRECT,iItem,(prc?prc->top=iSubItem,prc->left=code,(LPARAM)prc:(LPARAM)NULL));
    }

    VOID SetWorkAreas(INT nWorkAreas,LPRECT lprc){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETWORKAREAS,nWorkAreas,(LPARAM)lprc);
    }

    VOID GetWorkAreas(INT nWorkAreas,LPRECT lprc){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_GETWORKAREAS,nWorkAreas,(LPARAM)lprc);
    }

    BOOL GetNumberOfWorkAreas(LPUINT lpuWorkAreas){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETNUMBEROFWORKAREAS,0,(LPARAM)lpuWorkAreas);
    }

    VOID SetHoverTime(DWORD dwHoverTime){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETHOVERTIME,0,(LPARAM)dwHoverTime);
    }

    DWORD GetHoverTime(){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_GETHOVERTIME,0,0);
    }

    HWND GetHeader(){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,LVM_GETHEADER,0,0);
    }

    INT GetSelectionMark(){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETSELECTIONMARK,0,0);
    }

    INT SetSelectionMark(INT iIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_SETSELECTIONMARK,0,(LPARAM)iIndex);
    }

    HWND SetToolTips(HWND hwndToolTip){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,LVM_SETTOOLTIPS,(WPARAM)hwndToolTip,0);
    }

    HWND GetToolTips(){
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,LVM_GETTOOLTIPS,0,0);
    }

    BOOL SetUnicodeFormat(BOOL fUnicode){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETUNICODEFORMAT,fUnicode,0);
    }

    BOOL GetUnicodeFormat(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETUNICODEFORMAT,0,0);
    }

    DWORD GetItemSpacing(BOOL fSmall){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_GETITEMSPACING,fSmall,0);
    }

    DWORD SetExtendedStyle(DWORD dwExStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,0,dwExStyle);
    }

    DWORD SetExtendedStyleEx(DWORD dwMask,DWORD dw){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_SETEXTENDEDLISTVIEWSTYLE,dwMask,dw);
    }

    DWORD ApproximateViewRect(INT cx,INT cy,INT iCount=-1){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_APPROXIMATEVIEWRECT,iCount,MAKELPARAM(cx,cy));
    }

    BOOL GetColumnOrderArray(INT iCount,PINT lpiArray)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETCOLUMNORDERARRAY,(WPARAM)iCount,(LPARAM)lpiArray);
    }

    DWORD GetExtendedStyle() const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_GETEXTENDEDLISTVIEWSTYLE,0,0);
    }

    BOOL SortItemsEx(PFNLVCOMPARE pfnCompare,LPARAM lParamSort){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SORTITEMSEX,(WPARAM)lParamSort,(LPARAM)pfnCompare);
    }

    BOOL GetISearchString(LPTSTR lpsz){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETISEARCHSTRING,0,(LPARAM)lpsz);
    }

  #endif

  #if (_WIN32_WINNT >= 0x0501)

    VOID SetSelectedColumn(INT iCol){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETSELECTEDCOLUMN,(WPARAM)iCol,0);
    }

    DWORD SetView(INT iView){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_SETVIEW,iView,0);
    }

    DWORD GetView()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,LVM_GETVIEW,0,0);
    }

    INT RemoveGroup(INT iGroupId){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_REMOVEGROUP,iGroupId,0);
    }

    VOID MoveGroup(INT iGroupId,INT toIndex){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_MOVEGROUP,iGroupId,toIndex);
    }

    //Not Defined in Mingw WinAPI 3.17
    #ifdef LVM_INSERTGROUP
    INT InsertGroup(INT index,PLVGROUP pgrp){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_INSERTGROUP,(WPARAM)index,(LPARAM)pgrp);
    }
    #endif

    #ifdef LVM_SETGROUPINFO
    INT SetGroupInfo(INT iGroupId,PLVGROUP pGroup){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_SETGROUPINFO,(WPARAM)iGroupId,(LPARAM)pGroup);
    }
    #endif

    #ifdef LVM_GETGROUPINFO
    INT GetGroupInfo(INT iGroupId,PLVGROUP pGroup)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETGROUPINFO,(WPARAM)iGroupId,(LPARAM)pGroup);
    }
    #endif

    #ifdef LVM_MOVEITEMTOGROUP
    VOID MoveItemToGroup(INT idItemFrom,INT idGroupTo){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_MOVEITEMTOGROUP,(WPARAM)idItemFrom,(LPARAM)idGroupTo);
    }
    #endif

    #ifdef LVM_SETGROUPMETRICS
    VOID SetGroupMetrics(PLVGROUPMETRICS pGroupMetrics){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_SETGROUPMETRICS,0,(LPARAM)pGroupMetrics);
    }
    #endif

    #ifdef LVM_GETGROUPMETRICS
    VOID GetGroupMetrics(PLVGROUPMETRICS pGroupMetrics)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_GETGROUPMETRICS,0,(LPARAM)pGroupMetrics);
    }
    #endif

    #ifdef LVM_SORTGROUPS
    BOOL SortGroups(PFNLVGROUPCOMPARE pfnGroupCompare,LPVOID plv){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SORTGROUPS,(WPARAM)pfnGroupCompare,(LPARAM)plv);
    }
    #endif

    #ifdef LVM_INSERTGROUPSORTED
    VOID InsertGroupSorted(PLVINSERTGROUPSORTED pStructInsert){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_INSERTGROUPSORTED,(WPARAM)pStructInsert,0);
    }
    #endif

    #ifdef LVM_SETINSERTMARK
    BOOL SetInsertMark(LPLVINSERTMARK lvim){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETINSERTMARK,0,(LPARAM)lvim);
    }
    #endif

    #ifdef LVM_GETINSERTMARK
    BOOL GetInsertMark(LPLVINSERTMARK lvim)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETINSERTMARK,0,(LPARAM)lvim);
    }
    #endif

    #ifdef LVM_INSERTMARKHITTEST
    INT InsertMarkHitTest(LPPOINT pPoint,LPLVINSERTMARK lvim)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_INSERTMARKHITTEST,(WPARAM)pPoint,(LPARAM)lvim);
    }
    #endif

    #ifdef LVM_SETINFOTIP
    BOOL SetInfoTip(PLVSETINFOTIP plvInfoTip){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETINFOTIP,0,(LPARAM)plvInfoTip);
    }
    #endif

    #ifdef LVM_SETTILEVIEWINFO
    BOOL SetTileViewInfo(PLVTILEVIEWINFO ptvi){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTILEVIEWINFO,0,(LPARAM)ptvi);
    }
    #endif

    #ifdef LVM_GETTILEVIEWINFO
    BOOL GetTileViewInfo(PLVTILEVIEWINFO ptvi)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETTILEVIEWINFO,0,(LPARAM)ptvi);
    }
    #endif

    #ifdef LVM_SETTILEINFO
    BOOL SetTileInfo(PLVTILEINFO pti){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTILEINFO,0,(LPARAM)pti);
    }
    #endif

    #ifdef LVM_GETTILEINFO
    BOOL GetTileInfo(PLVTILEINFO pti)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETTILEINFO,0,(LPARAM)pti);
    }
    #endif

    #ifdef LVM_SETTILEVIEWINFO
    BOOL SetTileViewInfo(DWORD dwM,DWORD dwF,UINT nLines,SIZE szTile,RECT rcLabel){
      LVTILEVIEWINFO tvi;
      tvi.cbSize=sizeof(LVTILEVIEWINFO);tvi.dwMask=dwM;tvi.dwFlags=dwF;
      tvi.sizeTile=szTile;tvi.cLines=nLines;tvi.rcLabelMargin=rcLabel;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTILEVIEWINFO,0,(LPARAM)&tvi);
    }
    #endif

    #ifdef LVM_SETTILEINFO
    BOOL SetTileInfo(INT iItem,UINT cCol,UINT *puCol){
      LVTILEINFO lvti;
      lvti.cbSize=sizeof(LVTILEINFO);lvti.iItem=iItem;
      lvti.cColumns=cCol;lvti.puColumns=puCol;
      return (BOOL)::SendMessage(m_hWnd,LVM_SETTILEINFO,0,(LPARAM)&lvti);
    }
    #endif

    INT EnableGroupView(BOOL fEnable){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_ENABLEGROUPVIEW,fEnable,0);
    }

    VOID RemoveAllGroups(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_REMOVEALLGROUPS,0,0);
    }

    BOOL HasGroup(INT iGroupId)const{
      XASSERT(::IsWindow(m_hWnd))
      return ::SendMessage(m_hWnd,LVM_HASGROUP,iGroupId,0);
    }

    INT GetInsertMarkRect(LPRECT pRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETINSERTMARKRECT,0,(LPARAM)pRect);
    }

    COLORREF SetInsertMarkColor(COLORREF color){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_SETINSERTMARKCOLOR,0,(LPARAM)color);
    }

    COLORREF GetInsertMarkColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_GETINSERTMARKCOLOR,0,0);
    }

    UINT GetSelectedColumn()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_GETSELECTEDCOLUMN,0,0);
    }

    BOOL IsGroupViewEnabled() const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_ISGROUPVIEWENABLED,0,0);
    }

    COLORREF GetOutlineColor()const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_GETOUTLINECOLOR,0,0);
    }

    COLORREF SetOutlineColor(COLORREF color){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,LVM_SETOUTLINECOLOR,0,(LPARAM)color);
    }

    VOID CancelEditLabel(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,LVM_CANCELEDITLABEL,0,0);
    }

  #endif

  #if (_WIN32_WINNT >= 0x0600)
    //Not Defined in Mingw WinAPI 3.17
    #ifdef LVM_GETITEMINDEXRECT
    BOOL GetItemIndexRect(PLVITEMINDEX pItemIndex,INT iColumn,INT rectType,LPRECT pRect) const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETITEMINDEXRECT,(WPARAM)pItemIndex,(pRect?(pRect->top=iColumn,pRect->left=rectType,(LPARAM)pRect):(LPARAM)NULL));
    }
    #endif

    #ifdef LVM_SETITEMINDEXSTATE
    HRESULT SetItemIndexState(PLVITEMINDEX pItemIndex,DWORD dwState,DWORD dwMask){
      XASSERT(::IsWindow(m_hWnd))
      LV_ITEM _lvi={0};_lvi.stateMask=dwMask;_lvi.state=dwState;
      return (HRESULT)::SendMessage(m_hWnd,LVM_SETITEMINDEXSTATE,(WPARAM)pItemIndex,(LPARAM)&_lvi);
    }
    #endif

    #ifdef LVM_GETNEXTITEMINDEX
    BOOL GetNextItemIndex(PLVITEMINDEX pItemIndex,INT nFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETNEXTITEMINDEX,(WPARAM)pItemIndex,MAKELPARAM(nFlags, 0));
    }
    #endif

    #ifdef LVM_GETGROUPINFOBYINDEX
    BOOL GetGroupInfoByIndex(INT iIndex,PLVGROUP pGroup)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETGROUPINFOBYINDEX,iIndex,(LPARAM)pGroup);
    }
    #endif

    #ifdef LVM_GETGROUPCOUNT
    INT GetGroupCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETGROUPCOUNT,0,0);
    }
    #endif

    #ifdef LVM_SETGROUPINFO
    VOID SetGroupState(INT iGroupId,DWORD dwMask,DWORD dwState){
      XASSERT(::IsWindow(m_hWnd))
      LVGROUP _lvg;_lvg.cbSize=sizeof(_lvg);_lvg.mask=LVGF_STATE;_lvg.stateMask=dwMask;_lvg.state=dwState;
      ::SendMessage(m_hWnd,LVM_SETGROUPINFO,iGroupId,(LPARAM)&_lvg);
    }
    #endif

    #ifdef LVM_GETGROUPSTATE
    UINT GetGroupState(INT iGroupId,DWORD dwMask)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_GETGROUPSTATE,iGroupId,(LPARAM)dwMask);
    }
    #endif

    #ifdef LVM_GETEMPTYTEXT
    BOOL GetEmptyText(PWSTR pszText,UINT cchText){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETEMPTYTEXT,cchText,(LPARAM)pszText);
    }
    #endif

    #ifdef LVM_GETFOCUSEDGROUP
    INT GetFocusedGroup()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LVM_GETFOCUSEDGROUP,0,0);
    }
    #endif

    #ifdef LVM_GETGROUPRECT
    BOOL GetGroupRect(INT iGroupId,LPRECT lpRect,INT iType)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETGROUPRECT,iGroupId,(lpRect?lpRect->top=iType,(LPARAM)lpRect:(LPARAM)NULL));
    }
    #endif

    #ifdef LVM_MAPINDEXTOID
    UINT MapIndexToID(UINT index)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_MAPINDEXTOID,index,0);
    }
    #endif

    #ifdef LVM_MAPIDTOINDEX
    UINT MapIDToIndex(UINT id)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,LVM_MAPIDTOINDEX,id,0);
    }
    #endif

    #ifdef LVM_ISITEMVISIBLE
    BOOL IsItemVisible(INT index)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_ISITEMVISIBLE,index,0);
    }
    #endif

    #ifdef LVM_SETIMAGELIST
    HIMAGELIST SetGroupHeaderImageList(HIMAGELIST hImageList){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,LVM_SETIMAGELIST,LVSIL_GROUPHEADER,(LPARAM)hImageList);
    }
    #endif

    #ifdef LVM_GETIMAGELIST
    HIMAGELIST GetGroupHeaderImageList(){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,LVM_GETIMAGELIST,LVSIL_GROUPHEADER,0);
    }
    #endif

    #ifdef LVM_GETFOOTERRECT
    BOOL GetFooterRect(RECT *prc){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETFOOTERRECT,0,(LPARAM)prc);
    }
    #endif

    #ifdef LVM_GETFOOTERINFO
    BOOL GetFooterInfo(LPLVFOOTERINFO plvfi){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETFOOTERINFO,0,(LPARAM)plvfi);
    }
    #endif

    #ifdef LVM_GETFOOTERITEMRECT
    BOOL GetFooterItemRect(UINT iItem,RECT *prc){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETFOOTERITEMRECT,iItem,(LPARAM)prc);
    }
    #endif

    #ifdef LVM_GETFOOTERITEM
    BOOL GetFooterItem(UINT iItem,LVFOOTERITEM *pfi){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,LVM_GETFOOTERITEM,iItem,(LPARAM)pfi);
    }
    #endif

  #endif

  DECLARE_EVENT_TABLE()
};

#endif //_XLISTVIEW_H_
