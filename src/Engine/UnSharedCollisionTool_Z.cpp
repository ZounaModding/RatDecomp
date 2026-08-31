#include "ObjectCollisionTools_Z.h"

Bool SphereVsBox(const Sphere_Z& i_Sphere, const Box_Z& i_Box) {
    Vec3f l_Delta = i_Sphere.Center - Vec3f(i_Box.Mat.m.m[0][3], i_Box.Mat.m.m[1][3], i_Box.Mat.m.m[2][3]);

    const Vec3f l_Local(
        i_Box.Mat.m.m[0][0] * l_Delta.x + i_Box.Mat.m.m[0][1] * l_Delta.y + i_Box.Mat.m.m[0][2] * l_Delta.z,
        i_Box.Mat.m.m[1][0] * l_Delta.x + i_Box.Mat.m.m[1][1] * l_Delta.y + i_Box.Mat.m.m[1][2] * l_Delta.z,
        i_Box.Mat.m.m[2][0] * l_Delta.x + i_Box.Mat.m.m[2][1] * l_Delta.y + i_Box.Mat.m.m[2][2] * l_Delta.z
    );

    Float l_SquareDistance = 0.0f;
    Float l_SquareRadius = i_Sphere.Radius * i_Sphere.Radius;
    const Vec3f& l_BoxScale = i_Box.Scale;

    for (S32 i = 0; i < 3; i++) {
        if (l_Local[i] < -l_BoxScale[i]) {
            Float l_Distance = l_Local[i] + l_BoxScale[i];
            l_SquareDistance += l_Distance * l_Distance;
        }
        else if (l_Local[i] > l_BoxScale[i]) {
            Float l_Distance = l_Local[i] - l_BoxScale[i];
            l_SquareDistance += l_Distance * l_Distance;
        }

        if (l_SquareDistance > l_SquareRadius) {
            return FALSE;
        }
    }

    return TRUE;
}

Bool SegmentVsBox(const Segment_Z& i_Segment, const Box_Z& i_Box) {
    Vec3f l_Delta;
    l_Delta.x = i_Segment.Origin.x - i_Box.Mat.m.m[0][3];
    l_Delta.y = i_Segment.Origin.y - i_Box.Mat.m.m[1][3];
    l_Delta.z = i_Segment.Origin.z - i_Box.Mat.m.m[2][3];

    Float l_Min;
    Float l_Max = 2.0f;
    l_Min = -l_Max;
    const Float* l_Row = &i_Box.Mat.m.m[0][0];
    const Float* l_Scale = &i_Box.Scale.x;

    for (S32 i = 0; i < 3; i++, l_Row += 4, l_Scale++) {
        Float l_Origin = l_Row[0] * l_Delta.x + l_Row[1] * l_Delta.y + l_Row[2] * l_Delta.z;
        Float l_Direction = i_Segment.Length * (l_Row[0] * i_Segment.Direction.x + l_Row[1] * i_Segment.Direction.y + l_Row[2] * i_Segment.Direction.z);
        Float l_BoxScale = *l_Scale;

        if (l_Direction == 0.0f) {
            if (l_Origin < -l_BoxScale || l_Origin > l_BoxScale) {
                return FALSE;
            }
        }
        else {
            Float l_First;
            Float l_Second;
            Float l_InvDirection = 1.0f / l_Direction;
            l_First = l_InvDirection * (-l_BoxScale - l_Origin);
            l_Second = l_InvDirection * (l_BoxScale - l_Origin);

            if (l_First > l_Second) {
                if (l_Second > l_Min) {
                    l_Min = l_Second;
                }
                if (l_First < l_Max) {
                    l_Max = l_First;
                }
            }
            else {
                if (l_First > l_Min) {
                    l_Min = l_First;
                }
                if (l_Second < l_Max) {
                    l_Max = l_Second;
                }
            }

            if (l_Min > l_Max) {
                return FALSE;
            }
            if (l_Max < 0.0f) {
                return FALSE;
            }
            if (l_Min > 1.0f) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

// TODO: Finish matching
Bool SegmentVsSphere(const Segment_Z& i_Segment, const Sphere_Z& i_Sphere) {
    Float l_Projection;
    Float l_Distance;
    Float l_X;
    Float l_Y;
    Float l_Z;
    l_Projection = (i_Sphere.Center.x - i_Segment.Origin.x) * i_Segment.Direction.x + (i_Sphere.Center.y - i_Segment.Origin.y) * i_Segment.Direction.y + (i_Sphere.Center.z - i_Segment.Origin.z) * i_Segment.Direction.z;
    l_Distance = l_Projection;
    if (l_Projection < 0.0f) {
        l_Distance = 0.0f;
    }
    else if (l_Projection > i_Segment.Length) {
        l_Distance = i_Segment.Length;
    }

    l_X = i_Segment.Direction.x * l_Distance;
    l_Y = i_Segment.Direction.y * l_Distance;
    l_Z = i_Segment.Direction.z * l_Distance;
    l_X += i_Segment.Origin.x;
    l_Y += i_Segment.Origin.y;
    l_Z += i_Segment.Origin.z;
    l_X -= i_Sphere.Center.x;
    l_Y -= i_Sphere.Center.y;
    l_Z -= i_Sphere.Center.z;

    return !(l_X * l_X + l_Y * l_Y + l_Z * l_Z > i_Sphere.Radius * i_Sphere.Radius);
}
