#ifndef _ERRORLANGUAGE_Z_H_
#define _ERRORLANGUAGE_Z_H_
#include "Types_Z.h"
#include "Language_Z.h"

enum SystemHdlError {
    error_none = 0,
    error_unknown = 1,
    error_no_disk = 2,
    error_wrong_disk = 3,
    error_retry_occur = 4,
    error_tray = 5,
    error_fatal = 6,
    error_mgr_init = 8,
    type_error_count = 14,
};

extern const Char* ErrorTabLang[LANG_COUNT][type_error_count];

void InitTabError();
const Char* GetStringTabError(LanguageEnum_Z i_Enum, SystemHdlError i_Error);

#endif // _ERRORLANGUAGE_Z_H_
