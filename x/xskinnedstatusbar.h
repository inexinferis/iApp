#ifndef _XSKINNEDSTATUSBAR_H_
#define _XSKINNEDSTATUSBAR_H_

#include "xstatusbar.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

class XSkinnedStatusBar:public XStatusBar{
  DECLARE_DYNAMIC(XSkinnedStatusBar)
  public:
    XSkinnedStatusBar(HWND hwnd=NULL):XStatusBar(hwnd){}
    virtual ~XSkinnedStatusBar(){}

    BOOL SetText(LPCTSTR lpszText,INT nPane=0,INT nType=0){
      return XStatusBar::SetText(lpszText,nPane,nType|SBT_NOBORDERS|SBT_OWNERDRAW);
    }

    INT OnCreate(LPCREATESTRUCT pcs);
    BOOL OnDrawItem(UINT uCtrlID,PDRAWITEMSTRUCT pDIS);

  private:
    DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDSTATUSBAR_H_
