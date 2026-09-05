#ifndef _SURFACETREE_Z_H_
#define _SURFACETREE_Z_H_
#include "SystemObject_Z.h"
#include "StaticArray_Z.h"

#define ELE1LEAF 1
#define ELE2LEAF 2
#define ELE1EMPTY 4
#define ELE2EMPTY 8

struct SurfaceTreeEle {
    Sphere_Z m_BSphere;
    S32 m_Flag;
    S16 m_Off1;
    S16 m_Off2;

    SurfaceTreeEle() {
        m_Flag = 0;
    }
};

typedef DynArray_Z<SurfaceTreeEle, 32, FALSE> SurfaceTreeEleDA;

struct TreeStackEle {
    U16 m_Ele;
    U16 m_Count;

    TreeStackEle() { }

    TreeStackEle(U16 i_Ele) {
        m_Ele = i_Ele;
        m_Count = 0;
    }
};

#define RECURSE_TREE(Stack, Sph, Test, Ret, Tree, TabSph) \
    Ret = -1;                                             \
    while (Stack.GetSize()) {                             \
        TreeStackEle& TSE = Stack[Stack.GetSize() - 1];   \
        SurfaceTreeEle& STE = Tree[TSE.m_Ele];            \
        Sph = STE.m_BSphere;                              \
        if (Test) {                                       \
            if (TSE.m_Count == 0) {                       \
                TSE.m_Count++;                            \
                if (STE.m_Flag & ELE1LEAF) {              \
                    Sph = TabSph[STE.m_Off1].m_Sphere;    \
                    if (Test) {                           \
                        Ret = STE.m_Off1;                 \
                        break;                            \
                    }                                     \
                }                                         \
                else if (!(STE.m_Flag & ELE1EMPTY)) {     \
                    Stack.Add(TreeStackEle(STE.m_Off1));  \
                }                                         \
            }                                             \
            else if (TSE.m_Count == 1) {                  \
                TSE.m_Count++;                            \
                if (STE.m_Flag & ELE2LEAF) {              \
                    Sph = TabSph[STE.m_Off2].m_Sphere;    \
                    if (Test) {                           \
                        Ret = STE.m_Off2;                 \
                        break;                            \
                    }                                     \
                }                                         \
                else if (!(STE.m_Flag & ELE2EMPTY)) {     \
                    Stack.Add(TreeStackEle(STE.m_Off2));  \
                }                                         \
            }                                             \
            else {                                        \
                Stack.SetSize(Stack.GetSize() - 1);       \
            }                                             \
        }                                                 \
        else {                                            \
            Stack.SetSize(Stack.GetSize() - 1);           \
        }                                                 \
    }

#endif // _SURFACETREE_Z_H_
