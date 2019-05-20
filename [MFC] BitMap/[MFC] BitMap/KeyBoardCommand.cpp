#include "stdafx.h"
#include "KeyBoardCommand.h"
#include "[MFC] BitMapView.h"


KeyBoardCommand::KeyBoardCommand(POINT _pt)
{
	pt.x = _pt.x;
	pt.y = _pt.y;
}

bool KeyBoardCommand::Execute(CMFCBitMapView * View)
{
	View->SetPoint(pt);
	return true;
}

bool KeyBoardCommand::Redo(int command_key)
{
	return false;
}

bool KeyBoardCommand::Undo(int command_key)
{
	return false;
}

KeyBoardCommand::~KeyBoardCommand()
{
}
