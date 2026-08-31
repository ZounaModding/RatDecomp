#ifndef _PACK_Z_H_
#define _PACK_Z_H_

#include "Types_Z.h"

struct packet {
    S32 m_Bits;
    S32 m_DataSize;
    S32 m_Symbols;
    S32 m_Count;
    S32 m_Repeat[30];
    S32 m_Value[30];
};

struct chlink {
    U8* m_Data;
    chlink* m_Next;
    chlink* m_Back;
};

#define ENCODE_LINKS_MAX 32768
#define ENCODE_CH_MAX 65536

class Pack_Z {
public:
    S32 Pack(const U8* i_Data, S32 i_Size, S32 i_MaxBlockSize);
    void Init(U32 i_Size);
    void Flush();
    S32 EncodeRS(const U8* i_Input, U8* o_Output, S32 i_Size, S32 i_MaxBlockSize);
    Bool EncodePacket(U8* i_Data, packet* io_Packet);

    inline U8* GetPackedData() const { return m_Packed; }

    inline S32 GetPackedSize() const { return m_PackedSize; }

private:
    chlink* m_Links;
    chlink* m_Characters;
    chlink m_Empty;
    chlink* m_Head;
    packet m_Packets[4];
    U8* m_Start;
    U8* m_Finish;
    U8* m_Packed;
    S32 m_UnpackedSize;
    S32 m_PackedSize;
};

#endif
