#ifndef _ENEMYGENERATOR_G_H_
#define _ENEMYGENERATOR_G_H_
#include "ObjectGame_Z.h"
#include "Node_ZHdl.h"

class DummyGen;
class Node_Z;

class EnemyGenerator_G : public ObjectGame_Z {
public:
    void StreamRemoving(const Node_ZHdl& i_NodeHdl);
    void ParseHierarchy(Node_Z* i_Node, DummyGen* i_DummyGen);

private:
    U8 m_Unk_0x2c[0x14];
};
#endif // _ENEMYGENERATOR_G_H_
