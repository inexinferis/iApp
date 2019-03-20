#ifndef _XSKINNEDTREEVIEW_H_
#define _XSKINNEDTREEVIEW_H_

#include "xtreeview.h"
#include "xcoords.h"
#include "xgdi.h"
#include "ximagelist.h"
#include "xskinnedwnd.h"

#define WM_XTVSELCHANGED          WM_USER+0x0101 //A Default Notify Tree Change
#define EVENT_XTVSELCHANGED(pfun) \
	EVENT_MESSAGE_EX(WM_XTVSELCHANGED,TVL,(VOID(XEvent::*)(UINT,UINT))(&pfun))

class XSkinnedTreeView:public XTreeView{
  DECLARE_DYNAMIC(XSkinnedTreeView)
  public:
    XSkinnedTreeView(HWND hwnd=NULL);
    virtual ~XSkinnedTreeView();

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTREEVIEWSTYLE,LPVOID lpParam=NULL){
      //dwStyle=(dwStyle&~(TVS_TYPEMASK|LVS_SHOWSELALWAYS|LVS_EDITLABELS))| // this styles are removed
        //(LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_NOCOLUMNHEADER|LVS_SINGLESEL); // this styles are added
      HWND hWnd=XTreeView::Create(pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
      return hWnd;
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFTREEVIEWSTYLE,LPVOID lpParam=NULL){
      dwStyle=(dwStyle&~(LVS_TYPEMASK|LVS_SHOWSELALWAYS|LVS_EDITLABELS))| // this styles are removed
        (LVS_REPORT|LVS_OWNERDRAWFIXED|LVS_NOCOLUMNHEADER|LVS_SINGLESEL); // this styles are added
      HWND hWnd=XTreeView::CreateEx(dwStyleEx,pParentWnd,lpszWindowName,CtrlId,rect,dwStyle,lpParam);
      return hWnd;
    }

    VOID UpdateClient(INT x,INT y,INT cx,INT cy);

    INT OnCreate(LPCREATESTRUCT pcs);
    INT OnCustomDraw(LPNMTVCUSTOMDRAW lpnmcd);
    BOOL OnSelChanging(LPNMTREEVIEW ptv);
    VOID OnSelChanged(LPNMTREEVIEW ptv);

  private:

    XFont m_hFont;XWnd m_Client;
    INT m_x,m_y,m_cx,m_cy;

  DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDTREEVIEW_H_
