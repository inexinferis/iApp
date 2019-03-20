


#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "x/xregkey.h"
#include "utils.h"

class AppConfig{
  public:
    AppConfig();
    ~AppConfig();

    BOOL Load();
    BOOL Save();
    BOOL SaveToolsCfg();

    BOOL GetProgData();
    BOOL SaveProgData();

};

extern const TCHAR *g_RegStrMap[];

#endif //_REGISTER_H_
