#pragma once
#include "PositionCommand.h"
class KeyBoardCommand: public PositionCommand
{
public:
	KeyBoardCommand(POINT _pt);
	virtual bool Execute(CMFCBitMapView * View);
	virtual bool Redo(int command_key);
	virtual bool Undo(int command_key);
	~KeyBoardCommand();
};

