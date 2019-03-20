#include "xlinkctrl.h"

#if (_WIN32_WINNT >= 0x0501)

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XLinkCtrl,XWnd)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XLinkCtrl,XWnd)

#endif
