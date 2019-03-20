#ifndef _XGDI_H_
#define _XGDI_H_

#include "xapp.h" //for map...

#define HIMETRIC_INCH   2540    // HIMETRIC units per inch

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#endif

/**************************************************************/
// XCOLOR REFERENCE...
/**************************************************************/

typedef struct _RGB_{
  BYTE R,G,B;
}RGB,*LPRGB;

typedef struct _HSL_{
  double H,S,L;
}HSL,*LPHSL;

class XColorRef:public XObject{
	DECLARE_DYNAMIC(XColorRef)
  public:
    XColorRef():g_color(0){};
    XColorRef(COLORREF color):g_color(color){};
    XColorRef(BYTE byRed,BYTE byGreen,BYTE byBlue){
      g_color=RGB(byRed,byGreen,byBlue);
    }
    virtual ~XColorRef(){}

    XColorRef *FromColor(COLORREF color){
      return new XColorRef(color);
    }

    COLORREF Attach(COLORREF color){
      g_color=color;
      return color;
    }

    COLORREF Detach(){
      COLORREF color=g_color;
      g_color=0;
      return color;
    }

    COLORREF GetColor(){
      return (this == NULL ? 0 : g_color);
    }

    BYTE GetRed(){return GetRValue(g_color);}
    BYTE GetGreen(){return GetGValue(g_color);}
    BYTE GetBlue(){return GetBValue(g_color);}

    COLORREF GetSysColor(INT nIndex){
      return g_color=::GetSysColor(nIndex);
    }

    COLORREF MixColors(COLORREF color);
    COLORREF MidColor(COLORREF color);
    COLORREF LightColor(BYTE lScale);
    COLORREF DarkColor(BYTE lScale);
    COLORREF GrayColor();
    COLORREF GrayScale();
    COLORREF AlphaBlendColor(INT weighting);
    COLORREF BrigthColor(BYTE porc,BOOL Add=TRUE);
    COLORREF BleachColor(BYTE cant,BOOL Add=TRUE);

    BOOL IsLightColor(){
      return (GetRed()+GetGreen()+GetBlue())>(3*128);
    }

    COLORREF DarkenColor(BYTE lScale){
      return g_color=DarkColor(lScale);
    }

    COLORREF LightenColor(BYTE lScale){
      return g_color=LightColor(lScale);
    }

    COLORREF HSLtoRGB(HSL hsl);
    HSL RGBtoHSL();

    COLORREF SetBrightness(double brightness);
    COLORREF ModifyBrightness(double brightness);
    COLORREF SetSaturation(double Saturation);
    COLORREF ModifySaturation(double Saturation);
    COLORREF SetHue(double Hue);
    COLORREF ModifyHue(double Hue);

    XColorRef &operator=(COLORREF color){
      g_color=color;
      return *this;
    }

    operator COLORREF(){return g_color;}

  private:
    INT Round(double val);

  protected:
    COLORREF g_color;
};

/**************************************************************/
// XDC REFERENCE...
/**************************************************************/

class XDC:public XObject{
	DECLARE_DYNAMIC(XDC)
  public:
    XDC(HDC hDC=NULL):m_hDC(hDC),m_bAttached(FALSE){};
    virtual ~XDC(){
      if(m_hDC!=NULL&&m_bAttached)
        Delete();
    }

    HDC GetSafeHandle(){
      return (this==NULL?NULL:m_hDC);
    }

    BOOL IsValid(){
      return (this==NULL?FALSE:m_hDC!=NULL);
    }

    static XDC* FromHandlePermanent(HDC hDC){
      XASSERT(hDC!=NULL)
      XDC* pDC=NULL;
      if(hDC)
        pDC=XWinApp::m_XDCMap.Find(hDC);
      return pDC;
    }

    static XDC* FromHandle(HDC hDC){
      XASSERT(hDC!=NULL)
      XDC* pDC=NULL;
      if(hDC){
        pDC=FromHandlePermanent(hDC);
        if(!pDC){
          pDC=new XDC();
          if(pDC)pDC->Attach(hDC);
        }
      }
      return pDC;
    }

    HDC Attach(HDC hDC){
      XASSERT(m_hDC==NULL)
      if(!hDC)return NULL;
      m_bAttached=XWinApp::m_XDCMap.Add(hDC,this);
      m_hDC=hDC;
      return hDC;
    }

    HDC Detach(){
      HDC hDC=m_hDC;
      if(hDC){
        if(m_bAttached){
          XWinApp::m_XDCMap.Remove(hDC);
          m_bAttached=FALSE;
        }
        m_hDC=NULL;
      }
      return hDC;
    }

    HWND GetWindow()const{
      XASSERT(m_hDC!=NULL)
      return ::WindowFromDC(m_hDC);
    }

    HPEN GetCurrentPen()const{
      XASSERT(m_hDC!=NULL)
      return (HPEN)::GetCurrentObject(m_hDC,OBJ_PEN);
    }

    HBRUSH GetCurrentBrush()const{
      XASSERT(m_hDC!=NULL)
      return (HBRUSH)::GetCurrentObject(m_hDC,OBJ_BRUSH);
    }

    HPALETTE GetCurrentPalette()const{
      XASSERT(m_hDC!=NULL)
      return (HPALETTE)::GetCurrentObject(m_hDC,OBJ_PAL);
    }

    HFONT GetCurrentFont()const{
      XASSERT(m_hDC!=NULL)
      return (HFONT)::GetCurrentObject(m_hDC,OBJ_FONT);
    }

    HBITMAP GetCurrentBitmap()const{
      XASSERT(m_hDC!=NULL)
      return (HBITMAP)::GetCurrentObject(m_hDC,OBJ_BITMAP);
    }

    HGDIOBJ GetCurrentObject(UINT uObjectType){
      XASSERT(m_hDC!=NULL)
      return ::GetCurrentObject(m_hDC,uObjectType);
    }

  #if (_WIN32_WINDOWS >= 0x0600)
    DWORD GetLayout()const{
      XASSERT(m_hDC!=NULL)
      return ::GetLayout(m_hDC);
    }

    DWORD SetLayout(DWORD dwLayout){
      XASSERT(m_hDC!=NULL)
      return ::SetLayout(m_hDC,dwLayout);
    }
  #endif

    HDC Create(LPCTSTR lpszDriver,LPCTSTR lpszDevice=NULL,LPCTSTR lpszOutput=NULL,CONST DEVMODE *lpInitData=NULL){
      return Attach(::CreateDC(lpszDriver,lpszDevice,lpszOutput,lpInitData));
    }

    HDC CreateIC(LPCTSTR lpszDriver,LPCTSTR lpszDevice=NULL,LPCTSTR lpszOutput=NULL,CONST DEVMODE *lpInitData=NULL){
      return Attach(::CreateIC(lpszDriver,lpszDevice,lpszOutput,lpInitData));
    }

    HDC CreateCompatible(HDC hdc){
      return Attach(::CreateCompatibleDC(hdc));
    }

    HBITMAP CreateCompatibleBitmap(UINT x,UINT y){
      XASSERT(m_hDC!=NULL)
      return ::CreateCompatibleBitmap(m_hDC,x,y);
    }

    HBITMAP CreateDIBSection(const BITMAPINFO* pbmi,UINT iUsage,void** ppvBits,HANDLE hSection,DWORD dwOffset){
      XASSERT(m_hDC!=NULL)
      return ::CreateDIBSection(m_hDC,pbmi,iUsage,ppvBits,hSection,dwOffset);
    }

    BOOL Delete(){
      if(m_hDC!=NULL)
        return ::DeleteDC(Detach());
      return FALSE;
    }

    INT Save(){
      XASSERT(m_hDC!=NULL)
      return ::SaveDC(m_hDC);
    }

    BOOL Restore(INT nSavedDC){
      XASSERT(m_hDC!=NULL)
      return ::RestoreDC(m_hDC,nSavedDC);
    }

    INT GetDeviceCaps(INT nIndex)const{
      XASSERT(m_hDC!=NULL)
      return ::GetDeviceCaps(m_hDC,nIndex);
    }

    UINT SetBoundsRect(LPCRECT lpRectBounds,UINT flags){
      XASSERT(m_hDC!=NULL)
      return ::SetBoundsRect(m_hDC,lpRectBounds,flags);
    }

    UINT GetBoundsRect(LPRECT lpRectBounds,UINT flags){
      XASSERT(m_hDC!=NULL)
      return ::GetBoundsRect(m_hDC,lpRectBounds,flags);
    }

    HDC Reset(CONST DEVMODE* lpDevMode){
      XASSERT(m_hDC!=NULL)
      return ::ResetDC(m_hDC,lpDevMode);
    }

    POINT GetBrushOrg()const{
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::GetBrushOrgEx(m_hDC,&point);
      return point;
    }

    POINT SetBrushOrg(INT x,INT y){
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::SetBrushOrgEx(m_hDC,x,y,&point);
      return point;
    }

    POINT SetBrushOrg(POINT point){
      XASSERT(m_hDC!=NULL)
      ::SetBrushOrgEx(m_hDC,point.x,point.y,&point);
      return point;
    }

    INT EnumObjects(INT nObjectType,GOBJENUMPROC lpfn,LPARAM lpData){
      XASSERT(m_hDC!=NULL)
      return ::EnumObjects(m_hDC,nObjectType,lpfn,lpData);
    }

    HGDIOBJ SelectStockObject(INT nIndex){
      XASSERT(m_hDC!=NULL)
      return ::SelectObject(m_hDC,::GetStockObject(nIndex));
    }

    HGDIOBJ SelectObject(HGDIOBJ hgdiobj){
      XASSERT(m_hDC!=NULL)
      return ::SelectObject(m_hDC,hgdiobj);
    }

    COLORREF GetNearestColor(COLORREF crColor)const{
      XASSERT(m_hDC!=NULL)
      return ::GetNearestColor(m_hDC,crColor);
    }

    HPALETTE SelectPalette(HPALETTE hpal,BOOL bForceBackground){
      XASSERT(m_hDC!=NULL)
      return ::SelectPalette(m_hDC,hpal,bForceBackground);
    }

    UINT RealizePalette(){
      XASSERT(m_hDC!=NULL)
      return ::RealizePalette(m_hDC);
    }

    BOOL UpdateColors(){
      XASSERT(m_hDC!=NULL)
      return ::UpdateColors(m_hDC);
    }

    COLORREF GetBkColor()const{
      XASSERT(m_hDC!=NULL)
      return ::GetBkColor(m_hDC);
    }

    INT GetBkMode()const{
      XASSERT(m_hDC!=NULL)
      return ::GetBkMode(m_hDC);
    }

    INT GetPolyFillMode()const{
      XASSERT(m_hDC!=NULL)
      return ::GetPolyFillMode(m_hDC);
    }

    INT GetROP2()const{
      XASSERT(m_hDC!=NULL)
      return ::GetROP2(m_hDC);
    }

    INT GetStretchBltMode()const{
      XASSERT(m_hDC!=NULL)
      return ::GetStretchBltMode(m_hDC);
    }

    COLORREF GetTextColor()const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextColor(m_hDC);
    }

    COLORREF SetBkColor(COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetBkColor(m_hDC,crColor);
    }

    INT SetBkMode(INT iBkMode){
      XASSERT(m_hDC!=NULL)
      return ::SetBkMode(m_hDC,iBkMode);
    }

    INT SetPolyFillMode(INT nPolyFillMode){
      XASSERT(m_hDC!=NULL)
      return ::SetPolyFillMode(m_hDC,nPolyFillMode);
    }

    INT SetROP2(INT fnDrawMode){
      XASSERT(m_hDC!=NULL)
      return ::SetROP2(m_hDC,fnDrawMode);
    }

    INT SetStretchBltMode(INT iStretchMode){
      XASSERT(m_hDC!=NULL)
      return ::SetStretchBltMode(m_hDC,iStretchMode);
    }

    COLORREF SetTextColor(COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetTextColor(m_hDC,crColor);
    }

    BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const{
      XASSERT(m_hDC!=NULL)
      return ::GetColorAdjustment(m_hDC,lpColorAdjust);
    }

    BOOL SetColorAdjustment(CONST LPCOLORADJUSTMENT lpColorAdjust){
      XASSERT(m_hDC!=NULL)
      return ::SetColorAdjustment(m_hDC,lpColorAdjust);
    }

  #if (_WIN32_WINDOWS >= 0x0500)
    COLORREF GetDCBrushColor()const{
      XASSERT(m_hDC!=NULL)
      return ::GetDCBrushColor(m_hDC);
    }

    COLORREF GetDCPenColor()const{
      XASSERT(m_hDC!=NULL)
      return ::GetDCPenColor(m_hDC);
    }

    COLORREF SetDCBrushColor(COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetDCBrushColor(m_hDC,crColor);
    }

    COLORREF SetDCPenColor(COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetDCPenColor(m_hDC,crColor);
    }
  #endif

    INT SetGraphicsMode(INT iMode){
      XASSERT(m_hDC!=NULL)
      return ::SetGraphicsMode(m_hDC,iMode);
    }

    INT GetGraphicsMode()const{
      XASSERT(m_hDC!=NULL)
      return ::GetGraphicsMode(m_hDC);
    }

    BOOL SetWorldTransform(CONST XFORM* pXform){
      XASSERT(m_hDC!=NULL)
      return ::SetWorldTransform(m_hDC,pXform);
    }

    BOOL ModifyWorldTransform(CONST XFORM* pXform,DWORD iMode){
      XASSERT(m_hDC!=NULL)
      return ::ModifyWorldTransform(m_hDC,pXform,iMode);
    }

    BOOL GetWorldTransform(XFORM* pXform)const{
      XASSERT(m_hDC!=NULL)
      return ::GetWorldTransform(m_hDC,pXform);
    }

    INT GetMapMode()const{
      XASSERT(m_hDC!=NULL)
      return ::GetMapMode(m_hDC);
    }

    POINT GetViewportOrg()const{
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::GetViewportOrgEx(m_hDC,&point);
      return point;
    }

    INT SetMapMode(INT nMapMode)const{
      XASSERT(m_hDC!=NULL)
      return ::SetMapMode(m_hDC,nMapMode);
    }

    POINT SetViewportOrg(INT X,INT Y){
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::SetViewportOrgEx(m_hDC,X,Y,&point);
      return point;
    }

    POINT SetViewportOrg(POINT &point){
      XASSERT(m_hDC!=NULL)
      ::SetViewportOrgEx(m_hDC,point.x,point.y,&point);
      return point;
    }

    POINT OffsetViewportOrg(INT nXOffset,INT nYOffset){
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::OffsetViewportOrgEx(m_hDC,nXOffset,nYOffset,&point);
      return point;
    }

    SIZE GetViewportExt()const{
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::GetViewportExtEx(m_hDC,&size);
      return size;
    }

    SIZE SetViewportExt(INT cx,INT cy){
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::SetViewportExtEx(m_hDC,cx,cy,&size);
      return size;
    }

    SIZE SetViewportExt(SIZE &size){
      XASSERT(m_hDC!=NULL)
      ::SetViewportExtEx(m_hDC,size.cx,size.cy,&size);
      return size;
    }

    SIZE ScaleViewportExt(INT xNum,INT xDenom,INT yNum,INT yDenom){
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::ScaleViewportExtEx(m_hDC,xNum,xDenom,yNum,yDenom,&size);
      return size;
    }

    POINT GetWindowOrg()const{
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::GetWindowOrgEx(m_hDC,&point);
      return point;
    }

    POINT SetWindowOrg(INT X,INT Y){
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::SetWindowOrgEx(m_hDC,X,Y,&point);
      return point;
    }

    POINT SetWindowOrg(POINT &point){
      XASSERT(m_hDC!=NULL)
      ::SetWindowOrgEx(m_hDC,point.x,point.y,&point);
      return point;
    }

    POINT OffsetWindowOrg(INT nXOffset,INT nYOffset){
      XASSERT(m_hDC!=NULL)
      POINT point;
      ::OffsetWindowOrgEx(m_hDC,nXOffset,nYOffset,&point);
      return point;
    }

    SIZE GetWindowExt()const{
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::GetWindowExtEx(m_hDC,&size);
      return size;
    }

    SIZE SetWindowExt(INT cx,INT cy){
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::SetWindowExtEx(m_hDC,cx,cy,&size);
      return size;
    }

    SIZE SetWindowExt(SIZE &size){
      XASSERT(m_hDC!=NULL)
      ::SetWindowExtEx(m_hDC,size.cx,size.cy,&size);
      return size;
    }

    SIZE ScaleWindowExt(INT xNum,INT xDenom,INT yNum,INT yDenom){
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::ScaleWindowExtEx(m_hDC,xNum,xDenom,yNum,yDenom,&size);
      return size;
    }

    BOOL DPtoLP(LPPOINT lpPoints,INT nCount=1)const{
      XASSERT(m_hDC!=NULL)
      return ::DPtoLP(m_hDC,lpPoints,nCount);
    }

    BOOL DPtoLP(LPRECT lpRect)const{
      XASSERT(m_hDC!=NULL)
      return ::DPtoLP(m_hDC,(LPPOINT)lpRect,2);
    }

    VOID DPtoLP(LPSIZE lpSize)const{
      XASSERT(m_hDC!=NULL)
      SIZE sizeWinExt=GetWindowExt();
      SIZE sizeVpExt=GetViewportExt();
      lpSize->cx=MulDiv(lpSize->cx,abs(sizeWinExt.cx),abs(sizeVpExt.cx));
      lpSize->cy=MulDiv(lpSize->cy,abs(sizeWinExt.cy),abs(sizeVpExt.cy));
    }

    BOOL LPtoDP(LPPOINT lpPoints,INT nCount=1)const{
      XASSERT(m_hDC!=NULL)
      return ::LPtoDP(m_hDC,lpPoints,nCount);
    }

    BOOL LPtoDP(LPRECT lpRect)const{
      XASSERT(m_hDC!=NULL)
      return ::LPtoDP(m_hDC,(LPPOINT)lpRect,2);
    }

    VOID LPtoDP(LPSIZE lpSize)const{
      XASSERT(m_hDC!=NULL)
      SIZE sizeWinExt=GetWindowExt();
      SIZE sizeVpExt=GetViewportExt();
      lpSize->cx = MulDiv(lpSize->cx,abs(sizeVpExt.cx),abs(sizeWinExt.cx));
      lpSize->cy = MulDiv(lpSize->cy,abs(sizeVpExt.cy),abs(sizeWinExt.cy));
    }

    VOID DPtoHIMETRIC(LPSIZE lpSize)const;
    VOID HIMETRICtoDP(LPSIZE lpSize)const;

    VOID LPtoHIMETRIC(LPSIZE lpSize)const{
      XASSERT(lpSize!=NULL)
      LPtoDP(lpSize);
      DPtoHIMETRIC(lpSize);
    }

    VOID HIMETRICtoLP(LPSIZE lpSize)const{
      XASSERT(lpSize!=NULL)
      HIMETRICtoDP(lpSize);
      DPtoLP(lpSize);
    }

    BOOL FillRgn(HRGN hrgn,HBRUSH hbr){
      XASSERT(m_hDC!=NULL)
      return ::FillRgn(m_hDC,hrgn,hbr);
    }

    BOOL FrameRgn(HRGN hRgn,HBRUSH hBrush,INT nWidth,INT nHeight){
      XASSERT(m_hDC!=NULL)
      return ::FrameRgn(m_hDC,hRgn,hBrush,nWidth,nHeight);
    }

    BOOL InvertRgn(HRGN hRgn){
      XASSERT(m_hDC!=NULL)
      return ::InvertRgn(m_hDC,hRgn);
    }

    BOOL PaintRgn(HRGN hRgn){
      XASSERT(m_hDC!=NULL)
      return ::PaintRgn(m_hDC,hRgn);
    }

    INT GetClipBox(LPRECT lprc){
      XASSERT(m_hDC!=NULL)
      return ::GetClipBox(m_hDC,lprc);
    }

    BOOL PtVisible(INT x,INT y) const{
      XASSERT(m_hDC!=NULL)
      return ::PtVisible(m_hDC,x,y);
    }

    BOOL PtVisible(POINT point)const{
      XASSERT(m_hDC!=NULL)
      return ::PtVisible(m_hDC,point.x,point.y);
    }

    BOOL RectVisible(LPCRECT lpRect)const{
      XASSERT(m_hDC!=NULL)
      return ::RectVisible(m_hDC,lpRect);
    }

    INT SelectClipRgn(HRGN hrgn){
      XASSERT(m_hDC!=NULL)
      return ::SelectClipRgn(m_hDC,hrgn);
    }

    INT SelectClipRgn(HRGN hRgn,INT nMode){
      XASSERT(m_hDC!=NULL)
      return ::ExtSelectClipRgn(m_hDC,hRgn,nMode);
    }

    INT ExcludeClipRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect){
      XASSERT(m_hDC!=NULL)
      return ::ExcludeClipRect(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
    }

    INT ExcludeClipRect(LPRECT rc){
      XASSERT(m_hDC!=NULL)
      return ::ExcludeClipRect(m_hDC,rc->left,rc->top,rc->right,rc->bottom);
    }

    INT ExcludeUpdateRgn(HWND hWnd){
      XASSERT(m_hDC!=NULL)
      return ::ExcludeUpdateRgn(m_hDC,hWnd);
    }

    INT IntersectClipRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect){
      XASSERT(m_hDC!=NULL)
      return ::IntersectClipRect(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
    }

    INT IntersectClipRect(LPRECT rc){
      XASSERT(m_hDC!=NULL)
      return ::IntersectClipRect(m_hDC,rc->left,rc->top,rc->right,rc->bottom);
    }

    INT OffsetClipRgn(INT x,INT y){
      XASSERT(m_hDC!=NULL)
      return ::OffsetClipRgn(m_hDC,x,y);
    }

    INT OffsetClipRgn(SIZE size){
      XASSERT(m_hDC!=NULL)
      return ::OffsetClipRgn(m_hDC,size.cx,size.cy);
    }

    INT GetClipRgn(HRGN hrgn){
      XASSERT(m_hDC!=NULL)
      return ::GetClipRgn(m_hDC,hrgn);
    }

    POINT GetCurrentPosition(){
      XASSERT(m_hDC!=NULL)
      POINT pt;
      ::GetCurrentPositionEx(m_hDC,&pt);
      return pt;
    }

    POINT MoveTo(INT X,INT Y){
      XASSERT(m_hDC!=NULL)
      POINT pt;
      MoveToEx(m_hDC,X,Y,&pt);
      return pt;
    }

    LPPOINT MoveTo(LPPOINT pt){
      XASSERT(m_hDC!=NULL)
      ::MoveToEx(m_hDC,pt->x,pt->y,pt);
      return pt;
    }

    BOOL LineTo(INT nXEnd,INT nYEnd){
      XASSERT(m_hDC!=NULL)
      return ::LineTo(m_hDC,nXEnd,nYEnd);
    }

    BOOL LineTo(LPPOINT pt){
      XASSERT(m_hDC!=NULL)
      return ::LineTo(m_hDC,pt->x,pt->y);
    }

    BOOL Arc(INT x1,INT y1,INT x2,INT y2,INT x3,INT y3,INT x4,INT y4){
      XASSERT(m_hDC!=NULL)
      return ::Arc(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
    }

    BOOL Arc(LPCRECT lpRect,POINT ptStart,POINT ptEnd){
      XASSERT(m_hDC!=NULL)
      return ::Arc(m_hDC,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
    }

    BOOL Polyline(CONST LPPOINT lppt,INT cPoints){
      XASSERT(m_hDC!=NULL)
      return ::Polyline(m_hDC,lppt,cPoints);
    }

    BOOL AngleArc(INT x,INT y,INT nRadius,FLOAT fStartAngle,FLOAT fSweepAngle){
      XASSERT(m_hDC!=NULL)
      return ::AngleArc(m_hDC,x,y,nRadius,fStartAngle,fSweepAngle);
    }

    BOOL ArcTo(INT x1,INT y1,INT x2,INT y2,INT x3,INT y3,INT x4,INT y4){
      XASSERT(m_hDC!=NULL)
      return ::ArcTo(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
    }

    BOOL ArcTo(LPCRECT lpRect,POINT ptStart,POINT ptEnd){
      XASSERT(m_hDC!=NULL)
      return ArcTo(lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
    }

    INT GetArcDirection()const{
      XASSERT(m_hDC!=NULL)
      return ::GetArcDirection(m_hDC);
    }

    INT SetArcDirection(INT nArcDirection){
      XASSERT(m_hDC!=NULL)
      return ::SetArcDirection(m_hDC,nArcDirection);
    }

    BOOL PolyDraw(CONST LPPOINT lpPoints,CONST LPBYTE lpTypes,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolyDraw(m_hDC,lpPoints,lpTypes,nCount);
    }

    BOOL PolylineTo(CONST LPPOINT lpPoints,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolylineTo(m_hDC,lpPoints,nCount);
    }

    BOOL PolyPolyline(CONST LPPOINT lpPoints,CONST LPDWORD lpPolyPoints,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolyPolyline(m_hDC,lpPoints,lpPolyPoints,nCount);
    }

    BOOL PolyBezier(CONST LPPOINT lpPoints,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolyBezier(m_hDC,lpPoints,nCount);
    }

    BOOL PolyBezierTo(CONST LPPOINT lpPoints,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolyBezierTo(m_hDC,lpPoints,nCount);
    }

    INT FillRect(LPCRECT rc,HBRUSH hbr){
      XASSERT(m_hDC!=NULL)
      return ::FillRect(m_hDC,rc,hbr);
    }

    INT FillRect(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HBRUSH hbr){
      XASSERT(m_hDC!=NULL)
      RECT rc={nXDest,nYDest,nWidth,nHeight};
      return ::FillRect(m_hDC,&rc,hbr);
    }

    INT FrameRect(LPCRECT lpRect,HBRUSH hBrush){
      XASSERT(m_hDC!=NULL)
      return ::FrameRect(m_hDC,lpRect,hBrush);
    }

    INT FrameRect(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HBRUSH hbr){
      XASSERT(m_hDC!=NULL)
      RECT rc={nXDest,nYDest,nWidth,nHeight};
      return ::FrameRect(m_hDC,&rc,hbr);
    }

    BOOL InvertRect(LPCRECT lprc){
      XASSERT(m_hDC!=NULL)
      return ::InvertRect(m_hDC,lprc);
    }

    BOOL DrawIcon(INT X,INT Y,HICON hIcon){
      XASSERT(m_hDC!=NULL)
      return ::DrawIcon(m_hDC,X,Y,hIcon);
    }

    BOOL DrawIcon(POINT &pt,HICON hIcon){
      XASSERT(m_hDC!=NULL)
      return ::DrawIcon(m_hDC,pt.x,pt.y,hIcon);
    }

    BOOL DrawIconEx(INT X,INT Y,HICON hIcon,INT cX,INT cY,UINT bIfAniCur,HBRUSH hbrFFDraw,UINT iFlags){
      XASSERT(m_hDC!=NULL)
      return ::DrawIconEx(m_hDC,X,Y,hIcon,cX,cY,bIfAniCur,hbrFFDraw,iFlags);
    }

    BOOL DrawIconEx(POINT &pt,HICON hIcon,SIZE &sz,UINT bIfAniCur,HBRUSH hbrFFDraw,UINT iFlags){
      XASSERT(m_hDC!=NULL)
      return ::DrawIconEx(m_hDC,pt.x,pt.y,hIcon,sz.cx,sz.cy,bIfAniCur,hbrFFDraw,iFlags);
    }

    BOOL DrawState(const LPPOINT pt,const LPSIZE sz,HBITMAP hBitmap,UINT nFlags,HBRUSH hBrush=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawState(m_hDC,hBrush,NULL,(LPARAM)hBitmap,0,pt->x,pt->y,sz->cx,sz->cy,nFlags|DST_BITMAP);
    }

    BOOL DrawState(const LPPOINT pt,const LPSIZE sz,HICON hIcon,UINT nFlags,HBRUSH hBrush=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawState(m_hDC,hBrush,NULL,(LPARAM)hIcon,0,pt->x,pt->y,sz->cx,sz->cy,nFlags|DST_ICON);
    }

    BOOL DrawState(const LPPOINT pt,const LPSIZE sz,LPCTSTR lpszText,UINT nFlags,BOOL bPrefixText,INT nTextLen,HBRUSH hBrush=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawState(m_hDC,hBrush,NULL,(LPARAM)lpszText,(WPARAM)nTextLen,pt->x,pt->y,sz->cx,sz->cy,nFlags|(bPrefixText?DST_PREFIXTEXT:DST_TEXT));
    }

    BOOL DrawState(const LPPOINT pt,const LPSIZE sz,DRAWSTATEPROC lpOutputFunc,PVOID lData,INT nFlags,HBRUSH hbr=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawState(m_hDC,hbr,lpOutputFunc,(LPARAM)lData,0,pt->x,pt->y,sz->cx,sz->cy,nFlags|DST_COMPLEX);
    }

    BOOL DrawState(INT x,INT y,INT cx,INT cy,DRAWSTATEPROC lpOutputFunc,LPARAM lData,WPARAM wData,INT fuFlags,HBRUSH hbr=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawState(m_hDC,hbr,lpOutputFunc,lData,wData,x,y,cx,cy,fuFlags);
    }

    BOOL Chord(INT x1,INT y1,INT x2,INT y2,INT x3,INT y3,INT x4,INT y4){
      XASSERT(m_hDC!=NULL)
      return ::Chord(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
    }

    BOOL Chord(LPCRECT lpRect,POINT ptStart,POINT ptEnd){
      XASSERT(m_hDC!=NULL)
      return ::Chord(m_hDC,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
    }

    BOOL DrawFocusRect(LPCRECT lprc){
      XASSERT(m_hDC!=NULL)
      return ::DrawFocusRect(m_hDC,lprc);
    }

    BOOL Ellipse(LPCRECT rc){
      XASSERT(m_hDC!=NULL)
      return ::Ellipse(m_hDC,rc->left,rc->top,rc->right,rc->bottom);
    }

    BOOL Ellipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect){
      XASSERT(m_hDC!=NULL)
      return ::Ellipse(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
    }

    BOOL Pie(INT x1,INT y1,INT x2,INT y2,INT x3,INT y3,INT x4,INT y4){
      XASSERT(m_hDC!=NULL)
      return ::Pie(m_hDC,x1,y1,x2,y2,x3,y3,x4,y4);
    }

    BOOL Pie(LPCRECT lpRect,POINT ptStart,POINT ptEnd){
      XASSERT(m_hDC!=NULL)
      return ::Pie(m_hDC,lpRect->left,lpRect->top,lpRect->right,lpRect->bottom,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
    }

    BOOL Polygon(const LPPOINT lpPoints,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::Polygon(m_hDC,lpPoints,nCount);
    }

    BOOL PolyPolygon(const LPPOINT lpPoints,const PINT lpPolyCounts,INT nCount){
      XASSERT(m_hDC!=NULL)
      return ::PolyPolygon(m_hDC,lpPoints,lpPolyCounts,nCount);
    }

    BOOL Rectangle(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect){
      XASSERT(m_hDC!=NULL)
      return ::Rectangle(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
    }

    BOOL Rectangle(LPCRECT rc){
      XASSERT(m_hDC!=NULL)
      return ::Rectangle(m_hDC,rc->left,rc->top,rc->right,rc->bottom);
    }

    BOOL RoundRect(LPCRECT rc,POINT pt){
      XASSERT(m_hDC!=NULL)
      return ::RoundRect(m_hDC,rc->left,rc->top,rc->right,rc->bottom,pt.x,pt.y);
    }

    BOOL RoundRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,INT nWidth,INT nHeight){
      XASSERT(m_hDC!=NULL)
      return ::RoundRect(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect,nWidth,nHeight);
    }

    BOOL PatBlt(INT nXLeft,INT nYLeft,INT nWidth,INT nHeight,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::PatBlt(m_hDC,nXLeft,nYLeft,nWidth,nHeight,dwRop);
    }

    BOOL BitBlt(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HDC hdcSrc,INT nXSrc,INT nYSrc,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::BitBlt(m_hDC,nXDest,nYDest,nWidth,nHeight,hdcSrc,nXSrc,nYSrc,dwRop);
    }

    BOOL BitBlt(HDC hdcDest,INT nXDest,INT nYDest,INT nWidth,INT nHeight,INT nXSrc,INT nYSrc,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::BitBlt(hdcDest,nXDest,nYDest,nWidth,nHeight,m_hDC,nXSrc,nYSrc,dwRop);
    }

    BOOL StretchBlt(INT nXOriginDest,INT nYOriginDest,INT nWidthDest,INT nHeightDest,HDC hdcSrc,INT nXOriginSrc,INT nYOriginSrc,INT nWidthSrc,INT nHeightSrc,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::StretchBlt(m_hDC,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,hdcSrc,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,dwRop);
    }

    BOOL StretchBlt(HDC hdcDst,INT nXOriginDest,INT nYOriginDest,INT nWidthDest,INT nHeightDest,INT nXOriginSrc,INT nYOriginSrc,INT nWidthSrc,INT nHeightSrc,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::StretchBlt(hdcDst,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,m_hDC,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,dwRop);
    }

    COLORREF GetPixel(INT nXPos,INT nYPos)const{
      XASSERT(m_hDC!=NULL)
      return ::GetPixel(m_hDC,nXPos,nYPos);
    }

    COLORREF GetPixel(LPPOINT pt)const{
      XASSERT(m_hDC!=NULL)
      return ::GetPixel(m_hDC,pt->x,pt->y);
    }

    COLORREF SetPixel(INT nXPos,INT nYPos,COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetPixel(m_hDC,nXPos,nYPos,crColor);
    }

    COLORREF SetPixel(LPPOINT pt,COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetPixel(m_hDC,pt->x,pt->y,crColor);
    }

    BOOL FloodFill(INT x,INT y,COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::FloodFill(m_hDC,x,y,crColor);
    }

    BOOL ExtFloodFill(INT x,INT y,COLORREF crColor,UINT nFillType){
      XASSERT(m_hDC!=NULL)
      return ::ExtFloodFill(m_hDC,x,y,crColor,nFillType);
    }

    BOOL MaskBlt(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HDC hdcSrc,INT nXSrc,INT nYSrc,HBITMAP hbmMask,INT xMask,INT yMask,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::MaskBlt(m_hDC,nXDest,nYDest,nWidth,nHeight,hdcSrc,nXSrc,nYSrc,hbmMask,xMask,yMask,dwRop);
    }

    BOOL MaskBlt(HDC hdcDst,INT nXDest,INT nYDest,INT nWidth,INT nHeight,INT nXSrc,INT nYSrc,HBITMAP hbmMask,INT xMask,INT yMask,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::MaskBlt(hdcDst,nXDest,nYDest,nWidth,nHeight,m_hDC,nXSrc,nYSrc,hbmMask,xMask,yMask,dwRop);
    }

    BOOL StretchDIBits(INT XDest,INT YDest,INT nDestWidth,INT nDestHeight,INT XSrc,INT YSrc,INT nSrcWidth,INT nSrcHeight,CONST VOID* lpBits,CONST BITMAPINFO* lpBitsInfo,UINT iUsage,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::StretchDIBits(m_hDC,XDest,YDest,nDestWidth,nDestHeight,XSrc,YSrc,nSrcWidth,nSrcHeight,lpBits,lpBitsInfo,iUsage,dwRop);
    }

    BOOL PlgBlt(LPPOINT lpPoint,HDC hSrcDC,INT xSrc,INT ySrc,INT nWidth,INT nHeight,HBITMAP maskBitmap,INT xMask,INT yMask){
      XASSERT(m_hDC!=NULL)
      return ::PlgBlt(m_hDC,lpPoint,hSrcDC,xSrc,ySrc,nWidth,nHeight,maskBitmap,xMask,yMask);
    }

    BOOL PlgBlt(HDC hDestDC,LPPOINT lpPoint,INT xSrc,INT ySrc,INT nWidth,INT nHeight,HBITMAP maskBitmap,INT xMask,INT yMask){
      XASSERT(m_hDC!=NULL)
      return ::PlgBlt(hDestDC,lpPoint,m_hDC,xSrc,ySrc,nWidth,nHeight,maskBitmap,xMask,yMask);
    }

    BOOL SetPixelV(INT x,INT y,COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetPixelV(m_hDC,x,y,crColor);
    }

    BOOL SetPixelV(POINT point,COLORREF crColor){
      XASSERT(m_hDC!=NULL)
      return ::SetPixelV(m_hDC,point.x,point.y,crColor);
    }

  #if (_WIN32_WINDOWS >= 0x0600)
    BOOL GradientFill(PTRIVERTEX pVertex,ULONG nVertex,PVOID pMesh,ULONG nCount,ULONG ulMode){
      XASSERT(m_hDC!=NULL)
      return ::GradientFill(m_hDC,pVertex,nVertex,pMesh,nCount,ulMode);
    }
  #endif

  #if (_WIN32_WINDOWS >= 0x0600)
    BOOL TransparentBlt(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HDC hdcSrc,INT nXSrc,INT nYSrc,INT nSrcWidth,INT nSrcHeight,UINT clrTransparent){
      XASSERT(m_hDC!=NULL)
      return ::TransparentBlt(m_hDC,nXDest,nYDest,nWidth,nHeight,hdcSrc,nXSrc,nYSrc,nSrcWidth,nSrcHeight,clrTransparent);
    }

    BOOL TransparentBlt(HDC hdcDest,INT nXDest,INT nYDest,INT nWidth,INT nHeight,INT nXSrc,INT nYSrc,INT nSrcWidth,INT nSrcHeight,UINT clrTransparent){
      XASSERT(m_hDC!=NULL)
      return ::TransparentBlt(hdcDest,nXDest,nYDest,nWidth,nHeight,m_hDC,nXSrc,nYSrc,nSrcWidth,nSrcHeight,clrTransparent);
    }

    //Not Defined in Mingw WinAPI 3.17
    /*BOOL TransparentImage(INT nXDest,INT nYDest,INT nWidth,INT nHeight,HANDLE hdcSrc,INT nXSrc,INT nYSrc,DWORD dwRop){
      XASSERT(m_hDC!=NULL)
      return ::TransparentImage(m_hDC,nXDest,nYDest,nWidth,nHeight,hdcSrc,nXSrc,nYSrc,dwRop);
    }*/
  #endif

  #if (_WIN32_WINDOWS >= 0x0501)
    BOOL AlphaBlend(INT xDest,INT yDest,INT wDest,INT hDest,HDC hdcSrc,INT xSrc,INT ySrc,INT wSrc,INT hSrc,BLENDFUNCTION blend){
      XASSERT(m_hDC!=NULL)
      return ::AlphaBlend(m_hDC,xDest,yDest,wDest,hDest,hdcSrc,xSrc,ySrc,wSrc,hSrc,blend);
    }
  #endif

    BOOL TextOut(INT nXStart,INT nYStart,LPCTSTR lpString,INT cbString){
      XASSERT(m_hDC!=NULL)
      return ::TextOut(m_hDC,nXStart,nYStart,lpString,cbString);
    }

    BOOL ExtTextOut(INT X,INT Y,UINT fOpt,CONST RECT *lprc,LPCTSTR lpStr,UINT cCt,CONST INT *lpDx){
      XASSERT(m_hDC!=NULL)
      return ::ExtTextOut(m_hDC,X,Y,fOpt,lprc,lpStr,cCt,lpDx);
    }

    LONG TabbedTextOut(INT x,INT y,LPCTSTR lpszString,INT nCount,INT nTabPositions,LPINT lpnTabStopPositions,INT nTabOrigin){
      XASSERT(m_hDC!=NULL)
      return ::TabbedTextOut(m_hDC,x,y,lpszString,nCount,nTabPositions,lpnTabStopPositions,nTabOrigin);
    }

    INT DrawText(LPCTSTR lpString,INT nCount,LPRECT rect,UINT uFormat=0){
      XASSERT(m_hDC!=NULL)
      return ::DrawText(m_hDC,lpString,nCount,rect,uFormat);
    }

    INT DrawText(LPCTSTR lpString,LPRECT rect,UINT uFormat=0){
      XASSERT(m_hDC!=NULL)
      return ::DrawText(m_hDC,lpString,-1,rect,uFormat);
    }

    INT DrawTextEx(LPTSTR lpString,INT nCount,LPRECT rect,UINT uFormat,LPDRAWTEXTPARAMS lpDTP=NULL){
      XASSERT(m_hDC!=NULL)
      return ::DrawTextEx(m_hDC,lpString,nCount,rect,uFormat,lpDTP);
    }

    SIZE GetTextExtent(LPCTSTR lpString,INT cbString){
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::GetTextExtentPoint32(m_hDC,lpString,cbString,&size);
      return size;
    }

    DWORD GetTabbedTextExtent(LPCTSTR lpszString,INT nCount,INT nTabPositions,LPINT lpnTabStopPositions)const{
      XASSERT(m_hDC!=NULL)
      return ::GetTabbedTextExtent(m_hDC,lpszString,nCount,nTabPositions,lpnTabStopPositions);
    }

    BOOL GrayString(HBRUSH hBrush,GRAYSTRINGPROC lpfnOutput,LPARAM lpData,INT nCount,INT x,INT y,INT nWidth,INT nHeight){
      XASSERT(m_hDC!=NULL)
      return ::GrayString(m_hDC,hBrush,lpfnOutput,lpData,nCount,x,y,nWidth,nHeight);
    }

    UINT GetTextAlign()const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextAlign(m_hDC);
    }

    UINT SetTextAlign(UINT nFlags){
      XASSERT(m_hDC!=NULL)
			return ::SetTextAlign(m_hDC,nFlags);
    }

    INT GetTextFace(INT nCount,LPTSTR lpszFacename)const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextFace(m_hDC,nCount,lpszFacename);
    }

    BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics)const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextMetrics(m_hDC,lpMetrics);
    }

    INT SetTextJustification(INT nBreakExtra,INT nBreakCount){
      XASSERT(m_hDC!=NULL)
      return ::SetTextJustification(m_hDC,nBreakExtra,nBreakCount);
    }

    INT GetTextCharacterExtra()const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextCharacterExtra(m_hDC);
    }

    INT SetTextCharacterExtra(INT nCharExtra){
      XASSERT(m_hDC!=NULL)
      return ::SetTextCharacterExtra(m_hDC,nCharExtra);
    }

    DWORD GetCharacterPlacement(LPCTSTR lpString,INT nCount,INT nMaxExtent,LPGCP_RESULTS lpResults,DWORD dwFlags)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharacterPlacement(m_hDC,lpString,nCount,nMaxExtent,lpResults,dwFlags);
    }

  #if (_WIN32_WINDOWS >= 0x0501)
    //Not Defined in Mingw WinAPI 3.17
    /*BOOL GetCharABCWidthsI(UINT giFirst,UINT cgi,LPWORD pgi,LPABC lpabc)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharABCWidthsI(m_hDC,giFirst,cgi,pgi,lpabc);
    }

    BOOL GetCharWidthI(UINT giFirst,UINT cgi,LPWORD pgi,LPINT lpBuffer)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharWidthI(m_hDC,giFirst,cgi,pgi,lpBuffer);
    }

    BOOL GetTextExtentExPointI(LPWORD pgiIn,INT cgi,INT nMaxExtent,LPINT lpnFit,LPINT alpDx,LPSIZE lpSize)const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextExtentExPointI(m_hDC,pgiIn,cgi,nMaxExtent,lpnFit,alpDx,lpSize);
    }

    BOOL GetTextExtentPointI(LPWORD pgiIn,INT cgi,LPSIZE lpSize)const{
      XASSERT(m_hDC!=NULL)
      return ::GetTextExtentPointI(m_hDC,pgiIn,cgi,lpSize);
    }*/

  #endif

    BOOL DrawEdge(LPRECT rc,UINT edge,UINT grfFlags){
      XASSERT(m_hDC!=NULL)
      return ::DrawEdge(m_hDC,rc,edge,grfFlags);
    }

    BOOL DrawFrameControl(LPRECT lprc,UINT uType,UINT uState){
      XASSERT(m_hDC!=NULL)
      return ::DrawFrameControl(m_hDC,lprc,uType,uState);
    }

    BOOL ScrollDC(INT dx,INT dy,LPCRECT lpRectScroll,LPCRECT lpRectClip,HRGN hRgnUpdate,LPRECT lpRectUpdate){
      XASSERT(m_hDC!=NULL)
      return ::ScrollDC(m_hDC,dx,dy,lpRectScroll,lpRectClip,hRgnUpdate,lpRectUpdate);
    }

    BOOL GetCharWidth(UINT iFirstChar,UINT iLastChar,PINT lpBuffer){
      XASSERT(m_hDC!=NULL)
    #if (_WIN32_WINDOWS >= 0x0500)
      return ::GetCharWidth32(m_hDC,iFirstChar,iLastChar,lpBuffer);
    #else
      return ::GetCharWidth(m_hDC,iFirstChar,iLastChar,lpBuffer);
    #endif
    };

    DWORD SetMapperFlags(DWORD dwFlag){
      XASSERT(m_hDC!=NULL)
      return ::SetMapperFlags(m_hDC,dwFlag);
    }

    UINT GetDIBColorTable(UINT uStartIndex,UINT cEntries,RGBQUAD* pColors){
      XASSERT(m_hDC!=NULL)
      return ::GetDIBColorTable(m_hDC,uStartIndex,cEntries,pColors);
    }

    UINT SetDIBColorTable(UINT uStartIndex,UINT cEntries,CONST RGBQUAD* pColor){
      XASSERT(m_hDC!=NULL)
      return ::SetDIBColorTable(m_hDC,uStartIndex,cEntries,pColor);
    }

    INT SetDIBitsToDevice(INT XDest,INT YDest,DWORD dwWidth,DWORD dwHeight,INT XSrc,INT YSrc,
      UINT uStartScan,UINT cScanLines,CONST VOID* lpvBits,CONST BITMAPINFO* lpbmi,UINT fuColorUse){
      XASSERT(m_hDC!=NULL)
      return ::SetDIBitsToDevice(m_hDC,XDest,YDest,dwWidth,dwHeight,XSrc,YSrc,uStartScan,cScanLines,lpvBits,lpbmi,fuColorUse);
    }

    SIZE GetAspectRatioFilter()const{
      XASSERT(m_hDC!=NULL)
      SIZE size;
      ::GetAspectRatioFilterEx(m_hDC,&size);
      return size;
    }

    BOOL GetCharABCWidths(UINT nFirstChar,UINT nLastChar,LPABC lpabc)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharABCWidths(m_hDC,nFirstChar,nLastChar,lpabc);
    }

    DWORD GetFontData(DWORD dwTable,DWORD dwOffset,LPVOID lpData,DWORD cbData)const{
      XASSERT(m_hDC!=NULL)
      return ::GetFontData(m_hDC,dwTable,dwOffset,lpData,cbData);
    }

    INT GetKerningPairs(INT nPairs,LPKERNINGPAIR lpkrnpair) const{
      XASSERT(m_hDC!=NULL)
      return ::GetKerningPairs(m_hDC,nPairs,lpkrnpair);
    }

    DWORD GetGlyphOutline(UINT nChar,UINT nFormat,LPGLYPHMETRICS lpgm,DWORD cbBuffer,LPVOID lpBuffer,const MAT2* lpmat2) const{
      XASSERT(m_hDC!=NULL)
      return ::GetGlyphOutline(m_hDC,nChar,nFormat,lpgm,cbBuffer,lpBuffer,lpmat2);
    }

    BOOL GetCharABCWidths(UINT nFirstChar,UINT nLastChar,LPABCFLOAT lpABCF)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharABCWidthsFloat(m_hDC,nFirstChar,nLastChar,lpABCF);
    }

    BOOL GetCharWidth(UINT nFirstChar,UINT nLastChar,PFLOAT lpFloatBuffer)const{
      XASSERT(m_hDC!=NULL)
      return ::GetCharWidthFloat(m_hDC,nFirstChar,nLastChar,lpFloatBuffer);
    }

    UINT GetOutlineTextMetrics(UINT cbData,LPOUTLINETEXTMETRIC lpOTM){
      XASSERT(m_hDC!=NULL)
      return ::GetOutlineTextMetrics(m_hDC,cbData,lpOTM);
    }

    DWORD GetFontLanguageInfo()const{
      XASSERT(m_hDC!=NULL)
      return ::GetFontLanguageInfo(m_hDC);
    }

    INT Escape(INT nEscape,INT nCount,LPCSTR lpszInData,LPVOID lpOutData){
      XASSERT(m_hDC!=NULL)
      return ::Escape(m_hDC,nEscape,nCount,lpszInData,lpOutData);
    }

    INT ExtEscape(INT nEscape,INT cbInput,LPCSTR lpszInData,INT cbOutput,LPSTR lpszOutData){
      XASSERT(m_hDC!=NULL)
      return ::ExtEscape(m_hDC,nEscape,cbInput,lpszInData,cbOutput,lpszOutData);
    }

    INT DrawEscape(INT nEscape,INT nInputSize,LPCSTR lpszInputData){
      XASSERT(m_hDC!=NULL)
      return ::DrawEscape(m_hDC,nEscape,nInputSize,lpszInputData);
    }

    INT StartDoc(LPDOCINFO lpDocInfo){
      XASSERT(m_hDC!=NULL)
      return ::StartDoc(m_hDC,lpDocInfo);
    }

    INT StartPage(){
      XASSERT(m_hDC!=NULL)
      return ::StartPage(m_hDC);
    }

    INT EndPage(){
      XASSERT(m_hDC!=NULL)
      return ::EndPage(m_hDC);
    }

    INT SetAbortProc(ABORTPROC lpfn){
      XASSERT(m_hDC!=NULL)
      return ::SetAbortProc(m_hDC,lpfn);
    }

    INT AbortDoc(){
      XASSERT(m_hDC!=NULL)
      return ::AbortDoc(m_hDC);
    }

    INT EndDoc(){
      XASSERT(m_hDC!=NULL)
      return ::EndDoc(m_hDC);
    }

    BOOL AddMetaFileComment(UINT nDataSize,const BYTE* pCommentData){
      XASSERT(m_hDC!=NULL)
      return ::GdiComment(m_hDC,nDataSize,pCommentData);
    }

    BOOL EnumMetaFile(HMETAFILE hmf,MFENUMPROC lpMetaFunc,LPARAM lParam){
      XASSERT(m_hDC!=NULL)
      return ::EnumMetaFile(m_hDC,hmf,lpMetaFunc,lParam);
    }

    BOOL PlayMetaFile(HENHMETAFILE hEnhMF,LPCRECT lpBounds){
      XASSERT(m_hDC!=NULL)
      return ::PlayEnhMetaFile(m_hDC,hEnhMF,lpBounds);
    }

    BOOL AbortPath(){
      XASSERT(m_hDC!=NULL)
      return ::AbortPath(m_hDC);
    }

    BOOL BeginPath(){
      XASSERT(m_hDC!=NULL)
      return ::BeginPath(m_hDC);
    }

    BOOL CloseFigure(){
      XASSERT(m_hDC!=NULL)
      return ::CloseFigure(m_hDC);
    }

    BOOL EndPath(){
      XASSERT(m_hDC!=NULL)
      return ::EndPath(m_hDC);
    }

    BOOL FillPath(){
      XASSERT(m_hDC!=NULL)
      return ::FillPath(m_hDC);
    }

    BOOL FlattenPath(){
      XASSERT(m_hDC!=NULL)
      return ::FlattenPath(m_hDC);
    }

    FLOAT GetMiterLimit() const{
      XASSERT(m_hDC!=NULL)
      FLOAT fMiterLimit;
      ::GetMiterLimit(m_hDC,&fMiterLimit);
      return fMiterLimit;
    }

    INT GetPath(LPPOINT lpPoints,LPBYTE lpTypes,INT nCount) const{
      XASSERT(m_hDC!=NULL)
      return ::GetPath(m_hDC,lpPoints,lpTypes,nCount);
    }

    BOOL SetMiterLimit(FLOAT fMiterLimit){
      XASSERT(m_hDC!=NULL)
      return ::SetMiterLimit(m_hDC,fMiterLimit,NULL);
    }

    BOOL StrokeAndFillPath(){
      XASSERT(m_hDC!=NULL)
      return ::StrokeAndFillPath(m_hDC);
    }

    BOOL StrokePath(){
      XASSERT(m_hDC!=NULL)
      return ::StrokePath(m_hDC);
    }

    BOOL WidenPath(){
      XASSERT(m_hDC!=NULL)
      return ::WidenPath(m_hDC);
    }

    BOOL SelectClipPath(INT nMode){
      XASSERT(m_hDC!=NULL)
      return ::SelectClipPath(m_hDC,nMode);
    }

    UINT GetSystemPaletteEntries(UINT iStartIndex,UINT nEntries,LPPALETTEENTRY lppe){
      XASSERT(m_hDC!=NULL)
      return ::GetSystemPaletteEntries(m_hDC,iStartIndex,nEntries,lppe);
    }

    /********************************************************************************\
      Extra Drawing Functions...
    \********************************************************************************/

    VOID Draw3dRect(LPCRECT lpRect,COLORREF clrTopLeft,COLORREF clrBottomRight);
    VOID Draw3dRect(INT x,INT y,INT cx,INT cy,COLORREF clrTopLeft,COLORREF clrBottomRight);

    VOID DrawShadow(LPRECT rc,COLORREF color);
    VOID DrawGradient(LPRECT rc,COLORREF color1,COLORREF color2,BYTE direction);

    VOID FillSolidRgn(HRGN hrgn,COLORREF crColor);

    VOID DrawSolidRoundRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,INT nWidth,INT nHeight,COLORREF outlincol,COLORREF fillcol,INT lWidth=0);
    VOID DrawSolidRoundRect(LPRECT rc,LPPOINT pt,COLORREF outlincol,COLORREF fillcol,INT nWidth=0);

    VOID FillSolidRect(LPRECT rc,COLORREF crColor);
    VOID FillSolidRect(INT nXDest,INT nYDest,INT nWidth,INT nHeight,COLORREF crColor);

    VOID DrawSolidRectangle(LPRECT rc,COLORREF outlincol,COLORREF fillcol,INT nWidth=0);
    VOID DrawSolidRectangle(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,COLORREF fillcol,INT nWidth=0);

    VOID FrameSolidRect(LPRECT rc,COLORREF outlincol,INT nWidth=0);
    VOID FrameSolidRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,INT nWidth=0);

    VOID FillSolidEllipse(LPRECT rc,COLORREF crColor);
    VOID FillSolidEllipse(INT nXDest,INT nYDest,INT nWidth,INT nHeight,COLORREF crColor);

    VOID DrawSolidEllipse(LPRECT rc,COLORREF outlincol,COLORREF fillcol,INT nWidth=0);
    VOID DrawSolidEllipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,COLORREF fillcol,INT nWidth=0);

    VOID FrameSolidEllipse(LPRECT rc,COLORREF outlincol,INT nWidth=0);
    VOID FrameSolidEllipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,INT nWidth=0);

    VOID DrawSolidLine(LPPOINT pt,LPSIZE sz,COLORREF lincol,INT nWidth=0);
    VOID DrawSolidLine(INT x,INT y,INT cx,INT cy,COLORREF lincol,INT nWidth=0);

    VOID DrawColoredText(LPCTSTR lpString,LPRECT rect,INT ibkmode,COLORREF bkgcol,COLORREF txtcol,UINT uFormat=0,INT nCount=-1);
    VOID TextOutEx(INT nXStart,INT nYStart,LPCTSTR lpString,INT ibkmode,COLORREF bkgcol,COLORREF txtcol);
    BOOL DrawSpecialChar(UINT x,UINT y,TCHAR Sign,COLORREF cColor=0,UINT size=14);

    BOOL PolyPatBlt(PRECT pRects,UINT cRects,HBRUSH hBrush,DWORD dwRop=PATCOPY);
    BOOL FramePatBlt(PRECT pRect,UINT uWidth,HBRUSH hBrush,DWORD dwRop=PATCOPY);

    XDC &operator=(const HDC hDC){
      XASSERT(m_hDC==NULL)
      m_hDC=hDC;
      return *this;
    }

    BOOL operator==(const HDC hDC) const{
      return (hDC==m_hDC);
    }

    BOOL operator!=(const HDC hDC) const{
      return (hDC!=m_hDC);
    }

    operator HDC() const {
      return (this == NULL ? NULL : m_hDC);
    }

  protected:
    HDC m_hDC;
    BOOL m_bAttached;
};

class XPaintDC:public XDC{
	DECLARE_DYNAMIC(XPaintDC)
  public:
    XPaintDC(HWND hWnd){
      XASSERT(::IsWindow(hWnd))
      Attach(::BeginPaint(m_hWnd=hWnd,&m_ps));
    }
    virtual ~XPaintDC(){
      if(m_hDC!=NULL&&::IsWindow(m_hWnd)){
        ::EndPaint(m_hWnd,&m_ps);
        Detach();
      }
    }
  public:
    PAINTSTRUCT m_ps;
  protected:
    HWND m_hWnd;
};

class XClientDC:public XDC{
	DECLARE_DYNAMIC(XClientDC)
  public:
    XClientDC(HWND hWnd=NULL){
      Attach(::GetDC(m_hWnd=hWnd));
    }
    virtual ~XClientDC(){
      if(m_hDC!=NULL)
        ::ReleaseDC(m_hWnd,Detach());
    }
  protected:
    HWND m_hWnd;
};

class XWindowDC:public XDC{
	DECLARE_DYNAMIC(XWindowDC)
  public:
    XWindowDC(HWND hWnd=NULL){
      Attach(::GetWindowDC(m_hWnd=hWnd));
    }
    virtual ~XWindowDC(){
      if(m_hDC!=NULL)
        ::ReleaseDC(m_hWnd,Detach());
    }
  protected:
    HWND m_hWnd;
};

class XDCEx:public XDC{
	DECLARE_DYNAMIC(XDCEx)
  public:
    XDCEx(HWND hWnd,HRGN hrgnClip,DWORD flags){
      Attach(::GetDCEx(m_hWnd=hWnd,hrgnClip,flags));
    }
    virtual ~XDCEx(){
      if(m_hDC!=NULL)
        ::ReleaseDC(m_hWnd,Detach());
    }
  protected:
    HWND m_hWnd;
};

/**************************************************************/
// XGDIOBJECT REFERENCE...
/**************************************************************/

class XGDIObject:public XObject{
	DECLARE_DYNAMIC(XGDIObject)
  public:
    XGDIObject(HGDIOBJ hObject=NULL):m_hObject(hObject),m_bAttached(FALSE){}

    virtual ~XGDIObject(){
      if(m_hObject&&m_bAttached)
        Delete();
    }

    BOOL IsValid()const{
      return (this==NULL?FALSE:m_hObject!=NULL);
    }

    static XGDIObject* FromHandlePermanent(HGDIOBJ hObject){
      XASSERT(hObject!=NULL)
      XGDIObject* pObj=NULL;
      if(hObject)
        pObj=XWinApp::m_XGDIMap.Find(hObject);
      return pObj;
    }

    static XGDIObject* FromHandle(HGDIOBJ hObject){
      XASSERT(hObject!=NULL)
      XGDIObject* pObj=NULL;
      if(hObject){
        pObj=FromHandlePermanent(hObject);
        if(!pObj){
          pObj=new XGDIObject();
          if(pObj)pObj->Attach(hObject);
        }
      }
      return pObj;
    }

    HGDIOBJ Attach(HGDIOBJ hObject){
      XASSERT(m_hObject==NULL)
      if(!hObject)return NULL;
      m_bAttached=XWinApp::m_XGDIMap.Add(hObject,this);
      m_hObject=hObject;
      return hObject;
    }

    HGDIOBJ Detach(){
      HGDIOBJ hObject=m_hObject;
      if(hObject){
        if(m_bAttached){
          XWinApp::m_XGDIMap.Remove(hObject);
          m_bAttached=FALSE;
        }
        m_hObject=NULL;
      }
      return hObject;
    }

    HGDIOBJ GetSafeHandle()const{
      return (this==NULL?NULL:m_hObject);
    }

    BOOL Create(INT nIndex){
      return Attach(::GetStockObject(nIndex))!=NULL;
    }

    BOOL Delete(){
      if(m_hObject)
        return ::DeleteObject(Detach());
      return FALSE;
    }

    INT GetObject(INT cbBuffer,LPVOID lpObject) const {
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,cbBuffer,lpObject);
    }

    DWORD GetType() const {
      XASSERT(m_hObject!=NULL)
      return ::GetObjectType(m_hObject);
    }

    BOOL Unrealize(){
      XASSERT(m_hObject!=NULL)
      return ::UnrealizeObject(m_hObject);
    }

    XGDIObject &operator=(const HGDIOBJ hObject){
      XASSERT(m_hObject==NULL)
      m_hObject=hObject;
      return *this;
    }

    BOOL operator==(const HGDIOBJ hObject)const{
      return (hObject==m_hObject);
    }

    BOOL operator!=(const HGDIOBJ hObject)const{
      return (hObject!=m_hObject);
    }

    operator HGDIOBJ()const{
      return (this==NULL?NULL:m_hObject);
    }

  protected:
    HGDIOBJ m_hObject;
    BOOL m_bAttached;
};

/**************************************************************/
// XPen REFERENCE...
/**************************************************************/

class XPen:public XGDIObject{
  DECLARE_DYNAMIC(XPen)
  public:
    XPen(HPEN hPen=NULL):XGDIObject(hPen){};
    XPen(INT nPenStyle,INT nWidth,COLORREF crColor){
      Attach(::CreatePen(nPenStyle,nWidth,crColor));
    }
    XPen(INT nPenStyle,INT nWidth,const LOGBRUSH* lplb,INT dwSCount=0,const DWORD* lpStyle=NULL){
      Attach(::ExtCreatePen(nPenStyle,nWidth,lplb,dwSCount,lpStyle));
    }
    virtual ~XPen(){}

    XPen *FromHandle(HPEN hPen){
      return (XPen*)XGDIObject::FromHandle(hPen);
    }

    HPEN Create(INT nPenStyle,INT nWidth,COLORREF crColor){
      return (HPEN)Attach(::CreatePen(nPenStyle,nWidth,crColor));
    }

    HPEN CreateIndirect(CONST LOGPEN *lplgpn){
      return (HPEN)Attach(::CreatePenIndirect(lplgpn));
    }

    HPEN ExtCreate(INT nPenStyle,INT nWidth,const LOGBRUSH* lplb,INT dwSCount=0,const DWORD* lpStyle=NULL){
      return (HPEN)Attach(::ExtCreatePen(nPenStyle,nWidth,lplb,dwSCount,lpStyle));
    }

    INT GetExtLog(EXTLOGPEN *pExgLogPen){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(EXTLOGPEN),pExgLogPen);
    }

    INT GetLogPen(LOGPEN* pLogPen){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(LOGPEN),pLogPen);
    }

    XPen &operator=(HPEN hPen){
      XASSERT(m_hObject==NULL)
      m_hObject=hPen;
      return *this;
    }

    operator HPEN() const {
      return (HPEN)m_hObject;
    }
};

/**************************************************************/
// XBrush REFERENCE...
/**************************************************************/

class XBrush:public XGDIObject{
  DECLARE_DYNAMIC(XBrush)
  public:
    XBrush(HBRUSH hBrush=NULL):XGDIObject(hBrush){}
    XBrush(COLORREF crColor){
      Attach(::CreateSolidBrush(crColor));
    }
    XBrush(INT fnStyle,COLORREF clrref){
      Attach(::CreateHatchBrush(fnStyle,clrref));
    }
    XBrush(HBITMAP hBmp){
      Attach(::CreatePatternBrush(hBmp));
    }
    virtual ~XBrush(){}

    static XBrush *FromHandle(HBRUSH hBrush){
      return (XBrush*)XGDIObject::FromHandle(hBrush);
    }

    HBRUSH CreateSolid(COLORREF crColor){
      return (HBRUSH)Attach(::CreateSolidBrush(crColor));
    }

    HBRUSH CreateHatch(INT fnStyle,COLORREF clrref){
      return (HBRUSH)Attach(::CreateHatchBrush(fnStyle,clrref));
    }

    HBRUSH CreateIndirect(CONST LOGBRUSH *lplb){
      return (HBRUSH)Attach(::CreateBrushIndirect(lplb));
    }

    HBRUSH CreatePattern(HBITMAP hBmp){
      return (HBRUSH)Attach(::CreatePatternBrush(hBmp));
    }

    HBRUSH CreateDIBPattern(HGLOBAL hglbDIBPacked,UINT fuColorSpec){
      return (HBRUSH)Attach(::CreateDIBPatternBrush(hglbDIBPacked,fuColorSpec));
    }

    HBRUSH CreateDIBPattern(CONST VOID *lpPackedDIB,UINT iUsage){
      return (HBRUSH)Attach(::CreateDIBPatternBrushPt(lpPackedDIB,iUsage));
    }

    HBRUSH CreateSysColor(UINT nIndex){
      return (HBRUSH)Attach(::CreateSolidBrush(GetSysColor(nIndex)));
    }

    INT GetLogBrush(LOGBRUSH *pLogBrush){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(LOGBRUSH),pLogBrush);
    }

    XBrush &operator=(HBRUSH hBrush){
      XASSERT(m_hObject==NULL)
      m_hObject=hBrush;
      return *this;
    }

    operator HBRUSH() const {
      return (HBRUSH)m_hObject;
    }
};

/**************************************************************/
// XFont REFERENCE...
/**************************************************************/

class XFont:public XGDIObject{
  DECLARE_DYNAMIC(XFont)
  public:
    XFont(HFONT hFont=NULL):XGDIObject(hFont){}
    virtual ~XFont(){}

    static XFont *FromHandle(HFONT hFont){
      return (XFont*)XGDIObject::FromHandle(hFont);
    }

    HFONT Create(INT nH,INT nW,INT nE,INT nO,INT fnW,UINT fdwI,
      UINT fdwU,UINT fdwSO,UINT fdwCS,UINT fdwOP,UINT fdwCP,UINT fdwQ,
      UINT fdwPAF,LPCTSTR lpszF){
      return (HFONT)Attach(::CreateFont(nH,nW,nE,nO,fnW,fdwI,fdwU,fdwSO,fdwCS,fdwOP,fdwCP,fdwQ,fdwPAF,lpszF));
    }

    HFONT CreateIndirect(CONST LOGFONT *lplf){
      return (HFONT)Attach(::CreateFontIndirect(lplf));
    }

    HFONT CreatePointFont(INT nPointSize,LPCTSTR lpszFaceName,HDC hDC=NULL,BOOL bBold=FALSE,BOOL bItalic=FALSE,BOOL bUnderline=FALSE);
    HFONT CreatePointFontIndirect(LOGFONT* lpLogFont,HDC hDC=NULL);

    INT GetLogFont(LOGFONT *pLogFont){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(LOGFONT),pLogFont);
    }

    XFont &operator=(HFONT hFont){
      XASSERT(m_hObject==NULL)
      m_hObject=hFont;
      return *this;
    }

    operator HFONT() const {
      return (HFONT)m_hObject;
    }
};

/**************************************************************/
// XBitmap REFERENCE...
/**************************************************************/

class XBitmap:public XGDIObject{
  DECLARE_DYNAMIC(XBitmap)
  public:
    XBitmap(HBITMAP hBitmap=NULL):XGDIObject(hBitmap){};
    virtual ~XBitmap(){}

    static XBitmap *FromHandle(HBITMAP hBitmap){
      return (XBitmap*)XGDIObject::FromHandle(hBitmap);
    }

    HBITMAP LoadBitmap(HINSTANCE hInst,UINT nIDBitmap){
      return (HBITMAP)Attach(::LoadBitmap(hInst,MAKEINTRESOURCE(nIDBitmap)));
    }

    HBITMAP LoadBitmap(HINSTANCE hInst,LPCTSTR szResourceName){
      return (HBITMAP)Attach(::LoadBitmap(hInst,szResourceName));
    }

    HBITMAP LoadOEMBitmap(UINT nIDBitmap){
      return (HBITMAP)Attach(::LoadBitmap(NULL,MAKEINTRESOURCE(nIDBitmap)));
    }

    HBITMAP LoadOEMBitmap(LPCTSTR szResourceName){
      return (HBITMAP)Attach(::LoadBitmap(NULL,szResourceName));
    }

    HBITMAP Create(INT nWidth,INT nHeight,UINT cPlanes,UINT cBitsPerPel,const LPVOID lpvBits){
      return (HBITMAP)Attach(::CreateBitmap(nWidth,nHeight,cPlanes,cBitsPerPel,lpvBits));
    }

    HBITMAP CreateIndirect(const BITMAP *lpbm){
      return (HBITMAP)Attach(::CreateBitmapIndirect(lpbm));
    }

    HBITMAP CreateCompatible(HDC hdc,UINT x,UINT y){
      return (HBITMAP)Attach(::CreateCompatibleBitmap(hdc,x,y));
    }

    HBITMAP CreateDiscardable(HDC hdc,INT nWidth,INT nHeight){
      return (HBITMAP)Attach(::CreateDiscardableBitmap(hdc,nWidth,nHeight));
    }

    HBITMAP CreateDIBitmap(HDC hdc,CONST BITMAPINFOHEADER *lpbmih,DWORD fdwInit,const LPVOID lpbInit,CONST BITMAPINFO *lpbmi,UINT fuUsage){
      return (HBITMAP)Attach(::CreateDIBitmap(hdc,lpbmih,fdwInit,lpbInit,lpbmi,fuUsage));
    }

    HBITMAP CreateDIBSection(HDC hdc,CONST BITMAPINFO *lpbmi,UINT iUsage,PVOID *lpbInit,HANDLE hSection,DWORD dwOffset){
      return (HBITMAP)Attach(::CreateDIBSection(hdc,lpbmi,iUsage,lpbInit,hSection,dwOffset));
    }

    HBITMAP CreateMapped(HINSTANCE hInst,INT idB,UINT wF,LPCOLORMAP lpCM,INT iNM){
      return (HBITMAP)Attach(::CreateMappedBitmap(hInst,idB,wF,lpCM,iNM));
    }

    LONG SetBitmapBits(DWORD cBytes,const LPVOID lpBits){
      XASSERT(m_hObject!=NULL)
      return ::SetBitmapBits((HBITMAP)m_hObject,cBytes,lpBits);
    }

    LONG GetBitmapBits(DWORD cBytes,LPVOID lpBits){
      XASSERT(m_hObject!=NULL)
      return ::GetBitmapBits((HBITMAP)m_hObject,cBytes,lpBits);
    }

    SIZE SetBitmapDimension(INT nWidth,INT nHeight){
      SIZE size;
      XASSERT(m_hObject!=NULL)
      ::SetBitmapDimensionEx((HBITMAP)m_hObject,nWidth,nHeight,&size);
      return size;
    }

    SIZE SetBitmapDimension(LPSIZE nSize){
      SIZE size;
      XASSERT(m_hObject!=NULL)
      ::SetBitmapDimensionEx((HBITMAP)m_hObject,nSize->cx,nSize->cy,&size);
      return size;
    }

    SIZE GetBitmapDimension(){
      SIZE size;
      XASSERT(m_hObject!=NULL)
      ::GetBitmapDimensionEx((HBITMAP)m_hObject,&size);
      return size;
    }

    INT GetDIBits(HDC hdc,UINT uSS,UINT cSL,PVOID lpvBits,LPBITMAPINFO lpbi,UINT uU){
      XASSERT(m_hObject!=NULL)
      return ::GetDIBits(hdc,(HBITMAP)m_hObject,uSS,cSL,lpvBits,lpbi,uU);
    }

    INT SetDIBits(HDC hdc,UINT uSS,UINT cSL,const LPVOID lpvBits,CONST BITMAPINFO *lpbmi,UINT fuCU){
      XASSERT(m_hObject!=NULL)
      return ::SetDIBits(hdc,(HBITMAP)m_hObject,uSS,cSL,lpvBits,lpbmi,fuCU);
    }

    INT GetBitmap(LPBITMAP pBitMap){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(BITMAP),pBitMap);
    }

    INT GetDibSection(DIBSECTION *pDibSection){
      XASSERT(m_hObject!=NULL)
      return ::GetObject(m_hObject,sizeof(DIBSECTION),pDibSection);
    }

    BOOL Draw(HDC hdc,LPPOINT pt,LPSIZE bsz,LPPOINT spt,HBITMAP g_bitmask=NULL);

    XBitmap &operator=(HBITMAP hBitmap){
      XASSERT(m_hObject==NULL)
      m_hObject=hBitmap;
      return *this;
    }

    operator HBITMAP() const {
      return (HBITMAP)m_hObject;
    }
};

/**************************************************************/
// XPalette REFERENCE...
/**************************************************************/

class XPalette:public XGDIObject{
  DECLARE_DYNAMIC(XPalette)
  public:
    XPalette(HPALETTE hPalette=NULL):XGDIObject(hPalette){}
    virtual ~XPalette(){}

    static XPalette *FromHandle(HPALETTE hPalette){
      return (XPalette*)XGDIObject::FromHandle(hPalette);
    }

    HPALETTE Create(CONST LOGPALETTE *lplgpl){
      return (HPALETTE)Attach(::CreatePalette(lplgpl));
    }

    HPALETTE CreateHalftone(HDC hdc){
      return (HPALETTE)Attach(::CreateHalftonePalette(hdc));
    }

    INT GetEntryCount(){
      return ::GetPaletteEntries((HPALETTE)m_hObject,0,(UINT)-1,NULL);
    }

    UINT GetEntries(UINT nSI,UINT nNE,LPPALETTEENTRY lpPE)const{
      return ::GetPaletteEntries((HPALETTE)m_hObject,nSI,nNE,lpPE);
    }

    UINT SetEntries(UINT nSI,UINT nNE,LPPALETTEENTRY lpPE){
      return ::SetPaletteEntries((HPALETTE)m_hObject,nSI,nNE,lpPE);
    }

    BOOL Animate(UINT nSI,UINT nNE,CONST PALETTEENTRY *ppe){
      return ::AnimatePalette((HPALETTE)m_hObject,nSI,nNE,ppe);
    }

    UINT GetNearestIndex(COLORREF crColor)const{
      return ::GetNearestPaletteIndex((HPALETTE)m_hObject,crColor);
    }

    BOOL Resize(UINT nEntries){
      return ::ResizePalette((HPALETTE)m_hObject,nEntries);
    }

    XPalette &operator=(HPALETTE hPalette){
      m_hObject=hPalette;
      return *this;
    }

    operator HPALETTE() const {
      return (HPALETTE)m_hObject;
    }
};

/**************************************************************/
// XRgn REFERENCE...
/**************************************************************/

class XRgn:public XGDIObject{
  DECLARE_DYNAMIC(XRgn)
  public:
    XRgn(HRGN hRgn):XGDIObject(hRgn){}
    virtual ~XRgn(){}

    static XRgn *FromHandle(HRGN hRgn){
      return (XRgn*)XGDIObject::FromHandle(hRgn);
    }

    HRGN CreateRectRgn(INT x,INT y,INT cx,INT cy){
      return (HRGN)Attach(::CreateRectRgn(x,y,cx,cy));
    }

    HRGN CreateRectRgnIndirect(CONST RECT *lprc){
      return (HRGN)Attach(::CreateRectRgnIndirect(lprc));
    }

    HRGN CreateEllipticRgn(INT x,INT y,INT cx,INT cy){
      return (HRGN)Attach(::CreateEllipticRgn(x,y,cx,cy));
    }

    HRGN CreateEllipticRgnIndirect(CONST RECT *lprc){
      return (HRGN)Attach(::CreateEllipticRgnIndirect(lprc));
    }

    HRGN CreatePolygonRgn(const POINT *lppt,INT cPoints,INT fnPolyFillMode){
      return (HRGN)Attach(::CreatePolygonRgn(lppt,cPoints,fnPolyFillMode));
    }

    HRGN CreatePolyPolygonRgn(const POINT *lppt,const INT *lpPolyCounts,INT nCount,INT fnPolyFillMode){
      return (HRGN)Attach(::CreatePolyPolygonRgn(lppt,lpPolyCounts,nCount,fnPolyFillMode));
    }

    HRGN CreateRoundRectRgn(INT x,INT y,INT cx,INT cy,INT nW,INT nH){
      return (HRGN)Attach(::CreateRoundRectRgn(x,y,cx,cy,nW,nH));
    }

    HRGN CreateRoundRectRgnIndirect(LPRECT rc,LPSIZE sz){
      return (HRGN)Attach(::CreateRoundRectRgn(rc->left,rc->top,rc->right,rc->bottom,sz->cx,sz->cy));
    }

    HRGN CreateFromData(CONST XFORM *lpXform,DWORD nCount,CONST RGNDATA *lpRgnData){
      return (HRGN)Attach(::ExtCreateRegion(lpXform,nCount,lpRgnData));
    }

    HRGN CreateFromPath(HDC hdc){
      return (HRGN)Attach(::PathToRegion(hdc));
    }

    BOOL SetRectRgn(INT x,INT y,INT cx,INT cy){
      XASSERT(m_hObject!=NULL)
      return ::SetRectRgn((HRGN)m_hObject,x,y,cx,cy);
    }

    BOOL SetRectRgn(LPRECT rc){
      XASSERT(m_hObject!=NULL)
      return ::SetRectRgn((HRGN)m_hObject,rc->left,rc->top,rc->right,rc->bottom);
    }

    INT CombineRgn(HRGN nrgn,HRGN rgn,INT fnCombineMode){
      XASSERT(m_hObject!=NULL)
      return ::CombineRgn((HRGN)m_hObject,nrgn,rgn,fnCombineMode);
    }

    INT CopyRgn(const HRGN rgn){
      XASSERT(m_hObject!=NULL)
      return ::CombineRgn((HRGN)m_hObject,rgn,NULL,RGN_COPY);
    }

    BOOL EqualRgn(HRGN rgn)const{
      XASSERT(m_hObject!=NULL)
      return ::EqualRgn((HRGN)m_hObject,rgn);
    }

    INT OffsetRgn(INT nX,INT nY){
      XASSERT(m_hObject!=NULL)
      return ::OffsetRgn((HRGN)m_hObject,nX,nY);
    }

    INT OffsetRgn(LPPOINT pt){
      XASSERT(m_hObject!=NULL)
      return ::OffsetRgn((HRGN)m_hObject,pt->x,pt->y);
    }

    INT GetRgnBox(LPRECT lprc)const{
      XASSERT(m_hObject!=NULL)
      return ::GetRgnBox((HRGN)m_hObject,lprc);
    }

    BOOL PtInRegion(INT X,INT Y)const{
      XASSERT(m_hObject!=NULL)
      return ::PtInRegion((HRGN)m_hObject,X,Y);
    }

    BOOL PtInRegion(LPPOINT pP)const{
      XASSERT(m_hObject!=NULL)
      return ::PtInRegion((HRGN)m_hObject,pP->x,pP->y);
    }

    BOOL RectInRegion(CONST RECT *lprc)const{
      XASSERT(m_hObject!=NULL)
      return ::RectInRegion((HRGN)m_hObject,lprc);
    }

    DWORD GetRegionData(DWORD dwCount,LPRGNDATA lpRgnData)const{
      XASSERT(m_hObject!=NULL)
      return ::GetRegionData((HRGN)m_hObject,dwCount,lpRgnData);
    }

    XRgn &operator=(HRGN hRgn){
      m_hObject=hRgn;
      return *this;
    }

    operator HRGN() const {
      return (HRGN)m_hObject;
    }
};

#endif //_XGDI_H_
