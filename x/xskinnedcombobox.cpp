#include "xskinnedcombobox.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedComboBox,XComboBox)
  EVENT_CREATE(XSkinnedComboBox::OnCreate)
  EVENT_CTLCOLOREDIT(XSkinnedComboBox::OnCtlColorEdit)
  EVENT_CTLCOLORLISTBOX(XSkinnedComboBox::OnCtlColorListBox)
  EVENT_NCPAINT(XSkinnedComboBox::OnNCPaint)
  EVENT_PAINT(XSkinnedComboBox::OnPaint)
  EVENT_SETFOCUS(XSkinnedComboBox::OnSetFocus)
  EVENT_KILLFOCUS(XSkinnedComboBox::OnKillFocus)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedComboBox,XComboBox)

XSkinnedComboBox::XSkinnedComboBox(HWND hwnd):XComboBox(hwnd){
  m_hFont.Create(XFONT_SIZE,0,0,0,FALSE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
  m_hBrush.CreateSolid(XCOLOR_BACKGROUND);
  m_bActive=FALSE;m_dwStyle=0;
}

XSkinnedComboBox::~XSkinnedComboBox(){
  m_hFont.Delete();
  m_hBrush.Delete();
}

INT XSkinnedComboBox::OnCreate(LPCREATESTRUCT /*pcs*/){
  INT ret=Default();
  SetFont(m_hFont);
  return ret;
}

HBRUSH XSkinnedComboBox::OnCtlColorEdit(HDC hDC,HWND /*hCtrl*/){
  XDC WDC(hDC);
  WDC.SetTextColor(XCOLOR_TEXT_ACTIVE);
  WDC.SetBkColor(XCOLOR_BACKGROUND);
  WDC.SetBkMode(TRANSPARENT);
  return m_hBrush;
}

HBRUSH XSkinnedComboBox::OnCtlColorListBox(HDC hDC,HWND /*hCtrl*/){
  XDC WDC(hDC);
  WDC.SetTextColor(XCOLOR_TEXT_ACTIVE);
  WDC.SetBkMode(TRANSPARENT);
  return m_hBrush;
}

VOID XSkinnedComboBox::OnSetFocus(HWND /*hWndLost*/){
  m_bActive=TRUE;
  OnNCPaint(NULL);
  Default();
}

VOID XSkinnedComboBox::OnKillFocus(HWND /*hWndRcv*/){
  m_bActive=FALSE;
  OnNCPaint(NULL);
  Default();
}

VOID XSkinnedComboBox::OnNCPaint(HRGN /*hRgn*/){
  RECT wr;
  XWindowDC WDC(*this);
  GetWindowRect(&wr);
  OffsetRect(&wr,-wr.left,-wr.top);
  WDC.FrameSolidRect(&wr,m_bActive?XCOLOR_HOVER:XCOLOR_NORMAL);
  //force repaint!
  RedrawWindow();
}

VOID XSkinnedComboBox::OnPaint(){
  XPaintDC dc(*this);

  RECT rButton;POINT pt;
  GetClientRect(&rButton);
  GetCursorPos(&pt);
  ScreenToClient(&pt);

  BOOL bClicked=((GetAsyncKeyState(MK_LBUTTON)&0x8000)&&PtInRect(&rButton,pt));
  dc.DrawSolidLine(rButton.right-18,rButton.top+3,rButton.right-18,rButton.bottom-3,m_bActive?XCOLOR_HOVER:XCOLOR_NORMAL);
  dc.DrawSpecialChar(rButton.right-16,rButton.top+3,'6',(bClicked)?XCOLOR_HOVER:XCOLOR_NORMAL);
  //draw text!
  if(GetWindowTextLength()){
    TCHAR sText[256];
    GetWindowText(sText,256);
    dc.SetTextColor(XCOLOR_TEXT_ACTIVE);
    dc.SetBkMode(TRANSPARENT);
    HFONT hOldFont=(HFONT)dc.SelectObject((HFONT)m_hFont);
    rButton.left+=4;rButton.top+=3;
    rButton.right-=20;rButton.bottom-=3;
    dc.FillSolidRect(&rButton,XCOLOR_BACKGROUND);
    dc.DrawText(sText,-1,&rButton,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOCLIP);
    dc.SelectObject(hOldFont);
  }
}
