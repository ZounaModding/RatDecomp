#ifndef _CAMERA_Z_H_
#define _CAMERA_Z_H_
#include "Object_Z.h"

struct Frustrum_Z {
};

class Camera_Z : public Object_Z {
public:
    void UpdateInverseWorldMatrix(Node_Z* i_Node);
    virtual ~Camera_Z();                                      /* 0x08 */
    virtual void Load(void** a1);                             /* 0x0C */
    virtual void EndLoad();                                   /* 0x10 */
    virtual void Draw(DrawInfo_Z& a1, ObjectDatas_Z* a2);     /* 0x14 */
    virtual void UpdateObject(Node_Z* a1, ObjectDatas_Z* a2); /* 0x18 */
};

#endif
