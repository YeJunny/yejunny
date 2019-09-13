#include "GameLib/Framework.h"
#include "Image.h"
#include "Title.h"

using namespace GameLib;

Title::Title(const char* imageName)
{
	mTitle = new Image(imageName);
	mTitle->Draw(0, 0, 0, 0, mTitle->GetWidth(), mTitle->GetHeight());
}

Title::~Title()
{
	delete mTitle;
}

void Title::Update(Sequence* parent)
{
	if (Framework::instance().isKeyTriggered(' '))
	{
		parent->SetSeq(Sequence::ESeq::SELECT);
		return;
	}

	parent->SetSeq(Sequence::ESeq::NONE);
}