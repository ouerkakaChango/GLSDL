#include "Action.h"
#include "Utility.h"

void Action::Check(float time)
{
	if (!bDone_ && time>time_)
	{
		DoAction();
		bDone_ = true;
	}
}