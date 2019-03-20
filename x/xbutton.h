#ifndef _XBUTTON_H_
#define _XBUTTON_H_

#include "xwnd.h"

#define XDEFBUTTONSTYLE WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN

class XButton:public XWnd{
  DECLARE_DYNAMIC(XButton)
  public:
    XButton(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XButton(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFBUTTONSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_BUTTON,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFBUTTONSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_BUTTON,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    HBITMAP GetBitmap() const{
      XASSERT(::IsWindow(m_hWnd))
      return (HBITMAP)::SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_BITMAP,0);
    }

    UINT GetButtonStyle() const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)GetWindowLong(GWL_STYLE)&0xFFFF;
    }

    INT GetCheck() const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,BM_GETCHECK,0,0);
    }

    HCURSOR GetCursor(){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_CURSOR,0);
    }

    HICON SetIcon(HICON hIcon){
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,BM_SETIMAGE,IMAGE_ICON,(LPARAM)hIcon);
    }

    HICON GetIcon()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,BM_GETIMAGE,IMAGE_ICON,0);
    }

    VOID SetState(BOOL bHighlight){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,BM_SETSTATE,bHighlight,0);
    }

    UINT GetState()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,BM_GETSTATE,0,0);
    }

    HBITMAP SetBitmap(HBITMAP hBitmap){
      XASSERT(::IsWindow(m_hWnd))
      return (HBITMAP)::SendMessage(m_hWnd,BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hBitmap);
    }

    VOID SetButtonStyle(UINT nStyle,BOOL bRedraw){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,BM_SETSTYLE,nStyle,(LPARAM)bRedraw);
    }

    VOID SetCheck(int nCheck){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,BM_SETCHECK,nCheck,0);
    }

    HCURSOR SetCursor(HCURSOR hCursor){
      XASSERT(::IsWindow(m_hWnd))
      return (HCURSOR)::SendMessage(m_hWnd,BM_SETIMAGE,IMAGE_CURSOR,(LPARAM)hCursor);
    }

  #if _WIN32_WINNT >= 0x501

    BOOL GetIdealSize(LPSIZE psize) const {
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_GETIDEALSIZE,0,(LPARAM)psize);
    }

    BOOL GetImageList(PBUTTON_IMAGELIST pbuttonImagelist) const {
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_GETIMAGELIST,0,(LPARAM)pbuttonImagelist);
    }

    BOOL GetTextMargin(LPRECT pmargin) const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_GETTEXTMARGIN,0,(LPARAM)pmargin);
    }

    BOOL SetImageList(PBUTTON_IMAGELIST pbuttonImagelist){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_SETIMAGELIST,0,(LPARAM)pbuttonImagelist);
    }

    BOOL SetTextMargin(LPRECT pmargin){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_SETTEXTMARGIN,0,(LPARAM)pmargin);
    }

  #endif

  #if _WIN32_WINNT >= 0x0600

    BOOL SetDropDownState(BOOL fDropDown){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_SETDROPDOWNSTATE,fDropDown,0);
    }

    HICON SetShield(BOOL fElevationRequired){
      XASSERT(::IsWindow(m_hWnd))
      return (HICON)::SendMessage(m_hWnd,BCM_SETSHIELD,0,(LPARAM)fElevationRequired);
    }

    BOOL GetNote(LPTSTR lpszNote,PUINT pcchNote)const{
      XASSERT(::IsWindow(m_hWnd)&&lpszNote!=NULL&&pcchNote!=NULL)
      return (BOOL)::SendMessage(m_hWnd,BCM_GETNOTE,(WPARAM)pcchNote,(LPARAM)lpszNote);
    }

    BOOL SetNote(LPCTSTR lpszNote){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_SETNOTE,0,(LPARAM)lpszNote);
    }

    UINT GetNoteLength()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,BCM_GETNOTELENGTH,0,0);
    }

    BOOL GetSplitInfo(PBUTTON_SPLITINFO pInfo)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_GETSPLITINFO,0,(LPARAM)pInfo);
    }

    BOOL SetSplitInfo(PBUTTON_SPLITINFO pInfo){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,BCM_SETSPLITINFO,0,(LPARAM)pInfo);
    }

    UINT GetSplitStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_STYLE;
      ::SendMessage(m_hWnd,BCM_GETSPLITINFO,0,(LPARAM)&bSplitInfo);
      return bSplitInfo.uSplitStyle;
    }

    BOOL SetSplitStyle(UINT nStyle){
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_STYLE;
      bSplitInfo.uSplitStyle=nStyle;
      return (BOOL)::SendMessage(m_hWnd,BCM_SETSPLITINFO,0,(LPARAM)&bSplitInfo);
    }

    BOOL GetSplitSize(LPSIZE pSize)const{
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_SIZE;
      BOOL fRet=(BOOL)::SendMessage(m_hWnd,BCM_GETSPLITINFO,0,(LPARAM)&bSplitInfo);
      pSize->cx=bSplitInfo.size.cx;
      pSize->cy=bSplitInfo.size.cy;
      return fRet;
    }

    BOOL SetSplitSize(LPSIZE pSize){
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_SIZE;
      bSplitInfo.size.cx=pSize->cx;
      bSplitInfo.size.cy=pSize->cy;
      return (BOOL)::SendMessage(m_hWnd,BCM_SETSPLITINFO,0,(LPARAM)&bSplitInfo);
    }

    HIMAGELIST GetSplitImageList()const{
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_IMAGE;
      ::SendMessage(m_hWnd,BCM_GETSPLITINFO,0,(LPARAM)&bSplitInfo);
      return bSplitInfo.himlGlyph;
    }

    BOOL SetSplitImageList(HIMAGELIST hSplitImageList){
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_IMAGE;
      bSplitInfo.himlGlyph=hSplitImageList;
      return (BOOL)::SendMessage(m_hWnd,BCM_SETSPLITINFO,0,(LPARAM)&bSplitInfo);
    }

    TCHAR GetSplitGlyph()const{
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_GLYPH;
      ::SendMessage(m_hWnd,BCM_GETSPLITINFO,0,(LPARAM)&bSplitInfo);
      return (TCHAR)(INT_PTR)bSplitInfo.himlGlyph;
    }

    BOOL SetSplitGlyph(TCHAR chGlyph){
      XASSERT(::IsWindow(m_hWnd))
      BUTTON_SPLITINFO bSplitInfo={0};
      bSplitInfo.mask=BCSIF_GLYPH;
      bSplitInfo.himlGlyph=(HIMAGELIST)(INT_PTR)chGlyph;
      return (BOOL)::SendMessage(m_hWnd,BCM_SETSPLITINFO,0,(LPARAM)&bSplitInfo);
    }

  #endif

  DECLARE_EVENT_TABLE()
};

#endif // _XBUTTON_H_
