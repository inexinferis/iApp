#ifndef _XSTATICLINK_H_
#define _XSTATICLINK_H_

#include "xstatic.h"
#include "xgdi.h"

class XStaticLink:public XStatic{
  DECLARE_DYNAMIC(XStaticLink)
  public:
    XStaticLink(HWND hwnd=NULL,COLORREF ColorUnvisited=RGB(0,0,255),COLORREF ColorVisited=RGB(128,0,128));
    virtual ~XStaticLink();

    LRESULT OnNCHitTest(POINTS pt);
    HBRUSH OnCtlColor(HDC hDC,UINT nCtlColor);
    BOOL OnSetCursor(HWND hWnd,DWORD flags);
    VOID OnFocusChange(HWND hWnd);
    VOID OnLButtonUp(UINT nFlags,POINTS pt);
    UINT OnGetDlgCode(DWORD dwVKey,LPMSG pMsg);
    VOID OnChar(UINT nChar,UINT kFlags);

  private:

    HFONT m_hFont;COLORREF m_Color;HCURSOR g_hCursorLink;
    COLORREF g_ColorUnvisited,g_ColorVisited;

    DECLARE_EVENT_TABLE()
};

#endif // _XSTATICLINK_H_
