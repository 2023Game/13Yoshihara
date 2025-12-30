#include "CSwitchButton.h"
#include "CSwitch.h"
#include "CModel.h"
#include "CWeight.h"
#include "CPhysicsManager.h"
#include "CollisionData.h"

// 下がる距離
constexpr float PUSH_POS_Y = 0.5f;
// 上下する速度
constexpr float MOVE_SPEED = 0.1f;
// 押しているかのコライダーの半径
constexpr float COL_RADIUS = 5.0f;

// 物理設定
const CVector HALF_EXTENTS = CVector(5.0f, 1.0f, 5.0f);

// コンストラクタ
CSwitchButton::CSwitchButton(CVector pos, CSwitch* owner, bool isAttach)
	: CObjectBase(ETag::eSwitch, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mState(EState::eDefault)
	, mDefaultY(pos.Y())
	, mpOwner(owner)
	, mIsAttach(isAttach)
{
	mpModel = CResourceManager::Get<CModel>("SwitchButton");
	// 座標を設定
	Position(pos);
	// コライダーを生成
	CreateCol();
}

// デストラクタ
CSwitchButton::~CSwitchButton()
{
}

// 更新
void CSwitchButton::Update()
{
	switch (mState)
	{
	case EState::eDefault:	UpdateDefault();	break;
	case EState::ePush:		UpdatePush();		break;
	}

	ChangeState(EState::eDefault);
}

// 描画
void CSwitchButton::Render()
{
	mpModel->Render(Matrix());
}

// コライダーを生成
void CSwitchButton::CreateCol()
{
	CPhysicsManager::Instance()->CreateBoxSensor(
		this,
		HALF_EXTENTS,
		ELayer::eSwitch,
		{ ELayer::ePlayer,ELayer::eObject,ELayer::eConnectObj }
	);
}

void CSwitchButton::OnCollision(const CollisionData& data)
{
	// 押しているかの判定コライダーとの衝突判定
	if (data.selfBody == GetSensor())
	{
		ChangeState(EState::ePush);

		// 重りを貼り付ける
		if (mIsAttach)
		{
			// 重りなら
			CWeight* weight = static_cast<CWeight*>(data.otherBody->getUserPointer());

			if (weight)
			{
				// 重りを貼り付ける
				CVector pos = Position();
				pos.Y(weight->Position().Y());
				weight->Position(pos);
				// 重りが張り付いている
				weight->SetIsAttach(true);
			}
		}
	}
}
// 状態を変更
void CSwitchButton::ChangeState(EState state)
{
	if (mState != state) mState = state;
}

// 押されていない時の更新処理
void CSwitchButton::UpdateDefault()
{
	// 押されていない
	mpOwner->SetOnOff(false);

	// 初期のY座標まで上昇していく
	if (Position().Y() < mDefaultY)
	{
		CVector pos = Position();
		pos.Y(pos.Y() + MOVE_SPEED);
		Position(pos);
	}
	// 以上なら初期のY座標に設定
	else
	{
		CVector pos = Position();
		pos.Y(mDefaultY);
		Position(pos);
	}
}

// 押されている時の更新処理
void CSwitchButton::UpdatePush()
{
	// 押されている
	mpOwner->SetOnOff(true);

	// 押されている状態のY座標まで下降していく
	if (Position().Y() > mDefaultY - PUSH_POS_Y)
	{
		CVector pos = Position();
		pos.Y(pos.Y() - MOVE_SPEED);
		Position(pos);
	}
	// 以下なら押されている状態のY座標に設定
	else
	{
		CVector pos = Position();
		pos.Y(mDefaultY - PUSH_POS_Y);
		Position(pos);
	}
}
