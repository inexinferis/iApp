#include "xskinneddlg.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedDlg,XDialog)
  EVENT_SETTEXT(XSkinnedDlg::OnSetText)
/* NC EVENTS */
  EVENT_NCCREATE(XSkinnedDlg::OnNCCreate)
  EVENT_NCCALCSIZE(XSkinnedDlg::OnNCCalcSize)
  EVENT_NCACTIVATE(XSkinnedDlg::OnNCActivate)
  EVENT_NCPAINT(XSkinnedDlg::OnNCPaint)
  EVENT_NCHITTEST(XSkinnedDlg::OnNCHitTest)
  EVENT_NCLBUTTONDOWN(XSkinnedDlg::OnNCLButtonDown)
  EVENT_NCLBUTTONUP(XSkinnedDlg::OnNCLButtonUp)
  EVENT_NCLBUTTONDBLCLK(XSkinnedDlg::OnNCLButtonDbClk)
  EVENT_NCRBUTTONDOWN(XSkinnedDlg::OnNCRButtonDown)
  EVENT_NCMOUSEMOVE(XSkinnedDlg::OnNCMouseMove)
  EVENT_NCDESTROY(XSkinnedDlg::OnNCDestroy)
/* NC EVENTS */
  EVENT_INITMENUPOPUP(XSkinnedDlg::OnInitMenuPopup)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedDlg,XDialog)

BOOL XSkinnedDlg::PreTranslateMessage(LPMSG lpMsg){
  return XDialog::PreTranslateMessage(lpMsg);
}

BOOL XSkinnedDlg::PreCreateWindow(LPCREATESTRUCT wcs){
  SkinPreCreateWindow(wcs);
  return XDialog::PreCreateWindow(wcs);
}

BOOL XSkinnedDlg::PreRegisterClass(LPWNDCLASSEX wcx){
  SkinPreRegisterClass(wcx);
  return XDialog::PreRegisterClass(wcx);
}

/*****************************************************************************\
  Events!
\*****************************************************************************/

BOOL XSkinnedDlg::OnSetText(LPSTR swText){
  DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
	BOOL bRet=Default();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,dwStyle);
  SkinSetText(swText);
  return bRet;
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

BOOL XSkinnedDlg::OnNCCreate(LPCREATESTRUCT cs){
  SkinNCCreate(cs);
  return Default();
}

BOOL XSkinnedDlg::OnNCActivate(BOOL bActivate,HRGN hRgn){
	DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
	BOOL bRet=Default();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,dwStyle);
  SkinNCActivate(bActivate,hRgn);
  return bRet;
}

VOID XSkinnedDlg::OnNCPaint(HRGN hRgn){
  SkinNCPaint(hRgn);
}

INT XSkinnedDlg::OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams){
  return SkinNCCalcSize(bValidCA,pParams);
}

LRESULT XSkinnedDlg::OnNCHitTest(POINTS pt){
  return SkinNCHitTest(pt);
}

VOID XSkinnedDlg::OnNCLButtonDown(UINT iHittest,POINTS pt){
  SkinNCLButtonDown(iHittest,pt);
}

VOID XSkinnedDlg::OnNCLButtonUp(UINT iHittest,POINTS pt){
  SkinNCLButtonUp(iHittest,pt);
}

VOID XSkinnedDlg::OnNCLButtonDbClk(UINT iHittest,POINTS pt){
  SkinNCLButtonDbClk(iHittest,pt);
}

VOID XSkinnedDlg::OnNCRButtonDown(UINT iHittest,POINTS pt){
  SkinNCRButtonDown(iHittest,pt);
}

VOID XSkinnedDlg::OnNCMouseMove(UINT iHittest,POINTS pt){
  SkinNCMouseMove(iHittest,pt);
}

VOID XSkinnedDlg::OnNCDestroy(){
  SkinNCDestroy();
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

VOID XSkinnedDlg::OnInitMenuPopup(HMENU hMenu,DWORD info){
  SkinInitMenuPopup(hMenu,info);
}

/*****************************************************************************\
  END MENUS
\*****************************************************************************/

INT XSkinnedDlg::MessageBox(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,UINT uTimeout){
  return m_SMB->MessageBox(*this,lpText,lpCaption,uType,uTimeout);
}

/*****************************************************************************\
  MessageBox
\*****************************************************************************/
