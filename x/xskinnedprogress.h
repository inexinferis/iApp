#ifndef _XSKINNEDPROGRESS_H_
#define _XSKINNEDPROGRESS_H_

#include "xprogress.h"
#include "xcoords.h"
#include "xgdi.h"
#include "xskinnedwnd.h"

class XSkinnedProgress:public XProgress{
  DECLARE_DYNAMIC(XSkinnedProgress)
  public:
    XSkinnedProgress(HWND hwnd=NULL):XProgress(hwnd){}
    virtual ~XSkinnedProgress(){}

    INT  OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams);
    VOID OnNCPaint(HRGN hRgn);

  private:

    DECLARE_EVENT_TABLE()
};

#endif // _XSKINNEDPROGRESS_H_
