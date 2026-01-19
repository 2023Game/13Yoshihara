#include "CSwitchFrame.h"
#include "CSwitch.h"
#include "CModel.h"

// コンストラクタ
CSwitchFrame::CSwitchFrame(const CVector& pos, const CVector& scale, CSwitch* owner)
	: CObjectBase(ETag::eSwitch, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
{
	mpModel = CResourceManager::Get<CModel>("SwitchFrame");
	// 座標設定
	Position(pos);
	Scale(scale);
}

// デストラクタ
CSwitchFrame::~CSwitchFrame()
{
}

// 描画
void CSwitchFrame::Render()
{
	mpModel->Render(Matrix());
}
