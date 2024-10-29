#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

// TODO:í—Ş•Ê‚ÌƒQ[ƒ€ƒƒjƒ…[‚É‘Î‰‚³‚¹‚é
class CGameMenu : public CTask
{
public:
	CGameMenu();
	~CGameMenu();

	void Open();
	void Close();
	bool IsOpened() const;

	void Decide(int select);

	void Update() override;
	void Render() override;

private:
	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	int mSelectIndex;
	bool mIsOpened;
};