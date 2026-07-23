#ifndef _TREE_Z_H_
#define _TREE_Z_H_
#include "Object_Z.h"

class Tree_Z : public Object_Z {
public:
    virtual ~Tree_Z() { }

    virtual void Load(void** i_Data);
    virtual void EndLoad();
    virtual void AfterEndLoad();
    virtual Bool MarkHandles();

    static BaseObject_Z* NewObject() {
        return NewL_Z(18) Tree_Z;
    }
};

#endif // _TREE_Z_H_
