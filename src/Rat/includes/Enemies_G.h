#ifndef _ENEMIES_G_H_
#define _ENEMIES_G_H_
#include "Friends_G.h"
// clang-format off

BEGIN_AGENT_CLASS(Enemies_G, Friends_G, 20)
public:
    Enemies_G();

    virtual ~Enemies_G() {}
    virtual void Init();

    DECL_BHV(CheckEvent);
private:
END_AGENT_CLASS

// clang-format on
#endif // _ENEMIES_G_H_
