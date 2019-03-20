#ifndef _XSKINNEDMENU_H_
#define _XSKINNEDMENU_H_

#include "xmenuex.h"
#include "xgdi.h"
#include "xcoords.h"

#define XFONT_SIZE    13
#define XFONT_FACE    _T("TAHOMA")
#define XBORDER_SIZE  4
#define XBUTTON_SIZE  20
//#define XCAPTION_SIZE XBUTTON_SIZE
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

#define XCOLOR_BACKGROUND          0x303030
#define XCOLOR_BACKGROUND_BORDER   0x000000
#define XCOLOR_BACKGROUND_ACTIVE   0x505050
#define XCOLOR_BACKGROUND_INACTIVE 0x404040
#define XCOLOR_NORMAL              0x707070
#define XCOLOR_HOVER               0xA0A0A0
#define XCOLOR_SELECTED            0xF0F0F0

#define XCOLOR_TEXT_ACTIVE         0xD0D0D0
#define XCOLOR_TEXT_INACTIVE       0x909090
#define XCOLOR_TEXT_DISABLED       0x606060

#ifndef MN_SIZEWINDOW
  #define MN_SIZEWINDOW 0x01E2
#endif

#ifndef ODS_HOTLIGHT
  #define ODS_HOTLIGHT 0x0040
#endif

#ifndef ODS_NOACCEL
  #define ODS_NOACCEL 0x0100
#endif

#ifndef OBM_CLOSE
  #define OBM_CLOSE 0x7FF2
#endif

#define AnsiUpperChar(c) ((TCHAR)LOWORD((DWORD_PTR)CharUpper((LPTSTR)(DWORD)MAKELONG((DWORD)c,0))))

class XSkinnedMenu:public XMenuEx{
  DECLARE_DYNCREATE(XSkinnedMenu)
  public:
    XSkinnedMenu();
    virtual ~XSkinnedMenu();

    //events
    virtual BOOL OnDrawItem(HWND hWnd,LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual BOOL OnMeasureItem(HWND hWnd,LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual LRESULT OnMenuChar(HWND hWnd,UINT nChar,UINT nFlags);

  protected:
    //for Subclassing!
    virtual XMenuEx* DoCreate(XMenuEx* parent);
    virtual XMenuEx* DoInsert(UINT uPos,UINT wID,LPCTSTR mtitle,UINT fState=MFS_ENABLED,UINT fType=MFT_STRING,
      HBITMAP hbmpItem=NULL,HBITMAP hbmpChecked=NULL,HBITMAP hbmpUnchecked=NULL);
    virtual BOOL DoConvert();
    virtual BOOL DoRevert();

    XBrush m_backBrush,m_borderBrush;
    HBITMAP m_hbmpItem,m_hbmpChecked,m_hbmpUnchecked;

  protected://internal initialization
    XSkinnedMenu(XMenuEx *mParent):XMenuEx(mParent){}
};

#endif //_XSKINNEDMENU_H_
