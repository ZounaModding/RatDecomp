#include "Mesh_Z.h"

void MeshVolume_Z::Load(void** i_Data) {
    S32 i;
    S32 l_Nb;

    LOADDYNARRAY_Z(m_Vertices);
    LOADDYNARRAY_Z(m_UnkVertices);
    LOADDYNARRAY_Z(m_FaceVolumes);
    LOADDYNARRAY_Z(m_EdgeVolumes);

    l_Nb = m_EdgeVolumes.GetSize();
    for (i = 0; i < l_Nb; i++) {
        S32 l_Face1 = (S32)m_EdgeVolumes[i].m_Face1;
        S32 l_Face2 = (S32)m_EdgeVolumes[i].m_Face2;
        m_EdgeVolumes[i].m_Face1 = &m_FaceVolumes[l_Face1];
        m_EdgeVolumes[i].m_Face2 = &m_FaceVolumes[l_Face2];
        S32 l_Vertex1 = (S32)m_EdgeVolumes[i].m_Vertex1;
        S32 l_Vertex2 = (S32)m_EdgeVolumes[i].m_Vertex2;
        m_EdgeVolumes[i].m_Vertex1 = &m_Vertices[l_Vertex1];
        m_EdgeVolumes[i].m_Vertex2 = &m_Vertices[l_Vertex2];
    }

    l_Nb = m_FaceVolumes.GetSize();
    for (i = 0; i < l_Nb; i++) {
        S32 l_Vertex1 = (S32)m_FaceVolumes[i].m_Vertex1;
        S32 l_Vertex2 = (S32)m_FaceVolumes[i].m_Vertex2;
        S32 l_Vertex3 = (S32)m_FaceVolumes[i].m_Vertex3;
        m_FaceVolumes[i].m_Vertex1 = &m_Vertices[l_Vertex1];
        m_FaceVolumes[i].m_Vertex2 = &m_Vertices[l_Vertex2];
        m_FaceVolumes[i].m_Vertex3 = &m_Vertices[l_Vertex3];
    }
}

// TODO: Finish matching
void MeshVolume_Z::Clone(const MeshVolume_Z& i_MeshVolume) {
    S32 i;
    S32 l_Nb;

    l_Nb = i_MeshVolume.m_Vertices.GetSize();
    m_Vertices.SetSize(l_Nb);
    Sys_Z::MemCpyFrom(m_Vertices.GetArrayPtr(), i_MeshVolume.m_Vertices.GetArrayPtr(), l_Nb * sizeof(Vec4f));

    l_Nb = i_MeshVolume.m_UnkVertices.GetSize();
    m_UnkVertices.SetSize(l_Nb);
    Sys_Z::MemCpyFrom(m_UnkVertices.GetArrayPtr(), i_MeshVolume.m_UnkVertices.GetArrayPtr(), l_Nb * sizeof(Vec4f));

    l_Nb = i_MeshVolume.m_FaceVolumes.GetSize();
    m_FaceVolumes.SetSize(l_Nb);
    {
        const Vec4f* l_SourceVertices = i_MeshVolume.m_Vertices.GetArrayPtr();
        for (i = 0; i < l_Nb; i++) {
            S32 l_Vertex1 = AlignDown_Z((U32)i_MeshVolume.m_FaceVolumes[i].m_Vertex1 - (U32)l_SourceVertices, sizeof(Vec4f));
            S32 l_Vertex2 = AlignDown_Z((U32)i_MeshVolume.m_FaceVolumes[i].m_Vertex2 - (U32)l_SourceVertices, sizeof(Vec4f));
            S32 l_Vertex3 = AlignDown_Z((U32)i_MeshVolume.m_FaceVolumes[i].m_Vertex3 - (U32)l_SourceVertices, sizeof(Vec4f));
            m_FaceVolumes[i].m_Vertex1 = (Vec4f*)((U32)m_Vertices.GetArrayPtr() + l_Vertex1);
            m_FaceVolumes[i].m_Vertex2 = (Vec4f*)((U32)m_Vertices.GetArrayPtr() + l_Vertex2);
            m_FaceVolumes[i].m_Vertex3 = (Vec4f*)((U32)m_Vertices.GetArrayPtr() + l_Vertex3);
            m_FaceVolumes[i].m_Normal = i_MeshVolume.m_FaceVolumes[i].m_Normal;
            m_FaceVolumes[i].m_Dot = 1.f;
        }
    }

    S32 l_EdgeNb = i_MeshVolume.m_EdgeVolumes.GetSize();
    m_EdgeVolumes.SetSize(l_EdgeNb);
    for (i = 0; i < l_EdgeNb; i++) {
        S32 l_Face1 = AlignDown_Z((U32)i_MeshVolume.m_EdgeVolumes[i].m_Face1 - (U32)i_MeshVolume.m_FaceVolumes.GetArrayPtr(), sizeof(FaceVolume_Z));
        S32 l_Face2 = AlignDown_Z((U32)i_MeshVolume.m_EdgeVolumes[i].m_Face2 - (U32)i_MeshVolume.m_FaceVolumes.GetArrayPtr(), sizeof(FaceVolume_Z));
        m_EdgeVolumes[i].m_Face1 = (FaceVolume_Z*)((U32)m_FaceVolumes.GetArrayPtr() + l_Face1);
        m_EdgeVolumes[i].m_Face2 = (FaceVolume_Z*)((U32)m_FaceVolumes.GetArrayPtr() + l_Face2);
        S32 l_Vertex1 = AlignDown_Z((U32)i_MeshVolume.m_EdgeVolumes[i].m_Vertex1 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr(), sizeof(Vec4f));
        S32 l_Vertex2 = AlignDown_Z((U32)i_MeshVolume.m_EdgeVolumes[i].m_Vertex2 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr(), sizeof(Vec4f));
        m_EdgeVolumes[i].m_Vertex1 = (Vec4f*)((U32)m_Vertices.GetArrayPtr() + l_Vertex1);
        m_EdgeVolumes[i].m_Vertex2 = (Vec4f*)((U32)m_Vertices.GetArrayPtr() + l_Vertex2);
    }
}
