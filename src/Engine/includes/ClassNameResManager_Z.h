#ifndef _CLASSNAMERESMANAGER_Z_H_
#define _CLASSNAMERESMANAGER_Z_H_
#include "HashTable_Z.h"
#include "HoleArray_Z.h"
#include "Handle_Z.h"

class ClassNameRes_Z {
public:
    Name_Z m_Name;
    Name_Z m_FileName;
    S32 m_UnkS32_0x8;
    HashS32Table_Z m_ResourceHT;

    ClassNameRes_Z() {
        m_UnkS32_0x8 = -1;
    }

    void Flush() {
        m_ResourceHT.Flush();
    }

    void Load(void** i_Data);
    void UpdateLinks();
    void MarkHandles();

    void Minimize() {
        m_ResourceHT.Minimize();
    }
};

class ClassNameResManager_Z {
    HashName_ZTable_Z m_ClassHT; // Enum?
    HoleArray_Z<ClassNameRes_Z> m_ClassResHA;

public:
    ClassNameResManager_Z()
        : m_ClassHT(64) {
    }

    virtual void Shut();
    virtual Bool MarkHandles();
    virtual Bool Minimize();

    void Load(const Name_Z& i_Name, const Name_Z& i_FileName, void** i_Data);
    void UpdateLinks(const Name_Z& i_Name, const Name_Z& i_FileName);
    Bool GetClassID(const Name_Z& i_Name, const Name_Z& i_FileName, S32& o_ClassID, Bool i_AddClass = TRUE);
    void MarkHandlesFromClass(const Name_Z& i_Name, const Name_Z& i_FileName);
    void RemoveClassId(const S32 i_ClassID);
    void RemoveClassId(const Name_Z& i_Name, const Name_Z& i_FileName);
    BaseObject_ZHdl GetObjectInClass(const S32 i_ClassID, const S32 i_EnumID);
};

#endif
