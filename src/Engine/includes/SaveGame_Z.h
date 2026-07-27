#ifndef _SAVEGAME_Z_H_
#define _SAVEGAME_Z_H_
#include "Types_Z.h"

class ABC_Agent;

enum MCCOUNTRY {
    MCCOUNTRY_JAPAN,
    MCCOUNTRY_EUROPE,
    MCCOUNTRY_USA,
    MCCOUNTRY_KOREA
};

struct SaveDate_Z {
};

struct FileInfo_Z {
};

class SaveGame_Z {
protected:
    S32 m_Type;

public:
    SaveGame_Z() {
        m_Type = 2;
    }

    virtual ~SaveGame_Z() { }

    virtual Bool Init() {
        return TRUE;
    }

    virtual void Shut() { }

    virtual void SetGameName(Char* a1) { }

    virtual void SetCountry(MCCOUNTRY a1) { }

    virtual void SetProductNumber(Char* a1) { }

    virtual void SetTitleName(Char* a1) { }

    virtual void SetFileComment(Char* a1) { }

    virtual void SaveFile(Char* a1, void* a2, S32 a3) { }

    virtual void SaveFileA(Char* a1, void* a2, S32 a3) { }

    virtual void LoadFile(Char* a1, void* a2, S32 a3) { }

    virtual void LoadFileA(Char* a1, void* a2, S32 a3) { }

    virtual void DelFile(Char* a1) { }

    virtual void DelFileA(Char* a1) { }

    virtual void RefreshCards() { }

    virtual void RefreshCard() { }

    virtual U32 GetCardStatus() {
        return 0;
    }

    virtual void GetCardFreeSpace() { }

    virtual void CalculateSpaceNeeded(S32 a1, S32* a2) { }

    virtual void CalculateSpaceNeededForOneFile(S32 a1) { }

    virtual void QueryFile(Char* a1) { }

    virtual void QueryFileDateAndSize(Char* a1, SaveDate_Z* a2, S32* a3) { }

    virtual void GetDirectory(FileInfo_Z* a1, S32 a2) { }

    virtual void GetActualFileSize(S32 a1) { }

    virtual void CardSync(S32& a1, S32& a2) { }

    virtual void CanSaveFile(S32 a1, S32& a2) { }

    virtual void FormatCard() { }

    virtual void FormatCardA() { }

    virtual void CreateDirectoryS() { }

    virtual void CreateDirectoryA() { }

    virtual void UnFormatCard() { }

    virtual void IsNewCardConnected() { }

    virtual U32 SetPortAndSlot(int a1, int a2) {
        return 0;
    }

    virtual void ResetManageMC() { }

    virtual void Update(Float a1) { }

    virtual void RegisterAgentAndMessage(ABC_Agent* a1, int a2) { }
};

#endif
