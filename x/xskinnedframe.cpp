#include "xskinnedframe.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedFrame,XFrameWnd)
  EVENT_SETTEXT(XSkinnedFrame::OnSetText)
  EVENT_SETICON(XSkinnedFrame::OnSetIcon)
  EVENT_PRINT(XSkinnedFrame::OnPrint)
/* NC EVENTS */
  EVENT_NCCREATE(XSkinnedFrame::OnNCCreate)
  EVENT_NCCALCSIZE(XSkinnedFrame::OnNCCalcSize)
  EVENT_NCACTIVATE(XSkinnedFrame::OnNCActivate)
  EVENT_NCPAINT(XSkinnedFrame::OnNCPaint)
  EVENT_NCHITTEST(XSkinnedFrame::OnNCHitTest)
  EVENT_NCLBUTTONDOWN(XSkinnedFrame::OnNCLButtonDown)
  EVENT_NCLBUTTONUP(XSkinnedFrame::OnNCLButtonUp)
  EVENT_NCLBUTTONDBLCLK(XSkinnedFrame::OnNCLButtonDbClk)
  EVENT_NCRBUTTONDOWN(XSkinnedFrame::OnNCRButtonDown)
  EVENT_NCMOUSEMOVE(XSkinnedFrame::OnNCMouseMove)
  EVENT_NCDESTROY(XSkinnedFrame::OnNCDestroy)
/* NC EVENTS */
  EVENT_INITMENUPOPUP(XSkinnedFrame::OnInitMenuPopup)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedFrame,XFrameWnd)

BOOL XSkinnedFrame::PreCreateWindow(LPCREATESTRUCT wcs){
  if(wcs->lpszClass==NULL)
		wcs->lpszClass=XDEFAULTSKINNEDWNDCLASSNAME;
  SkinPreCreateWindow(wcs);
  return TRUE;
}

BOOL XSkinnedFrame::PreRegisterClass(LPWNDCLASSEX wcx){
  if(wcx->lpszClassName==NULL)
		wcx->lpszClassName=XDEFAULTSKINNEDWNDCLASSNAME;
  SkinPreRegisterClass(wcx);
  return TRUE;
}

BOOL XSkinnedFrame::PreTranslateMessage(LPMSG lpMsg){
  SkinPreTranslateMessage(lpMsg);
  return XFrameWnd::PreTranslateMessage(lpMsg);
}

/*****************************************************************************\
  Events!
\*****************************************************************************/

HICON XSkinnedFrame::OnSetIcon(BOOL bBig,HICON hIcon){
  DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE){
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
    HICON bRet=(HICON)Default();
		SetWindowLong(GWL_STYLE,dwStyle);
    SkinSetIcon(bBig,hIcon);
    return bRet;
  }
  return (HICON)Default();
}

BOOL XSkinnedFrame::OnSetText(LPSTR swText){
  DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE){
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
    BOOL bRet=Default();
		SetWindowLong(GWL_STYLE,dwStyle);
    SkinSetText(swText);
    return bRet;
  }
  return Default();
}

VOID XSkinnedFrame::OnPrint(HDC hDC,UINT uFlags){
  SkinPrint(hDC,uFlags);
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

BOOL XSkinnedFrame::OnNCCreate(LPCREATESTRUCT cs){
  SkinNCCreate(cs);
  return Default();
}

BOOL XSkinnedFrame::OnNCActivate(BOOL bActivate,HRGN hRgn){
  DWORD dwStyle=GetStyle();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,(dwStyle&~WS_VISIBLE));
	BOOL bRet=Default();
	if(dwStyle&WS_VISIBLE)
		SetWindowLong(GWL_STYLE,dwStyle);
  SkinNCActivate(bActivate,hRgn);
  return bRet;
  //return SkinNCActivate(bActivate,hRgn);
}

VOID XSkinnedFrame::OnNCPaint(HRGN hRgn){
  SkinNCPaint(hRgn);
}

INT XSkinnedFrame::OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams){
  return SkinNCCalcSize(bValidCA,pParams);
}

LRESULT XSkinnedFrame::OnNCHitTest(POINTS pt){
  return SkinNCHitTest(pt);
}

VOID XSkinnedFrame::OnNCLButtonDown(UINT iHittest,POINTS pt){
  SkinNCLButtonDown(iHittest,pt);
}

VOID XSkinnedFrame::OnNCLButtonUp(UINT iHittest,POINTS pt){
  SkinNCLButtonUp(iHittest,pt);
}

VOID XSkinnedFrame::OnNCLButtonDbClk(UINT iHittest,POINTS pt){
  SkinNCLButtonDbClk(iHittest,pt);
}

VOID XSkinnedFrame::OnNCRButtonDown(UINT iHittest,POINTS pt){
  SkinNCRButtonDown(iHittest,pt);
}

VOID XSkinnedFrame::OnNCMouseMove(UINT iHittest,POINTS pt){
  SkinNCMouseMove(iHittest,pt);
}

VOID XSkinnedFrame::OnNCDestroy(){
  SkinNCDestroy();
}

/*****************************************************************************\
  NC EVENTS
\*****************************************************************************/

VOID XSkinnedFrame::OnInitMenuPopup(HMENU hMenu,DWORD info){
  SkinInitMenuPopup(hMenu,info);
}

/*****************************************************************************\
  END MENUS
\*****************************************************************************/

INT XSkinnedFrame::MessageBox(LPCTSTR lpText,LPCTSTR lpCaption,UINT uType,UINT uTimeout){
  return m_SMB->MessageBox(*this,lpText,lpCaption,uType,uTimeout);
}

/*****************************************************************************\
  MessageBox
\*****************************************************************************/
