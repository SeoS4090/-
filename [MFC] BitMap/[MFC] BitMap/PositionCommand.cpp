#include "stdafx.h"
#include "PositionCommand.h"


PositionCommand::PositionCommand()
{
}

POINT PositionCommand::GetPt()
{
	return pt;
}
void PositionCommand::SetPt(POINT _pt)
{
	pt.x = _pt.x;
	pt.y = _pt.y;
}

PositionCommand::~PositionCommand()
{
}
