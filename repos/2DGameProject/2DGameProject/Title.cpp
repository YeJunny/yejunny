#include "GameLib/Framework.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Input/Manager.h"
#include "Image.h"
#include "Title.h"
#include "Vector2.h"

using namespace GameLib;

Title::Title(const char* imageName)
{
	mTitle = new Image(imageName);
}

Title::~Title()
{
	delete mTitle;
}

void Title::Update(Sequence* parent)
{
	mTitle->Draw(0, 0, 0, 0, mTitle->GetWidth(), mTitle->GetHeight());

	if (Input::Manager::instance().keyboard().isTriggered(' '))
	{
		parent->SetSeq(Sequence::ESeq::SELECT);
		return;
	}

	parent->SetSeq(Sequence::ESeq::NONE);
}