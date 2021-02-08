#include "GUI.h"

GUI::GUI()
{
}

GUI::~GUI()
{
}

bool GUI::OnInit()
{
    render_loop_on = false;

	m_frame = new Main();
	m_frame->Show();

    activateRenderLoop(true);

	return true;
}

void GUI::activateRenderLoop(bool on)
{
    if (on && !render_loop_on)
    {
        Connect(wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(GUI::onIdle));
        render_loop_on = true;
    }
    else if (!on && render_loop_on)
    {
        Disconnect(wxEVT_IDLE, wxIdleEventHandler(GUI::onIdle));
        render_loop_on = false;
    }
}

void GUI::onIdle(wxIdleEvent& evt)
{
    if (render_loop_on)
    {
        m_frame->updateMem();
        evt.RequestMore(); // render continuously, not only once on idle
    }
}