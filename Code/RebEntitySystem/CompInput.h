#ifndef COMPINPUT_H
#define COMPINPUT_H

#include "RebEntity.h"
#include "..\RebSupport\RebTimer.h"
#include <map>

class CompInput : public Component
{
public:
	std::string GetType()
	{
		return "CompInput";
	}
};

class CompInpBasicControl : public CompInput, public IKeyListener, public IMouseListener
{
	IMEH * eh;
	float x, y;
	RebTimer timer;
	std::map<int, bool> keypressedmap;
public:

	CompInpBasicControl(IMEH * seh);

	std::string GetID()
	{
		return "CompInpBasicControl";
	}

	void onKeyEvent(RebEvent keyevent);

	void onMMotionEvent(RebEvent mev);

	void update();

	~CompInpBasicControl();
};

class TCompInpBasicControl : public TComponent
{
	IMEH * eh;
public:
	TCompInpBasicControl();

	std::string GetID()
	{
		return "TCompInpBasicControl";
	}

	std::string GetType()
	{
		return "CompInput";
	}

	Component * MakeComponent(RebGDC * data);

};


#endif