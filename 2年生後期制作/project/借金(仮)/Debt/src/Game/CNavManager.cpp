#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include <assert.h>

CNavManager* CNavManager::spInstance = nullptr;

// Œo˜HŠÇ—ƒNƒ‰ƒX‚ÌƒCƒ“ƒXƒ^ƒ“ƒX‚ðŽæ“¾
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// ƒfƒXƒgƒ‰ƒNƒ^
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	// ’Tõƒm[ƒh‚ð‘S‚Äíœ
	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// Œo˜H’Tõ—p‚Ìƒm[ƒh‚ð’Ç‰Á
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// Œo˜H’Tõ—p‚Ìƒm[ƒh‚ðŽæ‚èœ‚­
void CNavManager::RemoveNode(CNavNode* node)
{
	mNodes.remove(node);
}

// ‘S‚Ä‚Ìƒm[ƒh‚ÆŒo˜H‚ð•`‰æ
void CNavManager::Render()
{
	// NƒL[‚ÅŒo˜H’Tõƒm[ƒh‚Ì•`‰æƒ‚[ƒh‚ðØ‚è‘Ö‚¦
	if (CInput::PushKey('N'))
	{
		mIsRender = !mIsRender;
	}

	// Œo˜H’Tõƒm[ƒh‚ð•`‰æ‚µ‚È‚¢‚È‚ç‚ÎAˆÈ~ˆ—‚µ‚È‚¢
	if (!mIsRender) return;

	// ƒŠƒXƒg“à‚Ìƒm[ƒh‚ð‘S‚Ä•`‰æ
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}
}