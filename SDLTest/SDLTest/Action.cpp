#include "Action.h"
#include "Utility.h"

void Action::Check(float time)
{
	if (!bDone_ && Equals(time, time_, errorAccept_))
	{
		DoAction();
		bDone_ = true;
	}
}