#pragma once
#include "CFire.h"
#include "CItemDrop.h"

class CImage3D;

// ツタの高さと幅
#define VINE_HEIGHT 50.0f
#define VINE_WIDTH	100.0f

// 草の種類
enum class EGrassType
{
	eGrass,		// 草
	eVine,		// ツタ
};

// 火とつなげると燃える草オブジェクト
class CGrass : public CFire, public CItemDrop
{
public:
	// コンストラクタ(初期値は通常の草）
	CGrass(EGrassType type = EGrassType::eGrass,
		CVector fireOffsetPos = CVector::zero,
		float fireScale = FIRE_SCALE);
	// デストラクタ
	~CGrass();

	// 更新
	void Update() override;

private:
	// コライダーを生成
	void CreateCol() override;

	// 燃えたときの処理
	void Burning() override;

	// 全て削除
	void Delete() override;

	// 画像を生成
	void CreateImg();

	// 草の画像
	CImage3D* mpGrassImage;

	// 草の種類
	EGrassType mGrassType;

	// ツタ用のコライダー
	CCollider* mpVineCol;
};