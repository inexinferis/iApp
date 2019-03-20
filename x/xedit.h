#ifndef _XEDIT_H_
#define _XEDIT_H_

#include "xwnd.h"

#define XDEFEDITSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE

class XEdit:public XWnd{
  DECLARE_DYNAMIC(XEdit)
  public:
    XEdit(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XEdit(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFEDITSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_EDIT,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFEDITSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_EDIT,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    BOOL CanUndo()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_CANUNDO,0,0);
    }

    INT GetLineCount()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_GETLINECOUNT,0,0);
    }

    BOOL GetModify()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_GETMODIFY,0,0);
    }

    VOID SetModify(BOOL bModified=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETMODIFY,bModified,0);
    }

    VOID GetRect(LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_GETRECT,0,(LPARAM)lpRect);
    }

    VOID GetSel(PDWORD nStartChar=NULL,PDWORD nEndChar=NULL)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_GETSEL,(WPARAM)nStartChar,(LPARAM)nEndChar);
    }

    HLOCAL GetHandle(){
      XASSERT(::IsWindow(m_hWnd))
      return (HLOCAL)::SendMessage(m_hWnd,EM_GETHANDLE,0,0);
    }

    VOID SetHandle(HLOCAL hBuffer){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETHANDLE,(WPARAM)hBuffer,0);
    }

    INT GetLine(INT nIndex,LPTSTR lpszBuffer)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_GETLINE,nIndex,(LPARAM)lpszBuffer);
    }

    VOID EmptyUndoBuffer(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_EMPTYUNDOBUFFER,0,0);
    }

    BOOL FmtLines(BOOL bAddEOL){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_FMTLINES,bAddEOL,0);
    }

    VOID LimitText(INT nChars){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_LIMITTEXT,nChars,0);
    }

    INT LineFromChar(INT nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_LINEFROMCHAR,nIndex,0);
    }

    INT LineIndex(INT nLine=-1)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_LINEINDEX,nLine,0);
    }

    INT LineLength(INT nLine=-1)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_LINELENGTH,nLine,0);
    }

    VOID LineScroll(INT nLines,INT nChars=0){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_LINESCROLL,nChars,nLines);
    }

    VOID ReplaceSel(LPCTSTR lpszNewText,BOOL bCanUndo){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_REPLACESEL,(WPARAM) bCanUndo,(LPARAM)lpszNewText);
    }

    VOID SetPasswordChar(TCHAR ch){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETPASSWORDCHAR,ch,0);
    }

    VOID SetRect(LPCRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETRECT,0,(LPARAM)lpRect);
    }

    VOID SetRectNP(LPCRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETRECTNP,0,(LPARAM)lpRect);
    }

    VOID SetSel(INT nStartChar,INT nEndChar){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETSEL,nStartChar,nEndChar);
    }

    VOID ScrollCaret(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SCROLLCARET,0,0);
    }

    BOOL SetTabStops(INT nTabStops=0,LPINT rgTabStops=NULL){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETTABSTOPS,nTabStops,(LPARAM)rgTabStops);
    }

    BOOL Undo(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_UNDO,0,0);
    }

    VOID Clear(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_CLEAR,0,0);
    }

    VOID Copy(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_COPY,0,0);
    }

    VOID Cut(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_CUT,0,0);
    }

    VOID Paste(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,WM_PASTE,0,0);
    }

    BOOL SetReadOnly(BOOL bReadOnly=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETREADONLY,bReadOnly,0);
    }

    INT GetFirstVisibleLine()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_GETFIRSTVISIBLELINE,0,0);
    }

    TCHAR GetPasswordChar()const{
      XASSERT(::IsWindow(m_hWnd))
      return (TCHAR)::SendMessage(m_hWnd,EM_GETPASSWORDCHAR,0,0);
    }

    VOID SetMargins(UINT nLeft,UINT nRight){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETMARGINS,EC_LEFTMARGIN|EC_RIGHTMARGIN,MAKELONG(nLeft,nRight));
    }

    DWORD GetMargins()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,EM_GETMARGINS,0,0);
    }

    VOID SetLimitText(UINT nMax){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETLIMITTEXT,nMax,0);
    }

    UINT GetLimitText()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,EM_GETLIMITTEXT,0,0);
    }

    DWORD PosFromChar(UINT nChar) const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,EM_POSFROMCHAR,nChar,0);
    }

    INT CharFromPos(POINT pt)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_CHARFROMPOS,0,MAKELPARAM(pt.x,pt.y));
    }

  #if _WIN32_WINNT >= 0x501
    BOOL SetCueBanner(LPCWSTR lpszText,BOOL fDrawWhenFocused=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETCUEBANNER,fDrawWhenFocused,(LPARAM)lpszText);
    }

    BOOL GetCueBanner(LPWSTR lpszText,INT cchText)const{
      XASSERT(::IsWindow(m_hWnd)&&lpszText!=NULL)
      if(cchText==0||lpszText==NULL)return FALSE;lpszText[0]=0;
      return (BOOL)::SendMessage(m_hWnd,EM_GETCUEBANNER,(WPARAM)lpszText,(LPARAM)cchText);
    }

    BOOL ShowBalloonTip(LPCWSTR lpszTitle,LPCWSTR lpszText,INT ttiIcon){
      XASSERT(::IsWindow(m_hWnd))
      EDITBALLOONTIP bt;::ZeroMemory(&bt,sizeof(EDITBALLOONTIP));
      bt.cbStruct=sizeof(EDITBALLOONTIP);bt.pszTitle=lpszTitle;
      bt.pszText=lpszText;bt.ttiIcon=ttiIcon;
      return (BOOL)::SendMessage(m_hWnd,EM_SHOWBALLOONTIP,0,(LPARAM)&bt);
    }

    BOOL ShowBalloonTip(PEDITBALLOONTIP pEditBalloonTip){
      XASSERT(::IsWindow(m_hWnd)&&pEditBalloonTip!=NULL)
      if(pEditBalloonTip==NULL)return FALSE;
      return (BOOL)::SendMessage(m_hWnd,EM_SHOWBALLOONTIP,0,(LPARAM)pEditBalloonTip);
    }

    BOOL HideBalloonTip(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_HIDEBALLOONTIP,0,0);
    }
  #endif

  #if _WIN32_WINNT >= 0x0600
    VOID SetHighlight(INT ichStart,INT ichEnd){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETHILITE,ichStart,(LPARAM)ichEnd);
    }

    BOOL GetHighlight(PINT pichStart,PINT pichEnd)const{
      XASSERT(::IsWindow(m_hWnd)&&pichStart!=NULL&&pichEnd!=NULL)
      if(pichStart==NULL||pichEnd==NULL)return FALSE;
      DWORD dwHighlight=(DWORD)::SendMessage(m_hWnd,EM_GETHILITE,0,0);
      *pichStart=(INT)LOWORD(dwHighlight);
      *pichEnd=(INT)HIWORD(dwHighlight);
      return TRUE;
    }
  #endif

  DECLARE_EVENT_TABLE()
};

#endif // _XEDIT_H_
