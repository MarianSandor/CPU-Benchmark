#pragma once
#include "wx/wx.h"
#include "Main.h"

class GUI : public wxApp
{
public:
	GUI();
	~GUI();

private:
	Main *m_frame = nullptr;

public:
	bool render_loop_on;
	virtual bool OnInit();
	void onIdle(wxIdleEvent& evt);
	void activateRenderLoop(bool on);
};

