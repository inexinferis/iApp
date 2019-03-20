#include "xskinnedmsgbox.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedMsgBox,XMsgBox)
  EVENT_NCCREATE(XSkinnedMsgBox::OnNCCreate)
  EVENT_NCCALCSIZE(XSkinnedMsgBox::OnNCCalcSize)
  EVENT_NCACTIVATE(XSkinnedMsgBox::OnNCActivate)
  EVENT_NCPAINT(XSkinnedMsgBox::OnNCPaint)
  EVENT_NCHITTEST(XSkinnedMsgBox::OnNCHitTest)
  EVENT_NCLBUTTONDOWN(XSkinnedMsgBox::OnNCLButtonDown)
  EVENT_NCLBUTTONUP(XSkinnedMsgBox::OnNCLButtonUp)
  EVENT_NCLBUTTONDBLCLK(XSkinnedMsgBox::OnNCLButtonDbClk)
  EVENT_NCRBUTTONDOWN(XSkinnedMsgBox::OnNCRButtonDown)
  EVENT_NCMOUSEMOVE(XSkinnedMsgBox::OnNCMouseMove)
  EVENT_NCDESTROY(XSkinnedMsgBox::OnNCDestroy)
/* NC EVENTS */
  EVENT_INITMENUPOPUP(XSkinnedMsgBox::OnInitMenuPopup)
END_EVENT_TABLE()

BOOL XSkinnedMsgBox::PreTranslateMessage(LPMSG lpMsg){
  return XDialog::PreTranslateMessage(lpMsg);
}

BOOL XSkinnedMsgBox::PreCreateWindow(LPCREATESTRUCT wcs){
  SkinPreCreateWindow(wcs);
  return XDialog::PreCreateWindow(wcs);
}

BOOL XSkinnedMsgBox::PreRegisterClass(LPWNDCLASSEX wcx){
  SkinPreRegisterClass(wcx);
  return XDialog::PreRegisterClass(wcx);
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

BOOL XSkinnedMsgBox::OnNCCreate(LPCREATESTRUCT cs){
  SkinNCCreate(cs);
  return Default();
}

BOOL XSkinnedMsgBox::OnNCActivate(BOOL bActivate,HRGN hRgn){
	DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
	BOOL bRet=Default();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,dwStyle);
  SkinNCActivate(bActivate,hRgn);
  return bRet;
}

VOID XSkinnedMsgBox::OnNCPaint(HRGN hRgn){
  SkinNCPaint(hRgn);
}

INT XSkinnedMsgBox::OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams){
  return SkinNCCalcSize(bValidCA,pParams);
}

LRESULT XSkinnedMsgBox::OnNCHitTest(POINTS pt){
  return SkinNCHitTest(pt);
}

VOID XSkinnedMsgBox::OnNCLButtonDown(UINT iHittest,POINTS pt){
  SkinNCLButtonDown(iHittest,pt);
}

VOID XSkinnedMsgBox::OnNCLButtonUp(UINT iHittest,POINTS pt){
  SkinNCLButtonUp(iHittest,pt);
}

VOID XSkinnedMsgBox::OnNCLButtonDbClk(UINT iHittest,POINTS pt){
  SkinNCLButtonDbClk(iHittest,pt);
}

VOID XSkinnedMsgBox::OnNCRButtonDown(UINT iHittest,POINTS pt){
  SkinNCRButtonDown(iHittest,pt);
}

VOID XSkinnedMsgBox::OnNCMouseMove(UINT iHittest,POINTS pt){
  SkinNCMouseMove(iHittest,pt);
}

VOID XSkinnedMsgBox::OnNCDestroy(){
  SkinNCDestroy();
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

VOID XSkinnedMsgBox::OnInitMenuPopup(HMENU hMenu,DWORD info){
  SkinInitMenuPopup(hMenu,info);
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/
