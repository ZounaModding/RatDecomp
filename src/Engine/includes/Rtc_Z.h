#ifndef _RTC_Z_H_
#define _RTC_Z_H_
#include "ResourceObject_Z.h"

class Rtc_Z : public ResourceObject_Z {
public:
    virtual ~Rtc_Z();
    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();

private:
    U8 m_Pad_0x14[0x4c];
};

#endif // _RTC_Z_H_
