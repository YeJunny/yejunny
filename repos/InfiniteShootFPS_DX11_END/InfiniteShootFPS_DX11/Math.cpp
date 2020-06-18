#include "Math.h"

using namespace DirectX;

DirectX::XMVECTOR Math::LookRotationQuaternion(DirectX::XMVECTOR forward, DirectX::XMVECTOR up)
{
    XMVECTOR vector = XMVector3Normalize(forward);
    XMVECTOR vector2 = XMVector3Normalize(XMVector3Cross(up, vector));
    XMVECTOR vector3 = XMVector3Cross(vector, vector2);
    float m00 = XMVectorGetX(vector2);
    float m01 = XMVectorGetY(vector2);
    float m02 = XMVectorGetZ(vector2);
    float m10 = XMVectorGetX(vector3);
    float m11 = XMVectorGetY(vector3);
    float m12 = XMVectorGetZ(vector3);
    float m20 = XMVectorGetX(vector);
    float m21 = XMVectorGetY(vector);
    float m22 = XMVectorGetZ(vector);


    float num8 = (m00 + m11) + m22;
    XMVECTOR quaternion;
    if (num8 > 0.0f)
    {
        float num = static_cast<float>(sqrt(num8 + 1.0f));
        float num2 = 0.5f / num;
        float x = (m12 - m21) * num2;
        float y = (m20 - m02) * num2;
        float z = (m01 - m10) * num2;
        float w = num * 0.5f;
        quaternion = XMVectorSet(x, y, z, w);
        return quaternion;
    }
    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = static_cast<float>(sqrt(((1.0f + m00) - m11) - m22));
        float num4 = 0.5f / num7;
        float x = 0.5f * num7;
        float y = (m01 + m10) * num4;
        float z = (m02 + m20) * num4;
        float w = (m12 - m21) * num4;
        quaternion = XMVectorSet(x, y, z, w);
        return quaternion;
    }
    if (m11 > m22)
    {
        float num6 = static_cast<float>(sqrt(((1.0f + m11) - m00) - m22));
        float num3 = 0.5f / num6;
        float x = (m10 + m01) * num3;
        float y = 0.5f * num6;
        float z = (m21 + m12) * num3;
        float w = (m20 - m02) * num3;
        quaternion = XMVectorSet(x, y, z, w);
        return quaternion;
    }
    float num5 = static_cast<float>(sqrt(((1.0f + m22) - m00) - m11));
    float num2 = 0.5f / num5;
    float x = (m20 + m02) * num2;
    float y = (m21 + m12) * num2;
    float z = 0.5f * num5;
    float w = (m01 - m10) * num2;
    quaternion = XMVectorSet(x, y, z, w);
    return quaternion;
}
