#include "ClassManager_Z.h"
#include "Console_Z.h"
#include "Program_Z.h"
#include "Platform_Z.h"

#define BIGFILE_STREAM_EXTENSION ".STR"

String_Z<ARRAY_CHAR_MAX> DefaultStringClassName;

ClassManager_Z::ClassManager_Z() {
    m_UnkS32_0x16c = -1;
    REGISTERCOMMANDC("OpenBF", OpenBF, " [BigFileName] [StreamAgentInherit=Default StreamAgent_Z]");
    REGISTERCOMMANDC("OpenBFS", OpenBFS, " [BigFileName]");
    REGISTERCOMMAND("ForceBF", ForceBF);
    REGISTERCOMMAND("CloseBF", CloseBF);
    REGISTERCOMMANDC("CloneClass", CloneClass, " ClonedClassName ClassName");
    m_BfPathWrite.Empty();
    m_BfPathRead.Empty();
    m_BfCreate = FALSE;
    m_BfRead = FALSE;
    m_BfOpened = FALSE;
    m_BfExtension.StrCpy(BIGFILE_EXTENSION);
    m_BfNameExtension.StrCpy(BIGFILE_NAME_EXTENSION);
    m_BfStreamExtension.StrCpy(BIGFILE_STREAM_EXTENSION);
    m_BfVersion.Sprintf(BIGFILE_VERSION_STRING, User_BFVersion, BIGFILE_PATCH_VERSION, BIGFILE_PLATFORM_VERSION);
    m_BfHeaderText.StrCpy(m_BfVersion);
    m_BfHeaderText.StrCat(BIGFILE_VERSION_COPYRIGHT);
}

ClassManager_Z::~ClassManager_Z() {
}

S32 ClassManager_Z::CheckHandles() {
    return 0;
}

void ClassManager_Z::Minimize() {
}

void ClassManager_Z::ClearMark() {
}

void ClassManager_Z::RegisterClass(const Char* i_ClassName, const Char* i_ParentClassName, NewObjectProc i_NewObject) {
    Name_Z l_ClassName(i_ClassName);
    Name_Z l_ParentClassName(i_ParentClassName);
    S32 l_ClassId = m_ClassList.GetSize();

    S32Hash_Z l_NameToId(l_ClassName.GetID(), l_ClassId);
    m_ClassNameToIndex.Insert(l_NameToId);

    ClassDesc_Z l_ClassDesc(l_ClassName, l_ParentClassName, i_NewObject, -1);
    m_ClassList.Add(l_ClassDesc);
}

void ClassManager_Z::RegisterClassType(const Char* i_ClassName, U8 i_AddFlag, U8 i_RemoveFlag) {
    Name_Z l_ClassName(i_ClassName);
    S32Hash_Z l_NameToId(l_ClassName.GetID());
    const S32Hash_Z* l_Result = m_ClassNameToIndex.Search(l_NameToId);

    ASSERTLE_Z(l_Result, "", 151, "pResult");

    m_ClassList[l_Result->m_Ref].m_Flag |= i_AddFlag;
    m_ClassList[l_Result->m_Ref].m_Flag &= ~i_RemoveFlag;
}

Bool ClassManager_Z::IsObjectInherit(const Name_Z& i_ClassName, const Name_Z& i_ParentClassName) {
    S32 l_ClassId = GetClassIndex(Name_Z(i_ClassName));
    while (l_ClassId > 0) {
        if (m_ClassList[l_ClassId].m_ParentClassName == i_ParentClassName) {
            return TRUE;
        }
        l_ClassId = GetClassIndex(m_ClassList[l_ClassId].m_ParentClassName);
    }
    return FALSE;
}

Name_Z& ClassManager_Z::GetClassName(const BaseObject_ZHdl& i_Hdl) {
}

const BaseObject_ZHdl& ClassManager_Z::NewObject(const Name_Z& i_ClassName, const Name_Z& i_Name) {
    S16 l_ClassId = GetClassIndex(i_ClassName);
    if (l_ClassId < 0) {
#ifdef DEBUG_Z
        Report_Z("ClassManager: NewObject of unregistered class %08lx\n", i_ClassName.GetID());
#endif
        return m_NullHandle;
    }
    return NewObject(l_ClassId, i_Name);
}

const BaseObject_ZHdl& ClassManager_Z::NewObject(const Name_Z& i_ClassName) {
    S16 l_ClassId = GetClassIndex(i_ClassName);
    if (l_ClassId < 0) {
#ifdef DEBUG_Z
        Report_Z("ClassManager: NewObject of unregistered class %08lx\n", i_ClassName.GetID());
#endif
        return m_NullHandle;
    }
    String_Z<ARRAY_CHAR_MAX> l_ObjectNameString;
    fsprintfID(l_ObjectNameString, l_ClassId, m_ClassList[l_ClassId].m_Id);
    Name_Z l_ObjectName(l_ObjectNameString);
    return NewObject(l_ClassId, l_ObjectName);
}

const BaseObject_ZHdl& ClassManager_Z::NewObject(const Char* i_ClassName) {
    S16 l_ClassId = GetClassIndex(i_ClassName);
    if (l_ClassId < 0) {
#ifdef DEBUG_Z
        Report_Z("ClassManager: NewObject of unregistered class \"%s\"\n", i_ClassName);
#endif
        return m_NullHandle;
    }
    String_Z<ARRAY_CHAR_MAX> l_ObjectNameString;
    fsprintfID(l_ObjectNameString, l_ClassId, m_ClassList[l_ClassId].m_Id);
    m_ClassList[l_ClassId].m_Id++;
    Name_Z l_ObjectName(l_ObjectNameString);
    return NewObject(l_ClassId, l_ObjectName);
}

const BaseObject_ZHdl& ClassManager_Z::NewObject(S16 i_ClassId, const Name_Z& i_Name) {
    m_ClassList[i_ClassId].m_Id++;
    return CreateNewHandle(m_ClassList[i_ClassId].m_NewObject(), i_Name, i_ClassId);
}

const BaseObject_ZHdl& ClassManager_Z::NewResource(const Name_Z& i_ClassName, const Name_Z& i_Name) {
    S16 l_ClassId = GetClassIndex(i_ClassName);
#ifdef BUGFIXES_Z
    if (l_ClassId < 0) {
#ifdef DEBUG_Z
        Report_Z("ClassManager: NewResource of unregistered class %08lx\n", i_ClassName.GetID());
#endif
        return m_NullHandle;
    }
#endif
    return CreateNewHandle(m_ClassList[l_ClassId].m_NewObject(), i_Name, l_ClassId, HandleRec_Z::RSC);
}

void ClassManager_Z::InvalidClassSize(S16 a1) const {
}

Bool ClassManager_Z::GetFile(const Char* i_Path, File_Z& i_File) {
    return FALSE;
}

Bool CheckHandles() {
    return FALSE;
}

Bool AsynchCheckHandles() {
    return FALSE;
}

Bool CloneClass() {
    return FALSE;
}
