#ifndef _XCOORD_H_
#define _XCOORD_H_

#include "xobject.h"

#define RECTWIDTH(rc)    ((rc).right-(rc).left)
#define RECTHEIGHT(rc)   ((rc).bottom-(rc).top)

class XSize:public tagSIZE{
  public:
    XSize();
    XSize(LONG x,LONG y);
    XSize(const SIZE &size);
    XSize(DWORD dwSize);
	  XSize &SetSize(LONG nx,LONG ny);
    XSize &Offset(SIZE size);
    XSize &Offset(INT nx,INT ny);
    BOOL operator==(const SIZE &sZ);
    BOOL operator!=(const SIZE &sZ);
    XSize &operator-=(const SIZE &sZ);
    XSize &operator+=(const SIZE &sZ);
    XSize operator-(const POINT &pP);
    XSize operator+(const POINT &pP);
    XSize operator-(const SIZE &sZ);
    XSize operator+(const SIZE &sZ);
    XSize operator-(LONG offset);
    XSize operator+(LONG offset);
    XSize operator-();
    XSize &operator=(const SIZE &sZ);
    XSize &operator=(const RECT &rc);
    operator LPSIZE();
    operator const SIZE*()const;

  #ifdef _DEBUG
    VOID Dump();
  #endif
};

typedef class XSize XSIZE,*PXSIZE;

class XPoint:public tagPOINT{
  public:
    XPoint();
    XPoint(LONG nx,LONG ny);
    XPoint(const POINT &point);
    XPoint(DWORD dwPoint);
    XPoint &SetPoint(LONG nx,LONG ny);
    XPoint &Offset(POINT point);
    XPoint &Offset(INT nx,INT ny);
    BOOL operator==(const POINT &pP);
    BOOL operator!=(const POINT &pP);
    XPoint &operator-=(const POINT &pP);
    XPoint &operator+=(const POINT &pP);
    XPoint operator-(const POINT &pP);
    XPoint operator+(const POINT &pP);
    XPoint operator-(const SIZE &sZ);
    XPoint operator+(const SIZE &sZ);
    XPoint operator-(LONG offset);
    XPoint operator+(LONG offset);
    XPoint operator-();
    XPoint &operator=(const POINT &initPt);
    XPoint &operator=(const RECT &rc);
    operator LPPOINT();
    operator const POINT*()const;

  #ifdef _DEBUG
    VOID Dump();
  #endif
};

typedef class XPoint XPOINT,*PXPOINT;

class XRect:public tagRECT{
  public:
    XRect();
    XRect(LONG nx,LONG ny);
    XRect(LONG x,LONG y,LONG cx,LONG cy);
    XRect(const RECT &pP);
    XRect(const RECT *pP);
    XRect(const POINT &pPt,const SIZE &pSz);
    XRect(const POINT &pPt,const POINT &pSz);
    XRect &SetTopLeft(LONG x,LONG y);
    XRect &SetTopLeft(const PPOINT pP);
    XRect &SetBottomRight(LONG cx,LONG cy);
    XRect &SetBottomRight(const PPOINT pP);
    XRect &SetSize(const PSIZE nSz);
    BOOL SetRect(LONG x,LONG cx,LONG y,LONG cy);
    XRect &SetRect(const PPOINT pPt,const PSIZE pSz);
    XRect &SetRect(const PPOINT pPt,const PPOINT pSz);
    BOOL CopyRect(const RECT &pP);
    VOID SetRectEmpty();
    LONG Width();
    LONG Height();
    LONG GetXProm();
    LONG GetYProm();
    SIZE Size();
    POINT TopLeft();
    POINT BottomRight();
    POINT CenterPoint();
    VOID SwapLeftRight();
    VOID SwapLeftRight(LPRECT lpRect);
    BOOL IsRectEmpty();
    BOOL IsRectNull();
    BOOL EqualRect(LPCRECT rect);
    BOOL InflateRect(INT cx,INT cy);
    BOOL InflateRect(const SIZE &pSz);
    XRect &InflateRect(LPCRECT lpRect);
    XRect &InflateRect(INT l, INT t, INT r, INT b);
    BOOL DeflateRect(INT cx,INT cy);
    BOOL DeflateRect(const SIZE &pSz);
    XRect &DeflateRect(LPCRECT lpRect);
    XRect &DeflateRect(INT l, INT t, INT r, INT b);
  	XRect &ResizeRect(INT r, INT b);
  	XRect &ResizeRect(INT l, INT t, INT r, INT b);
    BOOL OffsetRect(INT cx,INT cy);
    BOOL OffsetRect(POINT &pPT);
    BOOL OffsetRect(SIZE &pSz);
    VOID MoveToX(INT x);
    VOID MoveToY(INT y);
    VOID MoveToXY(INT x,INT y);
    VOID MoveToXY(POINT pt);
    BOOL UnionRect(LPRECT rect1,LPRECT rect2);
    BOOL IntersectRect(LPRECT rect1,LPRECT rect2);
    BOOL SubtractRect(LPRECT rect1,LPRECT rect2);
    BOOL PtInRect(POINT opt);
    BOOL RectInRect(LPRECT orc);
    BOOL NormalizeRect();
    XRect MulDiv(INT nMultiplier,INT nDivisor)const;
    XRect &operator=(const RECT &rc);
    BOOL operator==(const RECT &pP);
    BOOL operator!=(const RECT &pP);
    XRect operator&(RECT &pP);
    XRect &operator&=(RECT &pP);
    XRect operator|(RECT &pP);
    XRect &operator|=(RECT &pP);
    XRect operator+(POINT &pt);
    XRect operator-(POINT pt);
    XRect operator+(SIZE &size);
    XRect operator-(SIZE size);
    XRect operator+(LPCRECT lpRect);
    XRect operator-(LPCRECT lpRect);
    XRect &operator+=(POINT &point);
    XRect &operator+=(SIZE &size);
    XRect &operator+=(LPCRECT lpRect);
    XRect &operator-=(POINT point);
    XRect &operator-=(SIZE size);
    XRect &operator-=(LPCRECT lpRect);
    operator LPCRECT()const;
    operator LPRECT();

  #ifdef _DEBUG
    VOID Dump();
  #endif
};

typedef class XRect XRECT,*PXRECT;

#include "xcoords.cpp"

#endif //_XCOORD_H_
