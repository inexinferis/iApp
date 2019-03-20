#ifndef _XDATETIMEPICK_H_
#define _XDATETIMEPICK_H_

#include "xwnd.h"

#define XDEFDATETIMEPICKSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XDateTimePick:public XWnd{
  DECLARE_DYNAMIC(XDateTimePick)
  public:
    XDateTimePick(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XDateTimePick(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFDATETIMEPICKSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(DATETIMEPICK_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFDATETIMEPICKSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,DATETIMEPICK_CLASS,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    DWORD GetRange(LPSYSTEMTIME pMinTime,LPSYSTEMTIME pMaxTime)const{
      XASSERT(::IsWindow(m_hWnd))
      SYSTEMTIME sysTimes[2];
      memset(sysTimes,0,sizeof(sysTimes));
      DWORD dwResult = (DWORD)::SendMessage(m_hWnd,DTM_GETRANGE,0,(LPARAM)sysTimes);
      if(pMinTime&&dwResult&GDTR_MIN)
        memcpy(pMinTime,&sysTimes[0],sizeof(SYSTEMTIME));
      if(pMaxTime&&dwResult&GDTR_MAX)
        memcpy(pMaxTime,&sysTimes[1],sizeof(SYSTEMTIME));
      return dwResult;
    }

    BOOL SetRange(LPSYSTEMTIME pMinTime,LPSYSTEMTIME pMaxTime){
      XASSERT(::IsWindow(m_hWnd))
      SYSTEMTIME sysTimes[2];WPARAM wFlags=0;
      if(pMinTime){
        memcpy(&sysTimes[0],pMinTime,sizeof(SYSTEMTIME));
        wFlags|=GDTR_MIN;
      }
      if(pMaxTime){
        memcpy(&sysTimes[1],pMaxTime,sizeof(SYSTEMTIME));
        wFlags|=GDTR_MAX;
      }
      return (BOOL)::SendMessage(m_hWnd,DTM_SETRANGE,wFlags,(LPARAM)sysTimes);
    }

    HFONT GetMonthCalFont()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HFONT)::SendMessage(m_hWnd,DTM_GETMCFONT,0,0);
    }

    HWND GetMonthCalCtrl()const{
      XASSERT(::IsWindow(m_hWnd))
      return (HWND)::SendMessage(m_hWnd,DTM_GETMONTHCAL,0,0);
    }

    VOID SetMonthCalFont(HFONT hFont,BOOL bRedraw=TRUE){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,DTM_SETMCFONT,(WPARAM)hFont,MAKELONG(bRedraw,0));
    }

    COLORREF SetMonthCalColor(INT iColor,COLORREF ref){
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,DTM_SETMCCOLOR,iColor,(LPARAM)ref);
    }

    BOOL SetTime(LPSYSTEMTIME pTimeNew=NULL){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,DTM_SETSYSTEMTIME,(WPARAM)(pTimeNew==NULL)?GDT_NONE:GDT_VALID,(LPARAM)pTimeNew);
    }

    DWORD GetTime(LPSYSTEMTIME pTimeDest)const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,DTM_GETSYSTEMTIME,0,(LPARAM) pTimeDest);
    }

    COLORREF GetMonthCalColor(INT iColor)const{
      XASSERT(::IsWindow(m_hWnd))
      return (COLORREF)::SendMessage(m_hWnd,DTM_GETMCCOLOR,iColor,0);
    }

    BOOL SetFormat(LPCTSTR pstrFormat){
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,DTM_SETFORMAT,0,(LPARAM)pstrFormat);
    }

  #if _WIN32_WINNT >= 0x0600

    DWORD SetMonthCalStyle(DWORD dwStyle){
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,DTM_SETMCSTYLE,0,(LPARAM)dwStyle);
    }

    DWORD GetMonthCalStyle()const{
      XASSERT(::IsWindow(m_hWnd))
      return (DWORD)::SendMessage(m_hWnd,DTM_GETMCSTYLE,0,0);
    }

    BOOL GetDateTimePickerInfo(LPDATETIMEPICKERINFO pDateTimePickerInfo)const{
      XASSERT(::IsWindow(m_hWnd)&&pDateTimePickerInfo!=NULL)
      return (BOOL)::SendMessage(m_hWnd,DTM_GETDATETIMEPICKERINFO,0,(LPARAM)pDateTimePickerInfo);
    }

    BOOL GetIdealSize(LPSIZE pSize)const{
      XASSERT(::IsWindow(m_hWnd))
      return (BOOL)::SendMessage(m_hWnd,DTM_GETIDEALSIZE,0,(LPARAM)pSize);
    }

    VOID CloseMonthCal(){
      XASSERT(::IsWindow(m_hWnd))
      ::SendMessage(m_hWnd,DTM_CLOSEMONTHCAL,0,0);
    }

  #endif

  DECLARE_EVENT_TABLE()
};

#endif // _XDATETIMEPICK_H_
