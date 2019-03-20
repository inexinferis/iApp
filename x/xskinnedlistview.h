#ifndef _XSKINNEDLISTVIEW_H_
#define _XSKINNEDLISTVIEW_H_

#include "xlistview.h"
#include "xcoords.h"
#include "xgdi.h"
#include "ximagelist.h"
#include "xskinnedwnd.h"

class XSkinnedListView:public XListView{
  DECLARE_DYNAMIC(XSkinnedListView)
  public:
    XSkinnedListView(HWND hwnd=NULL);
    virtual ~XSkinnedListView();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTVIEWSTYLE,LPVOID lpParam=NULL){
      dwStyle=(dwStyle&~(LVS_TYPEMASK|LVS_SHOWSELALWAYS|LVS_EDITLABELS))| // this styles are removed
        (LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_NOCOLUMNHEADER|LVS_SINGLESEL); // this styles are added
      HWND hWnd=XListView::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
      if(hWnd){
        InsertColumn(0,_T("Column"),LVCFMT_LEFT);//,LVSCW_AUTOSIZE_USEHEADER
        SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
      }
      return hWnd;
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLISTVIEWSTYLE,LPVOID lpParam=NULL){
      dwStyle=(dwStyle&~(LVS_TYPEMASK|LVS_SHOWSELALWAYS|LVS_EDITLABELS))| // this styles are removed
        (LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_NOCOLUMNHEADER|LVS_SINGLESEL); // this styles are added
      HWND hWnd=XListView::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
      if(hWnd){
        InsertColumn(0,_T("Column"),LVCFMT_LEFT);//,LVSCW_AUTOSIZE_USEHEADER
        SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
      }
      return hWnd;
    }

    INT InsertItem(INT nItem,LPCTSTR lpszItem,INT nImage,XWnd *pWin){
      XASSERT(::IsWindow(m_hWnd))
      LVITEM item;item.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;item.iItem=nItem;item.iSubItem=0;item.pszText=(LPTSTR)lpszItem;
      item.state=0;item.stateMask=0;item.iImage=nImage;item.lParam=(LPARAM)pWin;
      return (INT)::SendMessage(m_hWnd,LVM_INSERTITEM,0,(LPARAM)&item);
    }

    VOID UpdateClient(INT x,INT y,INT cx,INT cy);

    VOID OnMeasureItem(UINT uCtrlID,LPMEASUREITEMSTRUCT lpmis);
    BOOL OnDrawItem(UINT uCtrlID,PDRAWITEMSTRUCT pDIS);
    VOID OnItemChanged(LPNMLISTVIEW plv);

  private:

    XFont m_hFont;BOOL bNeedChange;
    INT m_x,m_y,m_cx,m_cy;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDLISTVIEW_H_
