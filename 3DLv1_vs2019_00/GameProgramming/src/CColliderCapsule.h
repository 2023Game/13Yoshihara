#ifndef CCOLLIDERCAPSULE_H
#define CCOLLIDERCAPSULE_H
#include "CCollider.h"

//カプセルコライダクラス
class CColliderCapsule : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="radius">直径</param>
	CColliderCapsule(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	/// <summary>
	/// カプセルコライダの設定
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="radius">直径</param>
	void Set(CCharacter3* parent, CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	void Render();//コライダの描画
	void Update();//座標の更新
	void ChangePriority();//優先順位の更新
private:
	CVector mSp;//始点
	CVector mEp;//終点
};
#endif