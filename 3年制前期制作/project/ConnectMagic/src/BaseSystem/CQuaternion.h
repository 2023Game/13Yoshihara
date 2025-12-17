#pragma once
#include "CVector.h"

class CQuaternion
{
public:
	CQuaternion(float x, float y, float z, float w);
	CQuaternion(float x, float y, float z);
	CQuaternion(const CVector& eulerAngles = CVector::zero);
	~CQuaternion();

	CVector operator * (const CVector& v) const;
	CQuaternion operator * (const CQuaternion& c) const;
	CQuaternion operator * (const float& f) const;
	void operator *= (const float& f);

	void Euler(float x, float y, float z);
	void Euler(const CVector& euler);
	CVector Euler() const;

	void RotationAxis(const CVector& axis, float angle);

	CMatrix Matrix() const;

	float Length() const;
	float LengthSqr() const;
	void Normalize();
	CQuaternion Normalized() const;
	CQuaternion Inverse() const;

	static CQuaternion AngleAxis(float angle, const CVector& axis);
	static CQuaternion FromToRotation(const CVector& fromDir, const CVector& toDir);
	static CQuaternion LookRotation(const CVector& forward);
	static CQuaternion LookRotation(const CVector& forward, const CVector& upwards);

	// 回転なし
	const static CQuaternion identity;

	// X軸の値を取得
	float X() const;
	// Y軸の値を取得
	float Y() const;
	// Z軸の値を取得
	float Z() const;
	// W軸の値を取得
	float W() const;

private:
	float mX, mY, mZ, mW;
};