#ifndef _XSKINNEDWND_H_
#define _XSKINNEDWND_H_

#include "xwnd.h"
#include "xgdi.h"
#include "xskinnedmenu.h"

#define XFONT_SIZE    13
#define XFONT_BIGSIZE 15
#define XFONT_FACE    _T("TAHOMA")
#define XBORDER_SIZE  4
#define XBUTTON_SIZE  20
#define XCAPTION_SIZE XBUTTON_SIZE
#define XTITLEMAX     256
#define XMENUSTRLEN   512

#define XCTRL_COLOR_DISABLED_FIRST  0x909090
#define XCTRL_COLOR_DISABLED_MIDDLE 0x606060
#define XCTRL_COLOR_DISABLED_SHADOW 0x303030

#define XCTRL_COLOR_NORMAL_FIRST  0xB0B0B0
#define XCTRL_COLOR_NORMAL_MIDDLE 0x808080
#define XCTRL_COLOR_NORMAL_SHADOW 0x505050

#define XCTRL_COLOR_SELECTED_FIRST  0xFFFFFF
#define XCTRL_COLOR_SELECTED_MIDDLE 0xB0B0B0
#define XCTRL_COLOR_SELECTED_SHADOW 0x808080

#define XCOLOR_DARKBACKGROUND      0x202020
#define XCOLOR_BACKGROUND          0x303030
#define XCOLOR_BACKGROUND2         0x272727
#define XCOLOR_BACKGROUND_BORDER   0x000000
#define XCOLOR_BACKGROUND_ACTIVE   0x505050
#define XCOLOR_BACKGROUND_INACTIVE 0x404040
#define XCOLOR_NORMAL              0x707070
#define XCOLOR_HOVER               0xA0A0A0
#define XCOLOR_SELECTED            0xF0F0F0

#define XCOLOR_TEXT_ACTIVE         0xD0D0D0
#define XCOLOR_TEXT_INACTIVE       0x909090

class XSkinnedWnd{
  protected:
    XSkinnedWnd(XWnd *Wnd);
  public:
    ~XSkinnedWnd();

    VOID SkinSetText(LPSTR swText);
    VOID SkinSetIcon(BOOL bBig,HICON hIcon);
    BOOL SkinPrint(HDC hDC,DWORD dwFlag);

    VOID SkinNCCreate(LPCREATESTRUCT cs);
    VOID SkinNCPaint(HRGN hRgn);
    BOOL SkinNCActivate(BOOL bActivate,HRGN hRgn);
    INT  SkinNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams);
    INT  SkinNCHitTest(POINTS pt);
    BOOL SkinNCLButtonDown(INT iHittest,POINTS pt);
    BOOL SkinNCLButtonUp(INT iHittest,POINTS pt);
    BOOL SkinNCLButtonDbClk(INT iHittest,POINTS pt);
    BOOL SkinNCRButtonDown(INT iHittest,POINTS pt);
    VOID SkinNCMouseMove(INT iHittest,POINTS pt);
    VOID SkinNCDestroy();

    VOID SkinInitMenuPopup(HMENU hMenu,DWORD info);

    VOID SkinPreTranslateMessage(LPMSG lpMsg);
    VOID SkinPreCreateWindow(LPCREATESTRUCT wcs);
    VOID SkinPreRegisterClass(LPWNDCLASSEX wcx);

  private:

    RECT GetButtonRect(RECT &wRect);
    VOID DrawFrame(XWND &pWnd,HRGN hRgn,BOOL bActive=TRUE);
    VOID DrawCaptionBar(XWND &pwnd,HRGN hRgn,BOOL bActive=TRUE,BOOL bUpdateCtrls=FALSE,BOOL bAccel=FALSE);
    INT  HitTest(XWND &pWnd,POINT &pt);

    //VOID DrawWindowFrame(XWND &pWnd,HRGN hRgn,BOOL bActive=TRUE);
    BOOL DrawMenuBar(XWND &pWnd,XDC &WDC,RECT &tr,BOOL bActive=TRUE,BOOL bAccel=FALSE);
    VOID DrawTitle(XWND &pWnd,XDC &WDC,RECT &tr,BOOL bActive=TRUE,BOOL bAccel=FALSE);
    VOID DrawButton(XDC &WDC,RECT &wr,UINT index,BOOL bEnabled=TRUE);
    VOID DrawResizeTriangle(XDC &WDC,RECT &wr);

    VOID DisableThemes();

    WNDPROC m_MenuProc;
    static HHOOK m_hCbtFilter;
    static XSkinnedWnd *m_pWndInit;
    static LRESULT CALLBACK NewMenuHook(INT code,WPARAM wParam,LPARAM lParam);
    static LRESULT CALLBACK NewMenuProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

  protected:

    BOOL m_sysmenu,m_closeable,m_maxable,m_minable,m_sizable,m_hasicon,m_active,m_hasmenu;
    RECT m_rIcon,m_rClose,m_rMax,m_rMin,m_rHelp,m_rMenu;//(?)
    UINT m_moveHitTest;HFONT m_hFont,m_hMenuFont;
    XWnd *wthis; XSkinnedMenu xSysMenu;
    HMENU m_hMenu;

    XBrush m_backBrush,m_borderBrush;
};

#endif //_XSKINNEDWND_H_
