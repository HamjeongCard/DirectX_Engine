#pragma once

#include <cmath>
#include <directxmath.h>

namespace EngineMath {

struct Vector3 {
    float x, y, z;

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }

    static float Dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    static Vector3 Normalize(const Vector3& v) {
        float len = std::sqrt(Dot(v, v));
        if (len > 0.00001f) {
            return Vector3(v.x / len, v.y / len, v.z / len);
        }
        return Vector3(0.0f, 0.0f, 0.0f);
    }
};

struct Matrix4x4 {
    float m[4][4] = { 0 };

    static Matrix4x4 Identity() {
        Matrix4x4 res;
        res.m[0][0] = 1.0f; res.m[1][1] = 1.0f;
        res.m[2][2] = 1.0f; res.m[3][3] = 1.0f;
        return res;
    }

    static Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b) {
        Matrix4x4 res;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                res.m[r][c] = a.m[r][0] * b.m[0][c] +
                              a.m[r][1] * b.m[1][c] +
                              a.m[r][2] * b.m[2][c] +
                              a.m[r][3] * b.m[3][c];
            }
        }
        return res;
    }

    static Matrix4x4 Transpose(const Matrix4x4& mat) {
        Matrix4x4 res;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                res.m[r][c] = mat.m[c][r];
            }
        }
        return res;
    }

    static Matrix4x4 RotationRollPitchYaw(float pitch, float yaw, float roll) {
        float cp = std::cos(pitch), sp = std::sin(pitch);
        float cy = std::cos(yaw),   sy = std::sin(yaw);
        float cr = std::cos(roll),  sr = std::sin(roll);

        Matrix4x4 res;
        res.m[0][0] = cy * cr + sy * sp * sr;
        res.m[0][1] = sr * cp;
        res.m[0][2] = -sy * cr + cy * sp * sr;
        res.m[0][3] = 0.0f;

        res.m[1][0] = -cy * sr + sy * sp * cr;
        res.m[1][1] = cr * cp;
        res.m[1][2] = sr * sy + cy * sp * cr;
        res.m[1][3] = 0.0f;

        res.m[2][0] = sy * cp;
        res.m[2][1] = -sp;
        res.m[2][2] = cy * cp;
        res.m[2][3] = 0.0f;

        res.m[3][0] = 0.0f;
        res.m[3][1] = 0.0f;
        res.m[3][2] = 0.0f;
        res.m[3][3] = 1.0f;

        return res;
    }

    static Matrix4x4 LookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Vector3 zAxis = Vector3::Normalize(target - eye);
        Vector3 xAxis = Vector3::Normalize(Vector3::Cross(up, zAxis));
        Vector3 yAxis = Vector3::Cross(zAxis, xAxis);

        Matrix4x4 res;
        res.m[0][0] = xAxis.x; res.m[0][1] = yAxis.x; res.m[0][2] = zAxis.x; res.m[0][3] = 0.0f;
        res.m[1][0] = xAxis.y; res.m[1][1] = yAxis.y; res.m[1][2] = zAxis.y; res.m[1][3] = 0.0f;
        res.m[2][0] = xAxis.z; res.m[2][1] = yAxis.z; res.m[2][2] = zAxis.z; res.m[2][3] = 0.0f;
        res.m[3][0] = -Vector3::Dot(xAxis, eye);
        res.m[3][1] = -Vector3::Dot(yAxis, eye);
        res.m[3][2] = -Vector3::Dot(zAxis, eye);
        res.m[3][3] = 1.0f;

        return res;
    }

    static Matrix4x4 PerspectiveFovLH(float fovAngleYRad, float aspectRatio, float nearZ, float farZ) {
        float height = 1.0f / std::tan(fovAngleYRad * 0.5f);
        float width = height / aspectRatio;
        float fRange = farZ / (farZ - nearZ);

        Matrix4x4 res;
        res.m[0][0] = width;
        res.m[1][1] = height;
        res.m[2][2] = fRange;
        res.m[2][3] = 1.0f;
        res.m[3][2] = -fRange * nearZ;
        res.m[3][3] = 0.0f;

        return res;
    }
};

} // namespace EngineMath
