#pragma once
#include "Command.h"
class PositionCommand : public Command
{
protected:
	POINT pt;
public:
	PositionCommand();
	virtual bool Execute(CMFCBitMapView * View) = 0;
	virtual bool Redo(int command_key) = 0;
	virtual bool Undo(int command_key) = 0;

	POINT GetPt();
	void SetPt(POINT pt);
	~PositionCommand();
};

