#ifndef MSL_STRING_H_
#define MSL_STRING_H_

#include <cstring.h>

namespace std {
using ::memchr;
using ::memcmp;
using ::memcpy;
using ::memmove;
using ::memset;

using ::strcat;
using ::strcmp;
using ::strcpy;
using ::strlen;
using ::strncmp;
using ::strncpy;
using ::strpbrk;
using ::strrchr;

using ::stricmp;
using ::strnicmp;
using ::wcslen;

inline char* strchr(char* str, int c) {
    return ::strchr(str, c);
}

inline char* strrchr(char* str, int c) {
    return ::strrchr(str, c);
}

inline char* strstr(char* str, const char* substr) {
    return ::strstr(str, substr);
}

}; // namespace std

#endif
