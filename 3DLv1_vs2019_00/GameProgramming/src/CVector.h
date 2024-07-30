
#ifndef CVECTOR_H
#define CVECTOR_H
#include "CMatrix.h"

/*
ベクトルクラス
ベクトルデータを扱います
*/
class CVector 
{
public:
	//正規化
	//大きさ1のベクトルを返す
	CVector Normalize() const;
	//外積
	//Cross(ベクトル)
	CVector Cross(const CVector& v)const;
	//*演算子のオーバーロード
	//CVector * floatの演算結果を返す
	CVector operator*(const float& f)const;
	//内積
	//Dot(ベクトル)
	float Dot(const CVector& v)const;
	//ベクトルの長さを返す
	float Length() const;
	//CVector*CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix& m);
	//-演算子のオーバーロード
	//CVector - CVectorの演算結果を返す
	CVector operator-(const CVector& v)const;
	//+演算子のオーバーロード
	//CVector + CVectorの演算結果を返す
	CVector operator+(const CVector& v)const;
	//+=演算子のオーバーロード
	//CVector1+=CVector2の演算を行う
	void operator +=(const CVector& v);
	//デフォルトコンストラクタ
	CVector();
	//コンストラクタ
	//CVector(X座標,Y座標,Z座標)
	CVector(float x, float y, float z);
	//各軸での値の設定
	//Set(X座標, Y座標, Z座標)
	void Set(float x, float y, float z);
	//Xの値を得る
	float X() const;
	//Yの値を得る
	float Y() const;
	//Zの値を得る
	float Z() const;
	
	void X(float f);
	void Y(float f);
	void Z(float f);

	//点と線の最短距離を求める
	float CalcPointLineDist(const CVector& p, const CVector& s, const CVector& e, CVector* mp, float* t);
	//2線間の最短距離を返す
	float CalcLineLineDist(
		const CVector& s1,//始点1
		const CVector& e1,//終点1
		const CVector& s2,//始点2
		const CVector& e2,//終点2
		CVector* mp1,//交点1
		CVector* mp2,//交点2
		float* t1,//比率1
		float* t2//比率2
	);
	//0~1の間にクランプ(値を強制的にある範囲内にすること)
	void clamp0to1(float& v);
	//2線分間の最短距離
	float CalcSegmentSegmentDist(
		const CVector& s1, const CVector& e1,//線分1
		const CVector& s2, const CVector& e2,//線分2
		CVector* mp1,//最短線の端点1（始点や終点になることもある）
		CVector* mp2//最短線の端点2（支店や終点になることもある）
	);
private:
	//3D各軸での値を設定
	float mX, mY, mZ;
};
#endif