#ifndef _XRICHEDIT_H_
#define _XRICHEDIT_H_

#include "xwnd.h"
#include <richedit.h>
#include <richole.h>

#define XDEFRICHEDITSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

#define RICHEDITDLLNAME _T("RICHED20.DLL")

class XRichEdit:public XWnd{
  DECLARE_DYNAMIC(XRichEdit)
  public:
    XRichEdit(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XRichEdit(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFRICHEDITSTYLE,LPVOID lpParam=NULL){
      if(!InitRichEdit())
        return NULL;
      return XWnd::Create(RICHEDIT_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFRICHEDITSTYLE,LPVOID lpParam=NULL){
      if(!InitRichEdit())
        return NULL;
      return XWnd::CreateEx(dwStyleEx,RICHEDIT_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    BOOL CanUndo()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_CANUNDO,0,0);
    }

    BOOL CanRedo()const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_CANREDO,0,0);
    }

    UNDONAMEID GetUndoName()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UNDONAMEID)::SendMessage(m_hWnd,EM_GETUNDONAME,0,0);
    }

    UNDONAMEID GetRedoName()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UNDONAMEID)::SendMessage(m_hWnd,EM_GETREDONAME,0,0);
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

    BOOL SetTextMode(UINT fMode){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETTEXTMODE,(WPARAM)fMode,0);
    }

    UINT GetTextMode()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,EM_GETTEXTMODE,0,0);
    }

    VOID GetRect(LPRECT lpRect)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_GETRECT,0,(LPARAM)lpRect);
    }

    POINTL PosFromChar(UINT nChar)const{
      XASSERT(::IsWindow(m_hWnd))
      POINTL pt;
      ::SendMessage(m_hWnd,EM_POSFROMCHAR,(WPARAM)&pt,nChar);
      return pt;
    }

    UINT GetOptions()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,EM_GETOPTIONS,0,0);
    }

    VOID SetOptions(WORD wOp,DWORD dwFlags){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETOPTIONS,(WPARAM)wOp,(LPARAM)dwFlags);
    }

    BOOL SetAutoURLDetect(BOOL bEnable=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_AUTOURLDETECT,(WPARAM)bEnable,0);
    }

    VOID EmptyUndoBuffer(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_EMPTYUNDOBUFFER,0,0);
    }

    UINT SetUndoLimit(UINT nLimit){
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,EM_SETUNDOLIMIT,(WPARAM)nLimit,0);
    }

    VOID ReplaceSel(LPCTSTR lpszNewText,BOOL bCanUndo=FALSE){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_REPLACESEL,(WPARAM)bCanUndo,(LPARAM)lpszNewText);
    }

    VOID SetRect(LPCRECT lpRect){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SETRECT,0,(LPARAM)lpRect);
    }

    VOID StopGroupTyping(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_STOPGROUPTYPING,0,0);
    }

    BOOL Redo(){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_REDO,0,0);
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

    BOOL DisplayBand(LPRECT pDisplayRect){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_DISPLAYBAND,0,(LPARAM)pDisplayRect);
    }

    VOID GetSel(CHARRANGE &cr)const{
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_EXGETSEL,0,(LPARAM)&cr);
    }

    VOID GetSel(LONG &nStartChar,LONG &nEndChar)const{
      XASSERT(::IsWindow(m_hWnd))
      CHARRANGE cr;
      ::SendMessage(m_hWnd,EM_EXGETSEL,0,(LPARAM)&cr);
      nStartChar=cr.cpMin;
      nEndChar=cr.cpMax;
    }

    BOOL GetPunctuation(UINT fType,PUNCTUATION* lpPunc)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_GETPUNCTUATION,(WPARAM)fType,(LPARAM)lpPunc);
    }

    BOOL SetPunctuation(UINT fType,PUNCTUATION* lpPunc){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETPUNCTUATION,(WPARAM)fType,(LPARAM)lpPunc);
    }

    VOID LimitText(LONG nChars){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_EXLIMITTEXT,0,nChars);
    }

    LONG LineFromChar(LONG nIndex)const{
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_EXLINEFROMCHAR,0,nIndex);
    }

    INT CharFromPos(POINTL pt)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_CHARFROMPOS,0,(LPARAM)&pt);
    }

    VOID SetSel(CHARRANGE &cr){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_EXSETSEL,0,(LPARAM)&cr);
    }

    VOID SetSel(LONG nStartChar,LONG nEndChar){
      XASSERT(::IsWindow(m_hWnd))
      CHARRANGE cr;
      cr.cpMin=nStartChar;
      cr.cpMax=nEndChar;
      ::SendMessage(m_hWnd,EM_EXSETSEL,0,(LPARAM)&cr);
    }

    DWORD FindWordBreak(UINT nCode,DWORD nStart)const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,EM_FINDWORDBREAK,(WPARAM)nCode,(LPARAM)nStart);
    }

    LONG FindText(DWORD dwFlags,FINDTEXTEX* pFindText)const{
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_FINDTEXTEX,dwFlags,(LPARAM)pFindText);
    }

    LONG FormatRange(FORMATRANGE* pfr,BOOL bDisplay=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_FORMATRANGE,(WPARAM)bDisplay,(LPARAM)pfr);
    }

    LONG GetEventMask()const{
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_GETEVENTMASK,0,0);
    }

    UINT GetLimitText()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT)::SendMessage(m_hWnd,EM_GETLIMITTEXT,0,0);
    }

    LONG GetSelText(LPSTR lpBuf)const{
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_GETSELTEXT,0,(LPARAM)lpBuf);
    }

    VOID HideSelection(BOOL bHide,BOOL bPerm){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_HIDESELECTION,bHide,bPerm);
    }

    VOID RequestResize(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_REQUESTRESIZE,0,0);
    }

    WORD GetSelectionType()const{
      XASSERT(::IsWindow(m_hWnd))
      return (WORD)::SendMessage(m_hWnd,EM_SELECTIONTYPE,0,0);
    }

    UINT GetWordWrapMode()const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT) ::SendMessage(m_hWnd,EM_GETWORDWRAPMODE,0,0);
    }

    UINT SetWordWrapMode(UINT uFlags)const{
      XASSERT(::IsWindow(m_hWnd))
      return (UINT) ::SendMessage(m_hWnd,EM_SETWORDWRAPMODE,(WPARAM) uFlags,0);
    }

    COLORREF SetBackgroundColor(BOOL bSysColor,COLORREF cr){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,EM_SETBKGNDCOLOR,bSysColor,cr);
    }

    BOOL SetTextColor(BOOL bSelection,COLORREF cr){
      CHARFORMAT frmt;
      frmt.cbSize=sizeof(frmt);
      frmt.dwMask=CFM_COLOR;
      frmt.crTextColor=cr;
      frmt.dwEffects=0;
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,bSelection?SCF_SELECTION:SCF_ALL,(LPARAM)&frmt);
    }

    DWORD SetEventMask(DWORD dwEventMask){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,EM_SETEVENTMASK,0,dwEventMask);
    }

    IRichEditOle* GetIRichEditOle()const{
      XASSERT(::IsWindow(m_hWnd))
      IRichEditOle *pRichItem=NULL;
      ::SendMessage(m_hWnd,EM_GETOLEINTERFACE,0,(LPARAM)&pRichItem);
      return pRichItem;
    }

    BOOL SetOLECallback(IRichEditOleCallback* pCallback){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETOLECALLBACK,0,(LPARAM)pCallback);
    }

    BOOL SetTargetDevice(HDC hDC,LONG lLineWidth){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_SETTARGETDEVICE,(WPARAM)hDC,lLineWidth);
    }

    LONG StreamIn(INT nFormat,EDITSTREAM &es){
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_STREAMIN,nFormat,(LPARAM)&es);
    }

    LONG StreamOut(INT nFormat,EDITSTREAM &es){
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,EM_STREAMOUT,nFormat,(LPARAM)&es);
    }

    LONG GetTextLength()const{
      XASSERT(::IsWindow(m_hWnd))
      return (LONG)::SendMessage(m_hWnd,WM_GETTEXTLENGTH,0,0);
    }

    LONG GetTextLengthEx(DWORD dwFlags,INT nCodePage=-1)const{
      XASSERT(::IsWindow(m_hWnd))
      GETTEXTLENGTHEX textLenEx;
      textLenEx.flags=dwFlags;
      textLenEx.codepage=nCodePage;
      if(nCodePage==-1){
    #ifdef _UNICODE
        textLenEx.codepage=1200;
    #else
        textLenEx.codepage=CP_ACP;
    #endif
      }
      return (LONG)::SendMessage(m_hWnd,EM_GETTEXTLENGTHEX,(WPARAM)&textLenEx,0);
    }

    BOOL CanPaste(UINT nFormat=0)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,EM_CANPASTE,nFormat,0);
    }

    VOID PasteSpecial(UINT nClipFormat,DWORD dvAspect,HMETAFILE hMF){
      XASSERT(::IsWindow(m_hWnd))
      REPASTESPECIAL reps;
      reps.dwAspect=dvAspect;
      reps.dwParam=(DWORD_PTR)hMF;
      ::SendMessage(m_hWnd,EM_PASTESPECIAL,nClipFormat,(LPARAM)&reps);
    }

    VOID LineScroll(INT nLines,INT nChars=0){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_LINESCROLL,nChars,nLines);
    }

    INT LineLength(INT nLine=-1)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_LINELENGTH,nLine,0);
    }

    INT LineIndex(INT nLine=-1)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_LINEINDEX,nLine,0);
    }

    BOOL SetParaFormat(PARAFORMAT &pf){
      XASSERT(::IsWindow(m_hWnd))
      pf.cbSize=sizeof(PARAFORMAT);
      return (BOOL)::SendMessage(m_hWnd,EM_SETPARAFORMAT,0,(LPARAM)&pf);
    }

    BOOL SetParaFormat(PARAFORMAT2 &pf){
      XASSERT(::IsWindow(m_hWnd))
      pf.cbSize=sizeof(PARAFORMAT2);
      return (BOOL)::SendMessage(m_hWnd,EM_SETPARAFORMAT,0,(LPARAM)&pf);
    }

    DWORD GetParaFormat(PARAFORMAT &pf)const{
      XASSERT(::IsWindow(m_hWnd))
      pf.cbSize=sizeof(PARAFORMAT);
      return (DWORD)::SendMessage(m_hWnd,EM_GETPARAFORMAT,0,(LPARAM)&pf);
    }

    DWORD GetParaFormat(PARAFORMAT2 &pf)const{
      XASSERT(::IsWindow(m_hWnd))
      pf.cbSize=sizeof(PARAFORMAT2);
      return (DWORD)::SendMessage(m_hWnd,EM_GETPARAFORMAT,0,(LPARAM)&pf);
    }

    INT GetTextRange(INT nFirst,INT nLast,LPTSTR refString)const{
      XASSERT(::IsWindow(m_hWnd))
      INT nLength=INT(nLast-nFirst+1);
      XASSERT(nLength>0)
      XUNUSED(nLength)
      TEXTRANGE textRange;
      textRange.chrg.cpMin=nFirst;
      textRange.chrg.cpMax=nLast;
      textRange.lpstrText=refString;
      return (INT)::SendMessage(m_hWnd,EM_GETTEXTRANGE,0,(LPARAM)&textRange);
    }

    BOOL SetDefaultCharFormat(CHARFORMAT &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,0,(LPARAM)&cf);
    }

    BOOL SetDefaultCharFormat(CHARFORMAT2 &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,0,(LPARAM)&cf);
    }

    BOOL SetSelectionCharFormat(CHARFORMAT &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);
    }

    BOOL SetSelectionCharFormat(CHARFORMAT2 &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);
    }

    BOOL SetWordCharFormat(CHARFORMAT &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_SELECTION|SCF_WORD,(LPARAM)&cf);
    }

    BOOL SetWordCharFormat(CHARFORMAT2 &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_SELECTION|SCF_WORD,(LPARAM)&cf);
    }

    BOOL SetAllCharFormat(CHARFORMAT &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_ALL,(LPARAM)&cf);
    }

    BOOL SetAllCharFormat(CHARFORMAT2 &cf){
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (BOOL)::SendMessage(m_hWnd,EM_SETCHARFORMAT,SCF_ALL,(LPARAM)&cf);
    }

    DWORD GetDefaultCharFormat(CHARFORMAT &cf)const{
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (DWORD)::SendMessage(m_hWnd,EM_GETCHARFORMAT,0,(LPARAM)&cf);
    }

    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf)const{
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (DWORD)::SendMessage(m_hWnd,EM_GETCHARFORMAT,0,(LPARAM)&cf);
    }

    DWORD GetSelectionCharFormat(CHARFORMAT &cf)const{
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (DWORD)::SendMessage(m_hWnd,EM_GETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);
    }

    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf)const{
      XASSERT(::IsWindow(m_hWnd))
      cf.cbSize=sizeof(cf);
      return (DWORD)::SendMessage(m_hWnd,EM_GETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);
    }

    INT GetLine(INT nIndex,LPTSTR lpszBuffer)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,EM_GETLINE,nIndex,(LPARAM)lpszBuffer);
    }

    INT GetLine(INT nIndex,LPTSTR lpszBuffer,INT nMaxLength)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(nMaxLength>0)
      *(LPINT)lpszBuffer=nMaxLength;
      return (INT)::SendMessage(m_hWnd,EM_GETLINE,nIndex,(LPARAM)lpszBuffer);
    }

    VOID ScrollCaret(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,EM_SCROLLCARET,0,0);
    }

  private:

    BOOL InitRichEdit(){
      HINSTANCE hInst=::GetModuleHandle(RICHEDITDLLNAME);
      if(!hInst)hInst=::LoadLibrary(RICHEDITDLLNAME);
      return (hInst!=NULL);
    }

  DECLARE_EVENT_TABLE()
};

#endif // _XRICHEDIT_H_
