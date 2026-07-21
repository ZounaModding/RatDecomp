#ifndef _GCMCMANAGER_Z_H_
#define _GCMCMANAGER_Z_H_
#include "SaveGame_Z.h"
#include "BaseStream_Z.h"
#include "GCMain_Z.h"
#include "StreamManager_Z.h"
#include <card.h>
#include <charPipeline/texPalette.h>

enum GC_MC_STATE {
    GC_MC_STATE_NONE = 0,
    GC_MC_STATE_RUNNING = 1,
    GC_MC_STATE_FINISHED = 2
};

enum GC_MC_CMD {
    GC_MC_CMD_NONE = 0,
    GC_MC_CMD_LOAD_ASYNC = 1,
    GC_MC_CMD_FORMAT_ASYNC = 2,
    GC_MC_CMD_DELETE_ASYNC = 3,
    GC_MC_CMD_FINISHED = 4
};

enum GC_MC_SLOT {
    GC_MC_SLOT_A = 0,
    GC_MC_SLOT_B = 1
};

typedef enum GC_ICONANIM_PLAYBACKRATE {
    GC_ICONANIM_PLAYBACKRATE_FAST = 1,
    GC_ICONANIM_PLAYBACKRATE_MED = 2,
    GC_ICONANIM_PLAYBACKRATE_SLOW = 3
} GC_ICONANIM_PLAYBACKRATE;

class McFiles_Z : public BaseStream_Z {
public:
    McFiles_Z() {
        m_Loaded = FALSE;
        m_Data = NULL;
        m_Unk_0x8 = 0;
    }

    virtual void Opened(S32 i_ErrorCode, S32 i_FileSize, S32 i_UserDefined) {
        m_Unk_0x8 = i_FileSize;
        m_Data = AllocAlignL_Z(i_FileSize, 109, 32);
        gData.StreamMgr->Read(m_Data, 0, i_FileSize, this, -1);
    }

    virtual void Readed(S32 i_ErrorCode, S32 i_NbBytesRead, S32 i_UserDefined) {
        m_Loaded = TRUE;
        gData.StreamMgr->Close(this, TRUE);
    }

    Bool IsLoaded() const { return m_Loaded; }

    void* GetData() const { return m_Data; }

private:
    S32 m_Unk_0x8;
    void* m_Data;
    Bool m_Loaded;
    U8 m_Pad_0x11[3];
};

enum GC_MC_CARD_STATUS {
    GC_MC_CARD_STATUS_OK = 0,
    GC_MC_CARD_STATUS_UNKNOWN = 1,
    GC_MC_CARD_STATUS_UNFORMATTED = 3,
    GC_MC_CARD_STATUS_NO_FILE = 6,
    GC_MC_CARD_STATUS_UNK_14 = 14,
    GC_MC_CARD_STATUS_NO_CARD = 22,
    GC_MC_CARD_STATUS_WRONG_DEVICE = 23
};

class GCCardData_Z {
public:
    U16 m_Type : 4;
    U16 m_Formatted : 4;
    U16 m_Slot : 4;
    U16 m_Port : 4;
    U16 m_Free;

    void Reset() {
        m_Type = 0;
        m_Formatted = 0;
        m_Free = 0;
        m_Slot = 0;
        m_Port = 0;
    }
};

struct IconEntry {
    U32 m_BitmapType;
    GC_ICONANIM_PLAYBACKRATE m_Rate;
    Char* m_FileName;
};

class GCMcManager_Z : public SaveGame_Z {
public:
    GCMcManager_Z();
    virtual ~GCMcManager_Z();
    virtual Bool Init();
    virtual void Shut();
    virtual void SetGameName(Char* a1);
    virtual void SetCountry(MCCOUNTRY a1);
    virtual void SetProductNumber(Char* a1);
    virtual void SetTitleName(Char* a1);
    virtual void SetFileComment(Char* a1);
    virtual void SaveFile(Char* a1, void* a2, S32 a3);
    virtual void SaveFileA(Char* a1, void* a2, S32 a3);
    virtual void LoadFile(Char* a1, void* a2, S32 a3);
    virtual void LoadFileA(Char* a1, void* a2, S32 a3);
    virtual void DelFile(Char* a1);
    virtual void DelFileA(Char* a1);
    virtual void RefreshCards();
    virtual void RefreshCard();
    virtual U32 GetCardStatus();
    virtual void GetCardFreeSpace();
    virtual void CalculateSpaceNeeded(S32 a1, S32* a2);
    virtual void CalculateSpaceNeededForOneFile(S32 a1);
    virtual void QueryFile(Char* a1);
    virtual void QueryFileDateAndSize(Char* a1, SaveDate_Z* a2, S32* a3);
    virtual void GetDirectory(FileInfo_Z* a1, S32 a2);
    virtual void GetActualFileSize(S32 a1);
    virtual void CardSync(S32& a1, S32& a2);
    virtual void FormatCard();
    virtual void FormatCardA();
    virtual void CreateDirectoryA();
    virtual void UnFormatCard();
    virtual void IsNewCardConnected();
    virtual U32 SetPortAndSlot(int a1, int a2);
    virtual void ResetManageMC();
    virtual void Update(Float a1);
    virtual void RegisterAgentAndMessage(ABC_Agent* a1, int a2);

    S32 GetIsBusy() const { return m_IsBusy; }

    void SetComment(Char* i_FirstLine, Char* i_SecondLine);
    void AddIconAnimFrame(McFiles_Z& i_File, GC_ICONANIM_PLAYBACKRATE i_Rate);
    void _ParseTPLFile(TEXPalettePtr i_Icon);
    U32 _MountCard();
    void _UnmountCard();

private:
    Char m_GameName[32];
    Char m_ProductNumber[32];
    Char m_FileComment[32];
    MCCOUNTRY m_Localization;
    void* m_MountWorkBuffer;
    U32 m_SectorSize;
    S32 m_BytesFree;
    S32 m_FileSlotsFree;
    U32 m_UseSlot;
    U8 m_UnkBytes_0x80[32];
    Char* m_ReadBuffer;
    void* m_DataToSave;
    void* m_BannerData;
    S32 m_DataToSaveSize;
    Char m_DataToSaveFileName[32];
    CARDFileInfo m_CardFileInfo;
    CARDStat m_CardStat;
    U32 m_ErrorNum;
    GC_MC_STATE m_State;
    U32 m_Unk_0x158;
    GC_MC_CMD m_CommandToRun;
    S32 m_IsBusy;
    U32 m_ReadBufferSize;
    TEXPalettePtr m_Icon;
    GCCardData_Z m_CardData[2];
    U32 m_BannerDataSize;
    U32 m_BannerType;
    U32 m_IconSizeTotal;
    S32 m_AddedTLUT;
    U32 m_IconCount;
    IconEntry m_IconData[8];
    Char m_Comment[64];
    U32 m_IconAnimPlaybackType;
    U32 m_Unk_0x22c;
    McFiles_Z m_McFiles[2];
    Bool m_Access;
};

#endif // _GCMCMANAGER_Z_H_
