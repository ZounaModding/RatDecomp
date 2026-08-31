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
    S32 l_Nb = i_MeshVolume.m_Vertices.GetSize();
    m_Vertices.SetSize(l_Nb);
    Sys_Z::MemCpyFrom(m_Vertices.GetArrayPtr(), i_MeshVolume.m_Vertices.GetArrayPtr(), l_Nb * sizeof(Vec4f));

    l_Nb = i_MeshVolume.m_UnkVertices.GetSize();
    m_UnkVertices.SetSize(l_Nb);
    Sys_Z::MemCpyFrom(m_UnkVertices.GetArrayPtr(), i_MeshVolume.m_UnkVertices.GetArrayPtr(), l_Nb * sizeof(Vec4f));

    l_Nb = i_MeshVolume.m_FaceVolumes.GetSize();
    m_FaceVolumes.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        FaceVolume_Z& l_Face = m_FaceVolumes[i];
        const FaceVolume_Z& l_SourceFace = i_MeshVolume.m_FaceVolumes[i];
        l_Face.m_Vertex1 = &m_Vertices[((U32)l_SourceFace.m_Vertex1 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr()) / sizeof(Vec4f)];
        l_Face.m_Vertex2 = &m_Vertices[((U32)l_SourceFace.m_Vertex2 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr()) / sizeof(Vec4f)];
        l_Face.m_Vertex3 = &m_Vertices[((U32)l_SourceFace.m_Vertex3 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr()) / sizeof(Vec4f)];
        l_Face.m_Normal = l_SourceFace.m_Normal;
        l_Face.m_Dot = 1.f;
    }

    l_Nb = i_MeshVolume.m_EdgeVolumes.GetSize();
    m_EdgeVolumes.SetSize(l_Nb);
    for (S32 i = 0; i < l_Nb; i++) {
        EdgeVolume_Z& l_Edge = m_EdgeVolumes[i];
        const EdgeVolume_Z& l_SourceEdge = i_MeshVolume.m_EdgeVolumes[i];
        l_Edge.m_Face1 = &m_FaceVolumes[((U32)l_SourceEdge.m_Face1 - (U32)i_MeshVolume.m_FaceVolumes.GetArrayPtr()) / sizeof(FaceVolume_Z)];
        l_Edge.m_Face2 = &m_FaceVolumes[((U32)l_SourceEdge.m_Face2 - (U32)i_MeshVolume.m_FaceVolumes.GetArrayPtr()) / sizeof(FaceVolume_Z)];
        l_Edge.m_Vertex1 = &m_Vertices[((U32)l_SourceEdge.m_Vertex1 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr()) / sizeof(Vec4f)];
        l_Edge.m_Vertex2 = &m_Vertices[((U32)l_SourceEdge.m_Vertex2 - (U32)i_MeshVolume.m_Vertices.GetArrayPtr()) / sizeof(Vec4f)];
    }
}
