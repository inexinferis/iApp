#include "xskinnedprogress.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedProgress,XProgress)
  EVENT_NCCALCSIZE(XSkinnedProgress::OnNCCalcSize)
  EVENT_NCPAINT(XSkinnedProgress::OnNCPaint)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedProgress,XProgress)

INT XSkinnedProgress::OnNCCalcSize(BOOL /*bValidCA*/,NCCALCSIZE_PARAMS *pParams){
  //change non client area size...
  RECT* lprc=(RECT*)pParams;
  if(lprc&&!::IsRectEmpty(lprc))
    InflateRect(lprc,-1,-1);
  return 0;
}

VOID XSkinnedProgress::OnNCPaint(HRGN /*hRgn*/){
  XWindowDC WDC(*this);RECT wr;
  GetWindowRect(&wr);
  OffsetRect(&wr,-wr.left,-wr.top);
  WDC.FrameSolidRect(&wr,XCOLOR_BACKGROUND_INACTIVE);
}
