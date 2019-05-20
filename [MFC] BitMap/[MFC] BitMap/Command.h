#pragma once
class CMFCBitMapView;
class Command
{
public:
	Command();
	virtual bool Execute(CMFCBitMapView * View) = 0;
	virtual bool Redo(int command_key) = 0;
	virtual bool Undo(int command_key) = 0;
	~Command();
};

