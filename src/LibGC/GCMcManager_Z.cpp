#include "GCMcManager_Z.h"
#include "GCMain_Z.h"
#include "Memory_Z.h"
#include "File_Z.h"
#include "StreamManager_Z.h"

ExternC_Z int sprintf(Char* i_Buffer, const Char* i_Format, ...);

GCMcManager_Z* McManager;

GCMcManager_Z::GCMcManager_Z() {
    McManager = this;
    m_MountWorkBuffer = NULL;
    m_ReadBuffer = NULL;
    m_DataToSave = NULL;
    m_Icon = NULL;
    m_BannerData = NULL;
    m_ReadBufferSize = 0;
    m_DataToSaveSize = 0;
    m_UseSlot = 0;
    m_FileSlotsFree = 0;
    m_BytesFree = 0;
    m_SectorSize = 0;
    m_Unk_0x22c = 0;
    m_AddedTLUT = 0;
    m_BannerType = 0;
    m_BannerDataSize = 0;
    m_IconSizeTotal = 0;
    m_IconCount = -1;
    memset(m_IconData, 0, sizeof(m_IconData));
    strcpy(m_Comment, "Asobo Studio.");
    sprintf(m_DataToSaveFileName, "");
    m_ErrorNum = 0;
    m_State = GC_MC_STATE_NONE;

    m_CardData[GC_MC_SLOT_A].Reset();
    m_CardData[GC_MC_SLOT_B].Reset();
}

GCMcManager_Z::~GCMcManager_Z() {
}

Bool GCMcManager_Z::Init() {
    File_Z l_File;
    m_IsBusy = FALSE;
    CARDInit();
    m_Access = FALSE;
    m_BannerDataSize = -1;
    gData.StreamMgr->Open("Icon.tpl", &m_McFiles[GC_MC_SLOT_A], NULL, 0, -1);
    gData.StreamMgr->Open("opening.bnr", &m_McFiles[GC_MC_SLOT_B], NULL, 0, -1);
    m_CommandToRun = GC_MC_CMD_FINISHED;
    m_Unk_0x158 = 0;
    return TRUE;
}

void GCMcManager_Z::Shut() {
}

void GCMcManager_Z::Update(Float i_DeltaTime) {
    if (m_McFiles[GC_MC_SLOT_A].IsLoaded() && m_McFiles[GC_MC_SLOT_B].IsLoaded()) {
        m_Access = TRUE;
        if (m_BannerDataSize == -1) {
            m_BannerDataSize = 6144;
            m_BannerType = 2;
            m_BannerData = (U8*)m_McFiles[GC_MC_SLOT_B].GetData() + 32;
        }
        if (m_IconCount == -1) {
            AddIconAnimFrame(m_McFiles[GC_MC_SLOT_A], GC_ICONANIM_PLAYBACKRATE_SLOW);
        }
    }

    if (CARDProbe(m_UseSlot) == 0) {
        m_CardData[m_UseSlot].m_Type = 0;
        if (m_State == GC_MC_STATE_RUNNING) {
            m_State = GC_MC_STATE_FINISHED;
            m_ErrorNum = 1;
        }
    }
    else if (m_CardData[m_UseSlot].m_Type == 0) {
        m_CardData[m_UseSlot].m_Type = 1;
    }
}

void GCMcManager_Z::SetGameName(Char* i_GameName) {
    if (i_GameName) {
        strcpy(m_GameName, i_GameName);
    }
}

void GCMcManager_Z::SetCountry(MCCOUNTRY i_Country) {
    m_Localization = i_Country;
}

void GCMcManager_Z::SetProductNumber(Char* i_ProductNumber) {
    if (i_ProductNumber) {
        strcpy(m_ProductNumber, i_ProductNumber);
    }
}

U32 GCMcManager_Z::SetPortAndSlot(int i_Port, int i_Slot) {
    m_UseSlot = i_Slot;
    m_ErrorNum = _MountCard();
    _UnmountCard();
    return m_ErrorNum;
}

void GCMcManager_Z::SetFileComment(Char* i_Comment) {
    strcpy(m_FileComment, i_Comment);
}

void GCMcManager_Z::SetTitleName(Char* i_TitleName) {
    if (i_TitleName) {
        if (strlen(i_TitleName) <= 32) {
            strcpy(m_GameName, i_TitleName);
        }

        SetComment(i_TitleName, "Asobo Studio.");
    }
}

void GCMcManager_Z::IsNewCardConnected() {
}

void GCMcManager_Z::CreateDirectoryA() {
}

void GCMcManager_Z::FormatCardA() {
}

void GCMcManager_Z::QueryFileDateAndSize(Char* a1, SaveDate_Z* a2, S32* a3) {
}

void GCMcManager_Z::CalculateSpaceNeededForOneFile(S32 a1) {
}

void GCMcManager_Z::RefreshCard() {
}

void GCMcManager_Z::ResetManageMC() {
}

void GCMcManager_Z::LoadFileA(Char* a1, void* a2, S32 a3) {
}

void GCMcManager_Z::SaveFileA(Char* a1, void* a2, S32 a3) {
}

void GCMcManager_Z::DelFile(Char* a1) {
}

void GCMcManager_Z::DelFileA(Char* a1) {
}

void GCMcManager_Z::RegisterAgentAndMessage(ABC_Agent* a1, int a2) {
}

void GCMcManager_Z::GetDirectory(FileInfo_Z* a1, S32 a2) {
}

void GCMcManager_Z::SaveFile(Char* a1, void* a2, S32 a3) {
}

void GCMcManager_Z::LoadFile(Char* a1, void* a2, S32 a3) {
}

void GCMcManager_Z::CardSync(S32& a1, S32& a2) {
}

void GCMcManager_Z::RefreshCards() {
}

U32 GCMcManager_Z::GetCardStatus() {
    GCCardData_Z& l_CardData = m_CardData[m_UseSlot];
    if (l_CardData.m_Type == 2) {
        return GC_MC_CARD_STATUS_NO_CARD;
    }
    if (l_CardData.m_Type == 3) {
        return GC_MC_CARD_STATUS_WRONG_DEVICE;
    }
    if (l_CardData.m_Type == 1) {
        if (l_CardData.m_Formatted) {
            return GC_MC_CARD_STATUS_OK;
        }
        return GC_MC_CARD_STATUS_UNFORMATTED;
    }
    return GC_MC_CARD_STATUS_UNKNOWN;
}

void GCMcManager_Z::GetCardFreeSpace() {
}

void GCMcManager_Z::CalculateSpaceNeeded(S32 a1, S32* a2) {
}

void GCMcManager_Z::QueryFile(Char* a1) {
}

void GCMcManager_Z::FormatCard() {
}

void GCMcManager_Z::UnFormatCard() {
}

U32 GCMcManager_Z::_MountCard() {
    if (!m_MountWorkBuffer) {
        m_MountWorkBuffer = AllocAlignCL_Z(40960, "Alloc Mount Card", 1187, 32);
        if (!m_MountWorkBuffer) {
            m_ErrorNum = GC_MC_CARD_STATUS_UNK_14;
            return m_ErrorNum;
        }
    }

    S32 l_Result = CARDMount(m_UseSlot, m_MountWorkBuffer, NULL);
    if (l_Result == CARD_RESULT_ENCODING || l_Result == CARD_RESULT_BROKEN) {
        l_Result = CARDCheck(m_UseSlot);
    }

    m_CardData[m_UseSlot].m_Type = 1;
    m_CardData[m_UseSlot].m_Formatted = 1;

    switch (l_Result) {
        case CARD_RESULT_READY:
            m_ErrorNum = GC_MC_CARD_STATUS_OK;
            break;
        case CARD_RESULT_WRONGDEVICE:
            m_CardData[m_UseSlot].m_Type = 2;
            m_ErrorNum = GC_MC_CARD_STATUS_NO_CARD;
            break;
        case CARD_RESULT_NOCARD:
            m_CardData[m_UseSlot].m_Type = 0;
            m_ErrorNum = GC_MC_CARD_STATUS_UNKNOWN;
            break;
        case CARD_RESULT_NOFILE:
            m_ErrorNum = GC_MC_CARD_STATUS_NO_FILE;
            break;
        case CARD_RESULT_INSSPACE:
            m_ErrorNum = GC_MC_CARD_STATUS_UNK_14;
            break;
        case CARD_RESULT_ENCODING:
        case CARD_RESULT_BROKEN:
            m_CardData[m_UseSlot].m_Formatted = 0;
            m_ErrorNum = GC_MC_CARD_STATUS_UNFORMATTED;
            break;
        case CARD_RESULT_IOERROR:
            m_CardData[m_UseSlot].m_Formatted = 0;
            m_CardData[m_UseSlot].m_Type = 3;
            m_ErrorNum = GC_MC_CARD_STATUS_WRONG_DEVICE;
            break;
    }

    if (m_IsBusy) {
        m_IsBusy = FALSE;
    }
    return m_ErrorNum;
}

void GCMcManager_Z::_UnmountCard() {
    CARDUnmount(m_UseSlot);

    if (m_MountWorkBuffer) {
        Free_Z(m_MountWorkBuffer);
        m_MountWorkBuffer = NULL;
    }
}

void GCMcManager_Z::SetComment(Char* i_FirstLine, Char* i_SecondLine) {
    if (i_FirstLine) {
        if (i_SecondLine) {
            S32 l_Length = strlen(i_FirstLine);
            strcpy(m_Comment, i_FirstLine);

            S32 i;
            for (i = l_Length; i < 32; i++) {
                m_Comment[i] = ' ';
            }

            m_Comment[i] = '\0';
            strcat(m_Comment, i_SecondLine);
        }
    }
}

void GCMcManager_Z::_ParseTPLFile(TEXPalettePtr i_Icon) {
    U16 l_Index;

    if (i_Icon->versionNumber != 2142000) {
        return;
    }

    i_Icon->descriptorArray = (TEXDescriptorPtr)((U32)i_Icon->descriptorArray + (U32)i_Icon);

    for (l_Index = 0; l_Index < i_Icon->numDescriptors; l_Index++) {
        if (i_Icon->descriptorArray[l_Index].textureHeader) {
            i_Icon->descriptorArray[l_Index].textureHeader = (TEXHeaderPtr)((U32)i_Icon->descriptorArray[l_Index].textureHeader + (U32)i_Icon
            );

            if (!i_Icon->descriptorArray[l_Index].textureHeader->unpacked) {
                i_Icon->descriptorArray[l_Index].textureHeader->data = (Ptr)((U32)i_Icon->descriptorArray[l_Index].textureHeader->data + (U32)i_Icon
                );
                i_Icon->descriptorArray[l_Index].textureHeader->unpacked = TRUE;
            }
        }

        if (i_Icon->descriptorArray[l_Index].CLUTHeader) {
            i_Icon->descriptorArray[l_Index].CLUTHeader = (CLUTHeaderPtr)((U32)i_Icon->descriptorArray[l_Index].CLUTHeader + (U32)i_Icon
            );

            if (!i_Icon->descriptorArray[l_Index].CLUTHeader->unpacked) {
                i_Icon->descriptorArray[l_Index].CLUTHeader->data = (Ptr)((U32)i_Icon->descriptorArray[l_Index].CLUTHeader->data + (U32)i_Icon
                );
                i_Icon->descriptorArray[l_Index].CLUTHeader->unpacked = TRUE;
            }
        }
    }
}

void GCMcManager_Z::AddIconAnimFrame(
    McFiles_Z& i_File,
    GC_ICONANIM_PLAYBACKRATE i_Rate
) {
    S32 l_BitmapType;
    S32 l_IconSize;
    S32 l_TLUTSize;
    U32 l_Index;

    m_IconCount = 0;
    m_Icon = (TEXPalettePtr)i_File.GetData();
    _ParseTPLFile(m_Icon);

    for (l_Index = 0;
         l_Index < m_Icon->numDescriptors && m_IconCount < CARD_ICON_MAX;
         l_Index++) {
        TEXDescriptorPtr l_IconDescriptor = TEXGet(m_Icon, l_Index);

        U32 l_Format = l_IconDescriptor->textureHeader->format;
        switch (l_Format) {
            case GX_TF_RGB5A3:
                l_BitmapType = CARD_STAT_ICON_RGB5A3;
                l_IconSize = 2 * CARD_ICON_WIDTH * CARD_ICON_HEIGHT;
                l_TLUTSize = 0;
                break;
            case GX_TF_C8:
                l_BitmapType = CARD_STAT_ICON_C8;
                l_IconSize = CARD_ICON_WIDTH * CARD_ICON_HEIGHT;
                l_TLUTSize = 2 * 256;
                break;
            default:
                l_BitmapType = -1;
                break;
        }

        if (l_BitmapType != -1 && l_IconDescriptor->textureHeader->width == CARD_ICON_WIDTH && l_IconDescriptor->textureHeader->height == CARD_ICON_HEIGHT) {
            m_IconData[m_IconCount].m_Rate = i_Rate;
            m_IconData[m_IconCount].m_FileName = "Icon.tpl";
            m_IconData[m_IconCount].m_BitmapType = l_BitmapType;
            m_IconSizeTotal += l_IconSize;

            if (l_TLUTSize != 0 && m_AddedTLUT == 0) {
                m_IconSizeTotal += l_TLUTSize;
                m_AddedTLUT = TRUE;
            }

            m_IconCount++;
        }
    }
}
