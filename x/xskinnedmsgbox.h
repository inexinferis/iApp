#ifndef _XSKINNEDMSGBOX_H_
#define _XSKINNEDMSGBOX_H_

#include "xmsgbox.h"
#include "xskinnedwnd.h"
#include "xskinnedstatic.h"
#include "xskinnedbutton.h"

class XSkinnedMsgBox:public XMsgBox,XSkinnedWnd{
  public:
    XSkinnedMsgBox():XSkinnedWnd(this){}
    virtual ~XSkinnedMsgBox(){}

    BOOL OnNCCreate(LPCREATESTRUCT cs);
    VOID OnNCPaint(HRGN hRgn);
    BOOL OnNCActivate(BOOL bActivate,HRGN hRgn);
    INT  OnNCCalcSize(BOOL bValidCA,NCCALCSIZE_PARAMS *pParams);
    LRESULT  OnNCHitTest(POINTS pt);
    VOID OnNCLButtonDown(UINT iHittest,POINTS pt);
    VOID OnNCLButtonUp(UINT iHittest,POINTS pt);
    VOID OnNCLButtonDbClk(UINT iHittest,POINTS pt);
    VOID OnNCRButtonDown(UINT iHittest,POINTS pt);
    VOID OnNCMouseMove(UINT iHittest,POINTS pt);
    VOID OnNCDestroy();
    VOID OnInitMenuPopup(HMENU hMenu,DWORD info);

    virtual BOOL PreCreateWindow(LPCREATESTRUCT wcs);
    virtual BOOL PreRegisterClass(LPWNDCLASSEX wcx);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

  protected:
    // Overloaded for Skinning...
    virtual XStatic *GetStaticIcon(){
      return &m_SkinnedStcIcon;
    }

    virtual XStatic *GetStaticText(){
      return &m_SkinnedStcText;
    }

    virtual XButton *GetButton(UINT i){
      if(i<XMSGBOX_MAXBTNS)
        return &m_SkinnedButtons[i];
      return &m_SkinnedButtons[0];
    }

    XSkinnedStatic m_SkinnedStcIcon,m_SkinnedStcText;
    XSkinnedButton m_SkinnedButtons[XMSGBOX_MAXBTNS];

  DECLARE_EVENT_TABLE()
};

#endif //_XSKINNEDMSGBOX_H_
