#ifndef _CLASSMANAGER_Z_H_
#define _CLASSMANAGER_Z_H_
#include "Handle_Z.h"
#include "File_Z.h"
#include "HashTable_Z.h"
#include "Types_Z.h"

#define REGISTER_CLASS(a, b, c) gData.ClassMgr->RegisterClass(a, b, c)
#define REGISTER_CLASS_TYPE(a, b, c) gData.ClassMgr->RegisterClassType(a, b, c)

class ObjectDatas_Z;
class ObjectDatas_ZHdl;
class ResourceObject_ZHdl;
class ResourceObject_Z;
class Node_Z;
class BaseObject_Z;
struct ClassDesc_Z;

typedef BaseObject_Z* (*NewObjectProc)();
typedef DynArray_Z<ClassDesc_Z, 16> Class_ZDA;
typedef DynArray_Z<BaseObject_ZHdl, 32> BaseObject_ZHdlDA;

Bool OpenBF();
Bool OpenBFS();
Bool CloseBF();
Bool ForceBF();
Bool CloneClass();

#define FL_CLASS_NONE (U8)(0 << 0)
#define FL_CLASS_STREAM_XRAM (U8)(1 << 0)

struct ClassDesc_Z {
public:
    ClassDesc_Z(const Name_Z& i_Name, const Name_Z& i_ParentClassName, NewObjectProc i_NewObjectProc, S16 i_ClonedClassId) {
        m_ClassName = i_Name;
        m_ParentClassName = i_ParentClassName;
        m_NewObject = i_NewObjectProc;
        m_Id = 0;
        m_Flag = FL_CLASS_NONE;
        m_ClonedClassId = i_ClonedClassId;
    }

public:
    Name_Z m_ClassName;
    Name_Z m_ParentClassName;
    NewObjectProc m_NewObject;
    U8 m_Flag;
    U8 m_Id; // $SABE: Incrementing counter used for system object names
    S16 m_ClonedClassId;
};

struct BigFileRscHeader_Z {
    U32 m_Size;
    U32 m_LinkSize;
    U32 m_DataSize;
    U32 m_CompressedSize;
    Name_Z m_ClassName;
    Name_Z m_Name;
};

struct BigFileRsc_Z {
    BigFileRscHeader_Z* m_Header;
    U8* m_Data;
    BaseObject_Z* m_Rsc;
    U8 m_Flag;

    BigFileRsc_Z() {
        m_Flag = 0;
        m_Data = NULL;
        m_Header = NULL;
        m_Rsc = NULL;
    }
};

class ClassManager_Z : public HandleManager_Z {
public:
    ClassManager_Z();

    virtual S32 CheckHandles();
    virtual void Minimize();
    virtual void ClearMark();
    virtual void InvalidClassSize(S16 a1) const;
    virtual const String_Z<ARRAY_CHAR_MAX>& GetNameStrFromId(const Name_Z& i_Name) const;
    virtual ~ClassManager_Z();
    virtual Bool OpenBigFile(const Char* i_BfName, const Name_Z& i_StreamAgentClassName, Bool i_Unused);
    virtual void CloseBigFile();
    virtual void OpenBigFileStream(const Char* a1);
    virtual void CloseBigFileStream();
    virtual void ShutBigFile();
    virtual void GetResourceObject(const Char* a1);
    virtual void AssumeGetResourceObject(const Char* a1);
    virtual void ExcludeFromSaving(const BaseObject_ZHdl& a1);

    void LoadLinkId(void* i_ID, void** i_Data);
    void UpdateLinkFromId(BaseObject_ZHdl& i_Hdl, S32 i_ID);
    void UpdateLink(BaseObject_ZHdl& i_Hdl);
    void LoadName(Name_Z&, void**);
    void LoadLink(BaseObject_ZHdl& i_Hdl, void** i_Data);
    void RegisterClass(const Char* i_ClassName, const Char* i_ParentClassName, NewObjectProc i_NewObject);
    void RegisterClassType(const Char* i_ClassName, U8 i_AddFlag, U8 i_RemoveFlag);
    Bool IsObjectInherit(const Name_Z& i_ClassName, const Name_Z& i_ParentClassName);
    const BaseObject_ZHdl& NewObject(const Name_Z& i_ClassName, const Name_Z& i_Name);
    const BaseObject_ZHdl& NewObject(const Name_Z& i_ClassName);
    const BaseObject_ZHdl& NewObject(const Char* i_ClassName);
    Name_Z& GetClassName(const BaseObject_ZHdl& i_Hdl);

    const BaseObject_ZHdl& NewResource(const Name_Z& i_ClassName, const Name_Z& i_Name);
    Bool GetFile(const Char* i_Path, File_Z& i_File);

    S16 GetClassIndex(const Name_Z& i_ClassName) {
        S32Hash_Z l_NameToId(i_ClassName.GetID());
        const S32Hash_Z* l_Result = m_ClassNameToIndex.Search(l_NameToId);
        S16 l_ClassId;
        if (!l_Result) {
            return -1;
        }
        else {
            return l_Result->m_Ref;
        }
    }

    // BigFile methods

    void OpenBigFileName(const String_Z<ARRAY_CHAR_MAX>& i_NameFilePath);

    Bool IsBigFileOpened() const { return m_BfOpened; }

    Bool IsBigFileRead() const { return m_BfRead; }

    Bool IsBigFileWrite() const { return m_BfCreate; }

    void SetBigFileOpened(Bool i_Opened) {
        m_BfOpened = i_Opened;
    }

    void InitBigFile(Bool i_Read) {
        m_BfRead = i_Read;
        m_BfCreate = !m_BfRead;
    }

    Bool LoadResource(BigFileRsc_Z& i_Resource);

    const Char* GetBigFileHeaderText() const { return m_BfHeaderText; }

private:
    // $SABE: Keeping this private to avoid the dumb issue where Name_Z::GetID gets downcasted to S16 instead of it being a Name_Z
    const BaseObject_ZHdl& NewObject(S16 i_ClassId, const Name_Z& i_Name);

    Class_ZDA m_ClassList;
    HashTableBase_Z<S32Hash_Z> m_ClassNameToIndex;
    S32 m_UnkS32_0x16c;
    String_Z<ARRAY_CHAR_MAX> m_BfExtension;
    String_Z<ARRAY_CHAR_MAX> m_BfNameExtension;
    String_Z<ARRAY_CHAR_MAX> m_BfStreamExtension;
    String_Z<ARRAY_CHAR_MAX> m_BfPathRead;
    String_Z<ARRAY_CHAR_MAX> m_BfPathWrite;
    Bool m_BfCreate;
    Bool m_BfRead;
    Bool m_BfOpened;
    HashTableBase_Z<S32Hash_Z> m_ObjectNameToStringIndex;
    DynArray_Z<String_Z<ARRAY_CHAR_MAX>, 16> m_ObjectNameStrings;
    String_Z<ARRAY_CHAR_MAX> m_BfHeaderText; // Includes version
    String_Z<ARRAY_CHAR_MAX> m_BfVersion;    // Just version
};

// Load defines

#define LOADLINK_Z(dst) gData.ClassMgr->LoadLink(dst, i_Data)
#define LOADLINKID_Z(dst) gData.ClassMgr->LoadLinkId(dst, i_Data)

#endif
