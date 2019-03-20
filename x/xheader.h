#ifndef _XHEADER_H_
#define _XHEADER_H_

#include "xwnd.h"

#define XDEFHEADERSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XHeader:public XWnd{
  DECLARE_DYNAMIC(XHeader)
  public:
    XHeader(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XHeader(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFHEADERSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_HEADER,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFHEADERSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_HEADER,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    INT GetItemCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_GETITEMCOUNT,0,0);
    }

    INT InsertItem(INT nPos,HDITEM* phdi){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_INSERTITEM,nPos,(LPARAM)phdi);
    }

    BOOL DeleteItem(INT nPos){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_DELETEITEM,nPos,0);
    }

    BOOL GetItem(INT nPos,HDITEM* pHeaderItem)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_GETITEM,nPos,(LPARAM)pHeaderItem);
    }

    BOOL SetItem(INT nPos,HDITEM* pHeaderItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_SETITEM,nPos,(LPARAM)pHeaderItem);
    }

    BOOL Layout(HDLAYOUT* pHeaderLayout){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_LAYOUT,0,(LPARAM)pHeaderLayout);
    }

    INT HitTest(LPHDHITTESTINFO pHeaderHitTestInfo){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_HITTEST,0,(LPARAM)pHeaderHitTestInfo);
    }

    BOOL GetItemRect(INT nIndex,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd)&&lpRect!=NULL)
      return (BOOL)::SendMessage(m_hWnd,HDM_GETITEMRECT,nIndex,(LPARAM)lpRect);
    }

    INT OrderToIndex(INT nOrder)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_ORDERTOINDEX,nOrder,0);
    }

    INT SetHotDivider(POINT pt){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_SETHOTDIVIDER,TRUE,MAKELPARAM(pt.x,pt.y));
    }

    INT SetHotDivider(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_SETHOTDIVIDER,FALSE,nIndex);
    }

    HIMAGELIST GetImageList(INT nImageList=HDSIL_NORMAL)const{
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,HDM_GETIMAGELIST,nImageList,0);
    }

    HIMAGELIST SetImageList(HIMAGELIST hImageList,INT nImageList=HDSIL_NORMAL){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,HDM_SETIMAGELIST,nImageList,(LPARAM)hImageList);
    }

    HIMAGELIST CreateDragImage(INT nIndex){
      XASSERT(::IsWindow(m_hWnd))
      return (HIMAGELIST)::SendMessage(m_hWnd,HDM_CREATEDRAGIMAGE,nIndex,0);
    }

  #if _WIN32_IE >= 0x0500

    #ifdef HDM_GETBITMAPMARGIN
    INT GetBitmapMargin()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_GETBITMAPMARGIN,0,0);
    }
    #endif

    #ifdef HDM_SETBITMAPMARGIN
    INT SetBitmapMargin(INT nWidth){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_SETBITMAPMARGIN,nWidth,0);
    }
    #endif

    #ifdef HDM_SETFILTERCHANGETIMEOUT
    INT SetFilterChangeTimeout(DWORD dwTimeOut){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_SETFILTERCHANGETIMEOUT,0,dwTimeOut);
    }
    #endif

    #ifdef HDM_EDITFILTER
    INT EditFilter(INT nColumn,BOOL bDiscardChanges){
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_EDITFILTER,nColumn,MAKELPARAM(bDiscardChanges,0));
    }
    #endif

    #ifdef HDM_CLEARFILTER
    BOOL ClearFilter(INT nColumn){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_CLEARFILTER,nColumn,0);
    }

    BOOL ClearAllFilters(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_CLEARFILTER,(WPARAM)-1,0);
    }
    #endif

  #endif

  #if _WIN32_WINNT >= 0x600

    BOOL GetItemDropDownRect(INT iItem,LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_GETITEMDROPDOWNRECT,iItem,(LPARAM)lpRect);
    }

    BOOL GetOverflowRect(LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_GETOVERFLOWRECT,0,(LPARAM)lpRect);
    }

    INT GetFocusedItem()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,HDM_GETFOCUSEDITEM,0,0);
    }

    BOOL SetFocusedItem(INT iItem){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_SETFOCUSEDITEM,0,(LPARAM)iItem);
    }

  #endif

    BOOL SetOrderArray(INT iCount,LPINT piArray){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_SETORDERARRAY,iCount,(LPARAM)piArray);
    }

    BOOL GetOrderArray(LPINT piArray,INT iCount)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,HDM_GETORDERARRAY,iCount,(LPARAM)piArray);
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XHEADER_H_
