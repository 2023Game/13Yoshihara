#include "CDeliveryField.h"
#include "CDeliveryHome.h"
#include "Maths.h"

#define HOME_OFFSET_POSX 250.0f
#define HOME_OFFSET_POSY 12.0f

// 強制移動速度
#define FORCE_MOVE_SPEED 7.5f * 60.0f	

// コンストラクタ
CDeliveryField::CDeliveryField()
	: CFieldBase()
	, mHomeOffsetPos(CVector::zero)
{
	// スケールを設定
	Scale(Scale() * SCALE_RATIO);
	mpModel = CResourceManager::Get<CModel>("DeliveryStage");
	mpGroundColliderMesh = new CColliderMesh
	(
		this, ELayer::eGround,
		CResourceManager::Get<CModel>
		(
			"DeliveryStage_Ground_Collision"
		),
		true
	);
	// フィールドオブジェクトを生成
	CreateFieldObjects();
	// 家の位置をランダムに決める
	RandomHome();
}

// デストラクタ
CDeliveryField::~CDeliveryField()
{
	SAFE_DELETE(mpGroundColliderMesh);
}

// 更新
void CDeliveryField::Update()
{
	float moveSpeed = FORCE_MOVE_SPEED * Times::DeltaTime();
	Position(Position() + CVector::forward * moveSpeed);
	mpHome->Position(Position() + mHomeOffsetPos);
}

// 描画
void CDeliveryField::Render()
{
	CFieldBase::Render();
}

// 家の位置をランダムに決める
void CDeliveryField::RandomHome()
{
	int random = Math::Rand(0, 1);
	if (random == 0)
	{
		mpHome->Rotation(0.0f, 90.0f, 0.0f);
		mHomeOffsetPos = CVector(HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
	}
	else
	{
		mpHome->Rotation(0.0f, -90.0f, 0.0f);
		mHomeOffsetPos = CVector(-HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
	}
	random = Math::Rand(-25, 25);
	mHomeOffsetPos.Z((float)random * 10.0f);
}

// フィールドのオブジェクト生成
void CDeliveryField::CreateFieldObjects()
{
	mpHome = new CDeliveryHome();
	mpHome->Rotation(0.0f, 90.0f, 0.0f);
	mHomeOffsetPos = CVector(HOME_OFFSET_POSX, HOME_OFFSET_POSY, 0.0f);
}