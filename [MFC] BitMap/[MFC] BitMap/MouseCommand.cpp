#include "stdafx.h"
#include "MouseCommand.h"
#include "[MFC] BitMapView.h"

MouseCommand::MouseCommand(POINT _pt)
{
	pt.x = _pt.x;
	pt.y = _pt.y;
}


bool MouseCommand::Execute(CMFCBitMapView * View)
{
	View->SetPoint(pt);
	return true;
}

bool MouseCommand::Redo(int command_key)
{
	return false;
}

bool MouseCommand::Undo(int command_key)
{
	return false;
}


MouseCommand::~MouseCommand()
{
}
