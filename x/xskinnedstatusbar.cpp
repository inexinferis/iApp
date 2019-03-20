#include "xskinnedstatusbar.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedStatusBar,XStatusBar)
  EVENT_CREATE(XSkinnedStatusBar::OnCreate)
  EVENT_DRAWITEM_EX(XSkinnedStatusBar::OnDrawItem,TRUE)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedStatusBar,XStatusBar)

INT XSkinnedStatusBar::OnCreate(LPCREATESTRUCT pcs){
  INT ret=0;
  DWORD dwStyle=::GetWindowLongPtr(pcs->hwndParent,GWL_STYLE);
  //remove sizebox...
  if(dwStyle&WS_SIZEBOX){
    ::SetWindowLongPtr(pcs->hwndParent,GWL_STYLE,dwStyle&~WS_SIZEBOX);
    ret=Default();
    ::SetWindowLongPtr(pcs->hwndParent,GWL_STYLE,dwStyle);
  }else
    ret=Default();
  //SetFont(m_hFont);
  SetBkColor(XCOLOR_BACKGROUND);
  DisableWindowTheme();
  return ret;
}

BOOL XSkinnedStatusBar::OnDrawItem(UINT /*uCtrlID*/,PDRAWITEMSTRUCT pDIS){
  XDC WDC(pDIS->hDC);
  XRect wr(pDIS->rcItem);
  WDC.DrawGradient(wr,XCOLOR_BACKGROUND2,XCOLOR_BACKGROUND,1);
  //Draw Text
  LPCSTR ptStr=(PTSTR)(DWORD_PTR)pDIS->itemData;
  if(ptStr){
    wr.InflateRect(-5,-2);
    WDC.DrawColoredText(ptStr,&wr,TRANSPARENT,0,XCOLOR_TEXT_ACTIVE,DT_WORD_ELLIPSIS|DT_NOCLIP|DT_VCENTER|DT_SINGLELINE|DT_LEFT);
  }
  return TRUE;
}
