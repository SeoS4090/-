#pragma once
#include "PositionCommand.h"
class MouseCommand : public PositionCommand
{
private :

public:

	MouseCommand(POINT _pt);
	virtual bool Execute(CMFCBitMapView * View);
	virtual bool Redo(int command_key);
	virtual bool Undo(int command_key);
	~MouseCommand();
};

