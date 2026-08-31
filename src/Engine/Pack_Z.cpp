#include "Pack_Z.h"
#include "Memory_Z.h"
#include "Math_Z.h"

S32 Pack_Z::Pack(const U8* i_Data, S32 i_Size, S32 i_MaxBlockSize) {
    m_UnpackedSize = i_Size;
    if (m_Packed && m_Links && m_Characters) {
        EncodeRS(i_Data, m_Packed, m_UnpackedSize, i_MaxBlockSize);
    }
    return 100 - (m_PackedSize * 100) / m_UnpackedSize;
}

void Pack_Z::Init(U32 i_Size) {
    m_Packed = (U8*)AllocL_Z(i_Size * 2, 21);
    m_UnpackedSize = i_Size;
    m_Links = (chlink*)AllocL_Z(ENCODE_LINKS_MAX * sizeof(chlink), 23);
    m_Characters = (chlink*)AllocL_Z(ENCODE_CH_MAX * sizeof(chlink), 24);
}

void Pack_Z::Flush() {
    if (m_Packed) {
        Free_Z(m_Packed);
    }
    if (m_Links) {
        Free_Z(m_Links);
    }
    if (m_Characters) {
        Free_Z(m_Characters);
    }

    m_UnpackedSize = 0;
    m_PackedSize = 0;
    m_Packed = NULL;
    m_Links = NULL;
    m_Characters = NULL;
}

// TODO: Finish matching
S32 Pack_Z::EncodeRS(const U8* i_Input, U8* o_Output, S32 i_Size, S32 i_MaxBlockSize) {
    U32 l_Size;
    S32 l_Max;
    U8* l_OutputStart;
    U8* l_Output;
    U8* l_Current;

    U32 l_Code;
    S32 i, j;
    S32 l_Index;
    S32 l_Refill, l_RefillCount;

    S32 l_BlockSize;
    const U8* l_NextInput;
    S32 l_RemainingSize;

    l_Size = i_Size;
    o_Output[0] = (U8)(l_Size & 0xff);
    o_Output[1] = (U8)((i_Size >> 8) & 0xff);
    o_Output[2] = (U8)((i_Size >> 16) & 0xff);
    o_Output[3] = (U8)((i_Size >> 24) & 0xff);

    l_OutputStart = o_Output;
    l_Output = o_Output + 8;

    l_RemainingSize = i_Size;
    l_NextInput = i_Input;

    l_BlockSize = i_MaxBlockSize != 0 ? Min<S32>(i_MaxBlockSize, l_RemainingSize) : l_RemainingSize;

    i_Size = l_BlockSize;

    while (l_RemainingSize > 0) {
        m_Start = (U8*)l_NextInput;
        l_Current = m_Start;
        m_Finish = (U8*)l_NextInput + l_BlockSize;
        l_NextInput += l_BlockSize;

        m_Packets[0].m_Bits = 2;
        m_Packets[1].m_Bits = 3;
        m_Packets[2].m_Bits = 4;
        m_Packets[3].m_Bits = 5;

        m_Packets[0].m_Count = 0;
        m_Packets[1].m_Count = 0;
        m_Packets[2].m_Count = 0;
        m_Packets[3].m_Count = 0;

        m_Empty.m_Data = NULL;
        m_Empty.m_Next = NULL;
        m_Empty.m_Back = NULL;

        for (i = 0; i < ENCODE_CH_MAX; i++) {
            m_Characters[i].m_Next = &m_Empty;
            m_Characters[i].m_Data = NULL;
            m_Characters[i].m_Back = NULL;
        }

        m_Head = &m_Links[0];

        j = ENCODE_LINKS_MAX;
        if ((U32)l_BlockSize < ENCODE_LINKS_MAX) {
            j = l_BlockSize;
        }

        for (i = 0; i < j; i++) {
            l_Index = (m_Start[i] << 8) + m_Start[i + 1];

            m_Links[i].m_Data = &m_Start[i];
            m_Links[i].m_Next = m_Characters[l_Index].m_Next;
            m_Links[i].m_Back = &m_Characters[l_Index];

            m_Links[i].m_Next->m_Back = m_Characters[l_Index].m_Next = &m_Links[i];
        }

        l_Refill = ENCODE_LINKS_MAX;
        l_RefillCount = 28672;

        while (l_Current < m_Finish) {
            m_Packets[0].m_DataSize = m_Packets[1].m_DataSize = m_Packets[2].m_DataSize = m_Packets[3].m_DataSize = 0;

            l_Max = 3;

            if (EncodePacket(l_Current, &m_Packets[3])) {
                if (m_Packets[3].m_DataSize <= 540) {
                    if (EncodePacket(l_Current, &m_Packets[2])) {
                        if (m_Packets[2].m_DataSize > m_Packets[3].m_DataSize) {
                            l_Max = 2;
                        }

                        if (m_Packets[l_Max].m_DataSize <= 300) {
                            if (EncodePacket(l_Current, &m_Packets[1])) {
                                if (m_Packets[1].m_DataSize > m_Packets[l_Max].m_DataSize) {
                                    l_Max = 1;
                                }

                                if (m_Packets[l_Max].m_DataSize <= 180) {
                                    EncodePacket(l_Current, &m_Packets[0]);

                                    if (m_Packets[0].m_DataSize >= m_Packets[l_Max].m_DataSize) {
                                        l_Max = 0;
                                    }
                                }
                            }
                            else {
                                l_Max = 1;
                            }
                        }
                    }
                    else {
                        l_Max = 2;
                    }
                }
            }
            else {
                l_Max = 3;
            }

            m_Packets[l_Max].m_Count++;

            l_Code = 0;

            for (i = 0; i < m_Packets[l_Max].m_Symbols; i++) {
                if (m_Packets[l_Max].m_Repeat[i] >= 0) {
                    l_Code |= 0x80000000 >> i;
                }
            }

            l_Code |= l_Max;

            *l_Output++ = (U8)(l_Code >> 24);
            *l_Output++ = (U8)((l_Code >> 16) & 255);
            *l_Output++ = (U8)((l_Code >> 8) & 255);
            *l_Output++ = (U8)(l_Code & 255);

            for (i = 0; i < m_Packets[l_Max].m_Symbols; i++) {
                if (m_Packets[l_Max].m_Repeat[i] == -1) {
                    *l_Output++ = (U8)m_Packets[l_Max].m_Value[i];
                }
                else {
                    l_Code = (U32)((m_Packets[l_Max].m_Repeat[i] << (14 - l_Max)) + (m_Packets[l_Max].m_Value[i] - 1));

                    *l_Output++ = (U8)((l_Code >> 8) & 255);
                    *l_Output++ = (U8)(l_Code & 255);
                }
            }

            l_Current += m_Packets[l_Max].m_DataSize;

            m_Head += m_Packets[l_Max].m_DataSize;
            if (m_Head >= &m_Links[ENCODE_LINKS_MAX]) {
                m_Head -= ENCODE_LINKS_MAX;
            }

            if ((l_RefillCount -= m_Packets[l_Max].m_DataSize) < 0) {
                i = l_Refill;
                l_Refill += 4096;

                if ((U32)l_Refill > (U32)l_BlockSize) {
                    l_Refill = i_Size;
                }

                while (i < l_Refill) {
                    j = i & (ENCODE_LINKS_MAX - 1);

                    m_Links[j].m_Back->m_Next = &m_Empty;

                    l_Index = (m_Start[i] << 8) + m_Start[i + 1];

                    m_Links[j].m_Data = &m_Start[i];
                    m_Links[j].m_Next = m_Characters[l_Index].m_Next;
                    m_Links[j].m_Back = &m_Characters[l_Index];

                    m_Links[j].m_Next->m_Back = m_Characters[l_Index].m_Next = &m_Links[j];

                    i++;
                }

                l_RefillCount += 4096;
            }
        }

        l_RemainingSize -= l_BlockSize;
    }

    m_PackedSize = l_Output - l_OutputStart;

    l_OutputStart[4] = (U8)(m_PackedSize & 0xff);
    l_OutputStart[5] = (U8)((m_PackedSize >> 8) & 0xff);
    l_OutputStart[6] = (U8)((m_PackedSize >> 16) & 0xff);
    l_OutputStart[7] = (U8)((m_PackedSize >> 24) & 0xff);

    return m_PackedSize;
}

Bool Pack_Z::EncodePacket(U8* i_Data, packet* io_Packet) {
    S32 i;
    S32 l_Max;
    S32 l_MaxSize;
    U8* l_Limit;
    S32 l_TestLength;
    S32 l_Length;
    U8* l_Search;
    chlink* l_Link;
    U8* l_Match;
    chlink* l_Head = m_Head;

    l_Max = (1 << io_Packet->m_Bits) + 2;
    l_MaxSize = 0x10000 >> io_Packet->m_Bits;

    for (i = 0; i < 30; i++) {
        l_Limit = i_Data - l_MaxSize;
        if (l_Limit < m_Start) {
            l_Limit = m_Start;
        }

        l_Length = 2;
        l_Link = l_Head->m_Next;
        if (i_Data + l_Max >= m_Finish) {
            l_Max = m_Finish - i_Data;
        }

        if (l_Max > 2) {
            U8 l_ThirdCharacter = i_Data[2];
            while ((l_Search = l_Link->m_Data) >= l_Limit) {
                if (l_Search[2] == l_ThirdCharacter) {
                    l_TestLength = 3;
                    while (l_Search[l_TestLength] == i_Data[l_TestLength] && l_TestLength != l_Max) {
                        l_TestLength++;
                    }
                    if (l_TestLength > l_Length) {
                        l_Length = l_TestLength;
                        l_Match = l_Search;
                        if (l_Length == l_Max) {
                            break;
                        }
                    }
                }
                l_Link = l_Link->m_Next;
            }
        }

        if (l_Length == 2) {
            io_Packet->m_DataSize++;
            io_Packet->m_Repeat[i] = -1;
            io_Packet->m_Value[i] = *i_Data++;
            l_Head++;
        }
        else {
            io_Packet->m_DataSize += l_Length;
            io_Packet->m_Repeat[i] = l_Length - 3;
            io_Packet->m_Value[i] = i_Data - l_Match;
            i_Data += l_Length;
            l_Head += l_Length;
        }

        if (l_Head >= &m_Links[ENCODE_LINKS_MAX]) {
            l_Head -= ENCODE_LINKS_MAX;
        }

        if (i_Data == m_Finish) {
            io_Packet->m_Symbols = i + 1;
            return FALSE;
        }
    }

    io_Packet->m_Symbols = i;
    return TRUE;
}
