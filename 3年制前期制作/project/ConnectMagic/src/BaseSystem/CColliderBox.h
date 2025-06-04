#pragma once
#include <list>
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
	// デストラクタ
	~CColliderBox();

	/// <summary>
	/// ボックスコライダを構成する頂点の設定
	/// </summary>
	/// <param name="min">最小座標</param>
	/// <param name="max">最大座標</param>
	void Set(CVector min, CVector max);
	// 四角形コライダの配列を取得
	const std::vector<SRVertexData>& Get() const;

	//コライダ―描画
	void Render() override;

protected:
	// コライダ―の情報を更新
	void UpdateCol() override;

private:
	// 四角形コライダの配列作成
	std::vector<SRVertexData> mVertices;
};