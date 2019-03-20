#include "xskinnedstatic.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedStatic,XStatic)
  EVENT_SETTEXT(XSkinnedStatic::OnSetText)
  EVENT_STN_CLICKED(XTHISCTRLID,XSkinnedStatic::OnClicked)
  //just has child!
  EVENT_DRAWITEM_EX(XSkinnedStatic::OnDrawItem,FALSE)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedStatic,XStatic)

XSkinnedStatic::XSkinnedStatic(HWND hwnd):XStatic(hwnd){
  m_hFont.Create(XFONT_SIZE,0,0,0,FALSE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
  m_dwStyle=0;m_hClient=NULL;m_hIcon=NULL;m_hBmp=NULL;
}

XSkinnedStatic::~XSkinnedStatic(){
  m_hFont.Delete();
}

VOID XSkinnedStatic::SetFontSize(UINT uFs){
  m_hFont.Delete();
  m_hFont.Create(uFs,0,0,0,FALSE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
}

BOOL XSkinnedStatic::OnSetText(LPSTR /*swText*/){
  BOOL bRet=Default();
  HWND hWndParent=::GetParent(m_hWnd);
  if(hWndParent){
    XRECT rc;
    GetClientRect(&rc);
    MapWindowPoints(hWndParent,&rc);
    ::InvalidateRect(hWndParent,&rc,TRUE);
    ::UpdateWindow(hWndParent);
  }
  return bRet;
}

BOOL XSkinnedStatic::OnDrawItem(UINT /*uCtrlID*/,PDRAWITEMSTRUCT pDIS){
  XDC WDC(pDIS->hDC);
  XRect wr(pDIS->rcItem);
  if(MASKENUM(m_dwStyle,SS_BITMAP)){
    HBITMAP hBitmap=GetBitmap();
    if(hBitmap){
      XDC WMem;
      WMem.CreateCompatible(WDC);
      WMem.SelectObject(hBitmap);
      WDC.BitBlt(wr.left,wr.top,wr.right-wr.left,wr.bottom-wr.top,WMem,0,0,SRCCOPY);
    }
  }else if(MASKENUM(m_dwStyle,SS_ICON)){
    HICON hIcon=GetIcon();
    if(hIcon)
      WDC.DrawIconEx(wr.left,wr.top,hIcon,0,0,0,0,DI_NORMAL|DI_DEFAULTSIZE);
  }else{
    //Draw Text
    UINT len=GetWindowTextLength();
    if(len++){
      PTCHAR lpszText=(PTCHAR)malloc(len*sizeof(TCHAR));
      if(GetWindowText(lpszText,len)){
        HFONT hOldFont=(HFONT)WDC.SelectObject(m_hFont);
        DWORD dwTextFlags=DT_WORD_ELLIPSIS|DT_NOCLIP;

        if(MASKENUM(m_dwStyle,SS_LEFT))dwTextFlags|=DT_LEFT;
        if(MASKENUM(m_dwStyle,SS_CENTER))dwTextFlags|=DT_CENTER;
        if(MASKENUM(m_dwStyle,SS_RIGHT))dwTextFlags|=DT_RIGHT;
        if(m_dwStyle&SS_VCENTER)dwTextFlags|=DT_VCENTER|DT_SINGLELINE;

        WDC.DrawColoredText(lpszText,&wr,TRANSPARENT,0,XCOLOR_TEXT_ACTIVE,dwTextFlags);
        WDC.SelectObject(hOldFont);
      }
      free(lpszText);
    }
  }
  return TRUE;
}

VOID XSkinnedStatic::OnClicked(DWORD /*dwParams*/,HWND /*hWnd*/){
  if(m_hClient&&m_hClient->IsValid())
    m_hClient->SetFocus();
}
