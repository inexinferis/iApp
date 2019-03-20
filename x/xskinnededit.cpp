#include "xskinnededit.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedEdit,XEdit)
  EVENT_CREATE(XSkinnedEdit::OnCreate)
  EVENT_NCPAINT(XSkinnedEdit::OnNCPaint)
  EVENT_NCCALCSIZE(XSkinnedEdit::OnNCCalcSize)
  EVENT_CTLCOLOREDIT(XSkinnedEdit::OnCtlColorEdit)
  EVENT_SETFOCUS(XSkinnedEdit::OnSetFocus)
  EVENT_KILLFOCUS(XSkinnedEdit::OnKillFocus)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedEdit,XEdit)

XSkinnedEdit::XSkinnedEdit(HWND hwnd):XEdit(hwnd){
  m_hFont.Create(XFONT_SIZE,0,0,0,FALSE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
  m_hBrush.CreateSolid(XCOLOR_BACKGROUND);
  m_bActive=FALSE;m_dwStyle=0;
}

XSkinnedEdit::~XSkinnedEdit(){
  m_hFont.Delete();
  m_hBrush.Delete();
}

INT XSkinnedEdit::OnCreate(LPCREATESTRUCT /*pcs*/){
  INT ret=Default();
  SetFont(m_hFont);
  return ret;
}

HBRUSH XSkinnedEdit::OnCtlColorEdit(HDC hDC,HWND /*hCtrl*/){
  XDC WDC(hDC);
  WDC.SetTextColor(XCOLOR_TEXT_ACTIVE);
  WDC.SetBkMode(TRANSPARENT);
  return m_hBrush;
}

INT XSkinnedEdit::OnNCCalcSize(BOOL /*bValidCA*/,NCCALCSIZE_PARAMS *pParams){
  //change non client area size...
  RECT* lprc=(RECT*)pParams;
  if(lprc&&!::IsRectEmpty(lprc))
    InflateRect(lprc,-XEDIT_MARGIN,-XEDIT_MARGIN);
  return 0;
}

VOID XSkinnedEdit::OnSetFocus(HWND /*hWndLost*/){
  m_bActive=TRUE;
  OnNCPaint(NULL);
	Default();
}

VOID XSkinnedEdit::OnKillFocus(HWND /*hWndRcv*/){
  m_bActive=FALSE;
  OnNCPaint(NULL);
  Default();
}

VOID XSkinnedEdit::OnNCPaint(HRGN /*hRgn*/){
  RECT wr;
  XWindowDC WDC(*this);

  GetWindowRect(&wr);
  OffsetRect(&wr,-wr.left,-wr.top);

  //Draw Background
  WDC.FrameSolidRect(&wr,m_bActive?XCOLOR_HOVER:XCOLOR_NORMAL);
}
