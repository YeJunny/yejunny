#include "GameLib/Framework.h"
#include "Image.h"
#include "SelectStage.h"

using namespace GameLib;

SelectStage::SelectStage(const char* imageName)
{
	mSelect = new Image(imageName);
	mSelect->Draw(0, 0, 0, 0, mSelect->GetWidth(), mSelect->GetHeight());
}

SelectStage::~SelectStage()
{
	delete mSelect;
}

void SelectStage::Update(Sequence* parent)
{
	int stage = 0;

	char number[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for (int i = 0; i < 10; i++)
	{
		if (Framework::instance().isKeyTriggered(number[i]))
		{
			stage = number[i];
		}
	}

	if (stage != 0)
	{
		parent->SetSeq(Sequence::ESeq::GAME);
		parent->SetStage(stage);
		return;
	}

	parent->SetSeq(Sequence::ESeq::NONE);
}