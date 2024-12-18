#pragma once
#include "CCollider.h"

/// <summary>
/// ボックスコライダ
/// </summary>
class CColliderBox : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="min">最小座標</param>
	/// <param name="max">最大座標</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダ―の重量</param>
	CColliderBox(CObjectBase* owner, ELayer layer,
		CVector min, CVector max,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// ボックスコライダを構成する頂点の設定
	/// </summary>
	/// <param name="owner">コライダ―の持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="min">最小座標</param>
	/// <param name="max">最大座標</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダ―の重量</param>
	void Set(CObjectBase* owner, ELayer layer,
		CVector min, CVector max);

	/// <summary>
	/// ボックスの頂点を取得
	/// </summary>
	/// <param name="v0">前面　右上</param>
	/// <param name="v1">前面　左上</param>
	/// <param name="v2">前面　左下</param>
	/// <param name="v3">前面　右下</param>
	/// <param name="v4">背面　右上</param>
	/// <param name="v5">背面　左上</param>
	/// <param name="v6">背面　左下</param>
	/// <param name="v7">背面　右下</param>
	void Get(CVector* v0, CVector* v1, CVector* v2, CVector* v3,
		CVector* v4, CVector* v5, CVector* v6, CVector* v7) const;

	/// <summary>
	/// ボックスを構成する四角形コライダの頂点を取得
	/// </summary>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <param name="v3"></param>
	/// <param name="num"></param>
	void GetRectangle(CVector* v0, CVector* v1, CVector* v2, CVector* v3,
		int num);

	//コライダ―描画
	void Render() override;

protected:
	// コライダ―の情報を更新
	void UpdateCol() override;

private:
	CVector mMin;	// 最小座標
	CVector mMax;	// 最大座標
	CVector mV[8];	// ボックスの頂点の配列
	CVector mWV[8];	// ボックスの頂点のワールド座標
};