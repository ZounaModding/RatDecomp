#ifndef _HANDLE_Z_H_
#define _HANDLE_Z_H_
#include "String_Z.h"
#include "Types_Z.h"
#include "Name_Z.h"
#include "DynArray_Z.h"
#include "DynPtrArray_Z.h"
#include "HashTable_Z.h"
#include "Streaming_Z.h"
#include "BnkLinkArray_Z.h"

#define HANDLEREC_GRANULARITY 16384
#define HANDLE_NULL BaseObject_ZHdl()

#define HANDLE_MARKED_FALSE 0
#define HANDLE_MARKED_TRUE 1
#define HANDLE_MARKED_UNK 2

#define HDL_STR_LOAD_STAGE_ABORT -1
#define HDL_STR_LOAD_STAGE_INIT 1
#define HDL_STR_LOAD_STAGE_READ_HEADER 2
#define HDL_STR_LOAD_STAGE_PARSE_HEADER 3
#define HDL_STR_LOAD_STAGE_READ_TOC 4
#define HDL_STR_LOAD_STAGE_READ_BLOCKS 5
#define HDL_STR_LOAD_STAGE_READ_REQUESTED_BLOCKS 39
#define HDL_STR_LOAD_STAGE_LOAD_RESOURCES 6
#define HDL_STR_LOAD_STAGE_LOAD_REQUESTED_RESOURCES 40
#define HDL_STR_LOAD_STAGE_ENDLOAD 7
#define HDL_STR_LOAD_STAGE_AFTERENDLOAD 8
#define HDL_STR_LOAD_STAGE_LOADDONE 9
#define HDL_STR_LOAD_STAGE_FINISHED 10

class BaseObject_Z;
class BaseObject_ZHdl;
class HandleManager_Z;
class DrawInfo_Z;
class GCGlobals;
Extern_Z GCGlobals gData;

#define GETPTR(h) gData.ClassMgr->GetPtr(h)

#ifndef BUGFIXES_Z
// $SABE: Original version, does the dumb thing where each class in the chain sets GblID to 0
#define HANDLE_Z(ClassName, ParentName)                          \
    class ClassName##Hdl;                                        \
    class ClassName##Hdl : public ParentName##Hdl {              \
    private:                                                     \
    public:                                                      \
        ClassName##Hdl(void) {                                   \
            m_RealID.GblID = 0;                                  \
        }                                                        \
        ClassName##Hdl(const ClassName##Hdl& i_Org) {            \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
        }                                                        \
        ClassName##Hdl(const BaseObject_ZHdl& i_Org) {           \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
        }                                                        \
        ClassName##Hdl& operator=(const ClassName##Hdl& i_Org) { \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
            return *this;                                        \
        }                                                        \
        ClassName* operator->() const {                          \
            return (ClassName*)GETPTR((BaseObject_ZHdl&)*this);  \
        }                                                        \
        operator ClassName*() const {                            \
            return (ClassName*)GETPTR((BaseObject_ZHdl&)*this);  \
        }                                                        \
    };
#else
// $SABE: Bugfix version, no need to set GblID to 0 in each class, it's already done in the BaseObject_ZHdl constructor
#define HANDLE_Z(ClassName, ParentName)                          \
    class ClassName##Hdl;                                        \
    class ClassName##Hdl : public ParentName##Hdl {              \
    private:                                                     \
    public:                                                      \
        ClassName##Hdl(void) { }                                 \
        ClassName##Hdl(const ClassName##Hdl& i_Org) {            \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
        }                                                        \
        ClassName##Hdl(const BaseObject_ZHdl& i_Org) {           \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
        }                                                        \
        ClassName##Hdl& operator=(const ClassName##Hdl& i_Org) { \
            m_RealID.GblID = i_Org.m_RealID.GblID;               \
            return *this;                                        \
        }                                                        \
        ClassName* operator->() const {                          \
            return (ClassName*)GETPTR((BaseObject_ZHdl&)*this);  \
        }                                                        \
        operator ClassName*() const {                            \
            return (ClassName*)GETPTR((BaseObject_ZHdl&)*this);  \
        }                                                        \
    };
#endif // #ifndef BUGFIXES_Z

// ClassName##Hdl(int i_Val) {
//     m_RealID.GblID = i_Val;
// }

union HdlID {
    int GblID;

    struct {
        int Key : 8,
            ID : 24;
    } Ref;
};

class BaseObject_ZHdl {
    friend class HandleManager_Z;
    friend class BaseObject_Z;

public:
    BaseObject_ZHdl() {
        m_RealID.GblID = 0;
    }

    BaseObject_ZHdl(const int i_Val) {
        m_RealID.Ref.ID = i_Val;
        m_RealID.Ref.Key = (char)i_Val;
    }

    BaseObject_ZHdl(const BaseObject_ZHdl& i_Org) {
        m_RealID.GblID = i_Org.m_RealID.GblID;
    }

    int GetID() const {
        return m_RealID.Ref.ID;
    }

    char GetKey() const {
        return m_RealID.Ref.Key;
    }

    int GetGlobalID() const {
        return m_RealID.GblID;
    }

    Bool IsValid() const {
        BaseObject_Z* l_Ptr = *this;
        return l_Ptr != NULL;
    }

    Bool operator==(const BaseObject_ZHdl& i_Other) const {
        return m_RealID.GblID == i_Other.m_RealID.GblID;
    }

    inline BaseObject_Z* operator->() const;
    operator BaseObject_Z*() const;

    operator Bool() const {
        return IsValid();
    }

    HdlID m_RealID;
};

#define MarkValidHandle_Z(i_Handle)          \
    do {                                      \
        if (i_Handle.IsValid()) {             \
            GETPTR(i_Handle)->MarkHandles(); \
        }                                     \
    } while (0)

#define MarkHandle_Z(i_Handle)               \
    do {                                      \
        GETPTR(i_Handle)->MarkHandles(); \
    } while (0)

struct HandleRec_Z {
    enum {
        NONE = 0,
        DELETING = 1,
        RSC_XRAM = 2,
        RSC_XRAM_LOADED = 4,
        RSC = 8,
        RSC_STR_DONE = 16,
        RSC_STR_LOADING = 32,
        RSC_STR = 64,
        KEEP = (U8) ~(DELETING),
    };

    HandleRec_Z() {
        m_ObjPtr = NULL;
        m_Key = 1;
        m_Flag = 0;
    }

    S8 m_Key;
    S8 m_Flag;
    S8 m_Marked;
    S8 m_LastKeyBeforeAsyncDelete;
    BaseObject_Z* m_ObjPtr;
    Name_Z m_Name;
    S16 m_ClassID;
    S16 m_xRamBlock;
};

struct StrFileToc_Z {
    U32 m_Flag;
    Name_Z m_Name;
    S32 m_DependenciesIdx;
    S32 m_BlockStartIdx;
    S32 m_BlockEndIdx;
};

struct StrFileHeader_Z {
    Char m_HeaderText[256];
    S32 m_TableOfContentsCount;
    S32 m_DependencyCount;
    S32 m_TocAndDependenciesBlockCount;
    S32 m_DataBlockStart;
    S32 m_DataBlockEnd;
    U32 m_Unk_0x114;
    U32 m_Unk_0x118;
    U32 m_Unk_0x11c;
    U32 m_WorkingBufferSize[2];
    U8 m_Pad_0x128[1752];
} Aligned_Z(128);

struct RscQueue_Z {
    S32 m_TocRscIndex;
    S32 m_BlockStartIdx;
    S32 m_BlockEndIdx;
    RscQueue_Z* m_Next;
    RscQueue_Z* m_Prev;
};

struct RscOrder_Z {
    Float m_Priority;
    RscOrder_Z* m_Next;
    RscOrder_Z* m_Prev;
    RscQueue_Z* m_QueuedRsc;
};

struct StrWorkingBuffer_Z {
    U8* m_Data;
    S32 m_BlockStartIdx;
    S32 m_BlockEndIdx;
    RscOrder_Z* m_CurRscOrder;
};

class HandleStream_Z {
public:
    HandleStream_Z() {
        m_WorkingBufferLastOperationCount = 0;
        m_StreamStage = 0;
    }

    void Open(const Char* i_FileName);
    void Draw(DrawInfo_Z& i_DrawInfo);
    void Update(Float i_DeltaTime);

    void Queue(S32 i_TocIdx, Float i_Priority, Bool i_QueueDependencies);
    void Remove(RscOrder_Z* i_First, RscOrder_Z* i_End);
    void QueueRsc(RscQueue_Z** io_Queue, S32 i_TocRscIndex);
    RscOrder_Z* QueueZ(Float i_Priority);
    Bool Read();
    Bool GetHeader();

private:
    DynArray_Z<StrFileToc_Z, 1, FALSE, FALSE> m_TableOfContentsDA;
    S32DA m_DependencyDA;
    S32 m_StreamStage;
    S32 m_Unk_0x14;
    U32 m_CurBuffer;
    StreamX_Z m_Str;
    StrWorkingBuffer_Z m_WorkingBuffers[2];
    BnkLinkArray_Z<RscQueue_Z> m_RscQueueBank;
    RscOrder_Z* m_HeadRscOrder;
    BnkLinkArray_Z<RscOrder_Z> m_RscOrderBank;
    String_Z<ARRAY_CHAR_MAX> m_StrFileName;
    DynPtrArray_Z<BaseObject_Z*, 32> m_Resources;
    S32 m_PendingResources;
    U32 m_Unk_0x180;
    U32 m_Unk_0x184;
    U32 m_Unk_0x188;
    U32 m_Unk_0x18c;
    StrWorkingBuffer_Z m_WorkingBufferLastOperations[10];
    S32 m_WorkingBufferLastOperationCount;

    static StrFileHeader_Z gStrFileHeader;
} Aligned_Z(16);

class HandleManager_Z {
    friend class HandleStream_Z;

public:
    static HandleStream_Z HandleStream;

protected:
    DynArray_Z<HandleRec_Z, HANDLEREC_GRANULARITY> m_HandleRecDA;
    DynArray_Z<S32, HANDLEREC_GRANULARITY> m_FreeRecDA;
    HashS32Table_Z m_HandleIdHT;
    U32 m_HandleRecDASize;
    S32 m_NbFree;
    Name_Z m_NullName;
    BaseObject_ZHdl m_NullHandle;

public:
    Bool m_UnkBool_SetsDeltaTimeTo30fps_0x34;

protected:
    Bool m_DoAsynchCheckHandles;
    Bool m_ForbidCheckHandles;
    Bool m_CheckHandlesQueued;
    S32 m_NextHandleToDelete;
    S32 m_NextManagerToMarkHandles;
    U32 m_LastDeleteFrameNb;
    S32 m_UnkS32_0x44; // $SABE: Gets tested in HandleManager_Z::MarkHandle but never passes
    Float m_FramesSpentDeleting;

public:
    HandleManager_Z();

    virtual S32 CheckHandles();
    virtual S32 MarkHandles(S32 i_Manager);
    virtual void Update(Float i_DeltaTime);
    virtual void Draw(DrawInfo_Z& i_DrawInfo);
    virtual void Minimize();
    virtual void ClearMark();

    virtual void InvalidClassSize(S16 const a1) const { }

    virtual void RemoveResource(const BaseObject_ZHdl& a1);
    virtual S32 ChangeHandleName(const BaseObject_ZHdl& i_Hdl, const Name_Z& i_Name);
    virtual void DeleteHandle(const BaseObject_ZHdl& a1);

    virtual const String_Z<ARRAY_CHAR_MAX>& GetNameStrFromId(const Name_Z& i_Name) const {
        return m_DefaultNameString;
    }

    BaseObject_Z* GetPtr(const BaseObject_ZHdl& i_Hdl) const;
    BaseObject_Z* GetPtrXRam(const HandleRec_Z& i_HandleRec) const;
    Bool RemovedXRamResource(S32 i_Handle, U8 i_Flag);
    BaseObject_ZHdl U32ToHandle(S32 i_Value);
    S32 HandleToU32(const BaseObject_ZHdl& i_Hdl);
    void MarkU32Handle(U32 i_Hdl);
    Bool MarkHandle(const BaseObject_ZHdl&);
    void MarkHandles(const BaseObject_ZHdl&);
    U8 IsMarked(const BaseObject_ZHdl& i_Hdl);
    Bool AsynchCheckHandles();
    Bool IsAsynchDelHandle(const BaseObject_ZHdl& i_Hdl) const;
    void ForbidCheckHandles(Bool i_ForbidCheckHandles);
    const BaseObject_ZHdl& CreateNewHandle(BaseObject_Z* i_BObj, const Name_Z& i_Name, S16 i_ClassID, U8 i_Flag = HandleRec_Z::NONE);
    void ExpandSize(S32 i_NewSize = HANDLEREC_GRANULARITY);
    void AddResourceRef(const HandleRec_Z& i_HandleRec, S32 i_Index);
    void RemoveResourceRef(const HandleRec_Z& i_HandleRec);
    S32 IsResourceRef(S32 i_Hdl);

private:
    String_Z<ARRAY_CHAR_MAX> m_DefaultNameString;

public:
    inline Name_Z& GetHandleName(const BaseObject_ZHdl& i_Hdl) {
        S32 l_ID = i_Hdl.GetID();
        if (CheckKey(l_ID, i_Hdl.GetKey())) {
            return m_HandleRecDA[l_ID].m_Name;
        }

        return m_NullName;
    }

    inline Bool CheckKey(int i_ID, int i_Key) const {
        return (i_ID < m_HandleRecDA.GetSize() && m_HandleRecDA[i_ID].m_Key == i_Key);
    }

    inline Bool GetSetDeltaTimeTo30fps() {
        return m_UnkBool_SetsDeltaTimeTo30fps_0x34;
    }

    inline void SetSetDeltaTimeTo30fps(Bool i_Value) {
        m_UnkBool_SetsDeltaTimeTo30fps_0x34 = i_Value;
    }
};

#endif
