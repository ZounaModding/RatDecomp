#ifndef _SCRIPTINPUTMANAGER_Z_H_
#define _SCRIPTINPUTMANAGER_Z_H_

#include "InputEngine_Z.h"

class ScriptInputManager_Z : public InputEngine_Z {
public:
    virtual ~ScriptInputManager_Z() { }
    virtual void Update(Float i_DeltaTime);
};

#endif