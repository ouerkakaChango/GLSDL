#include "EndAction.h"

#include "God.h"

#include "Debug.h"

EndAction::EndAction()
{
}


EndAction::~EndAction()
{
}

void EndAction::DoAction()
{
	GOD.sceneManager_.ToNext();
}