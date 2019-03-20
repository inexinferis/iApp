#include "xstaticlink.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XStaticLink,XStatic)
  EVENT_NCHITTEST(XStaticLink::OnNCHitTest)
  EVENT_CTLCOLORSTATIC(XStaticLink::OnCtlColor)
  EVENT_SETCURSOR(XStaticLink::OnSetCursor)
  EVENT_SETFOCUS(XStaticLink::OnFocusChange)
	EVENT_KILLFOCUS(XStaticLink::OnFocusChange)
	EVENT_LBUTTONUP(XStaticLink::OnLButtonUp)
	EVENT_GETDLGCODE(XStaticLink::OnGetDlgCode)
	EVENT_CHAR(XStaticLink::OnChar)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XStaticLink,XStatic)

XStaticLink::XStaticLink(HWND hwnd,COLORREF ColorUnvisited,COLORREF ColorVisited):XStatic(hwnd){
  NONCLIENTMETRICS ncm;
	ncm.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS),&ncm,0);
	ncm.lfMessageFont.lfUnderline=TRUE;
	m_hFont=::CreateFontIndirect(&ncm.lfMessageFont);
	g_ColorUnvisited=ColorUnvisited;
	g_ColorVisited=ColorVisited;
	m_Color=g_ColorUnvisited;
	g_hCursorLink=::LoadCursor(NULL,IDC_HAND);
}

XStaticLink::~XStaticLink(){
  if(m_hFont)
    DeleteObject(m_hFont);
}

LRESULT XStaticLink::OnNCHitTest(POINTS /*pt*/){
  //to handle mouse events
	return HTCLIENT;
}

HBRUSH XStaticLink::OnCtlColor(HDC hDC, UINT /*nCtlColor*/){
	if((GetStyle()&0xFF)<=SS_RIGHT){
		::SelectObject(hDC,m_hFont);
		::SetTextColor(hDC,m_Color);
		::SetBkMode(hDC,TRANSPARENT);
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	return NULL;
}

BOOL XStaticLink::OnSetCursor(HWND /*hWnd*/,DWORD /*flags*/){
  if(g_hCursorLink){
    ::SetCursor(g_hCursorLink);
		return TRUE;
	}
	return FALSE;
}

VOID XStaticLink::OnLButtonUp(UINT /*nFlags*/,POINTS /*pt*/){
  HWND hParent=GetParent();
	XASSERT(hParent)
	::SendMessage(hParent,WM_COMMAND,MAKELPARAM(GetDlgCtrlID(),STN_CLICKED),(LPARAM)m_hWnd);
	m_Color=g_ColorVisited;
	Invalidate();
}

UINT XStaticLink::OnGetDlgCode(DWORD /*dwVKey*/,LPMSG /*pMsg*/){
  //to get onchar notification...
	return DLGC_WANTCHARS;
}

VOID XStaticLink::OnChar(UINT nChar, UINT /*kFlags*/){
	if(nChar==VK_SPACE){
	  HWND hParent=GetParent();
    XASSERT(hParent)
    ::SendMessage(hParent,WM_COMMAND,MAKELPARAM(GetDlgCtrlID(),STN_CLICKED),(LPARAM)m_hWnd);
    m_Color=g_ColorVisited;
    Invalidate();
	}
}

VOID XStaticLink::OnFocusChange(HWND /*hWnd*/){
  HWND hParent=GetParent();
	XASSERT(hParent)
	RECT rc;
	if(GetStyle()&(SS_BITMAP|SS_ICON|SS_ENHMETAFILE|SS_OWNERDRAW))
		GetWindowRect(&rc);
	else {
		UINT len=GetWindowTextLength();
    if(len){
      PTCHAR lpszText=(PTCHAR)malloc(++len*sizeof(TCHAR));
      GetWindowText(lpszText,len);
      XClientDC dc(*this);
      HFONT hOldFont=(HFONT)dc.SelectObject(m_hFont);
      ::SetRectEmpty(&rc);
      dc.DrawText(lpszText,&rc,DT_CALCRECT);
      dc.SelectObject(hOldFont);
      free(lpszText);
      ClientToScreen(&rc);
    }
	}
  ::InflateRect(&rc,2,2);
  XWnd pParent(hParent);
	pParent.ScreenToClient(&rc);
	XClientDC dcParent(hParent);
	dcParent.DrawFocusRect(&rc);
}
