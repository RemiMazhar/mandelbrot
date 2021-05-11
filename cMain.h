#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include "ExplorerWindow.h"
#include "JuliaWindow.h"
#include "PlayerWindow.h"


enum modes { exploreMandelbrot, julia, watch, NB_MODES };
typedef unsigned char uchar;

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	void modeChanged(wxCommandEvent& evt);

	ExplorerWindow* explorerWindow;
	JuliaWindow* juliaWindow;
	PlayerWindow* playerWindow;

public:
	int mode;

	wxBoxSizer* mainSizer;

	// controls
	wxChoice* modeSelection;

	wxDECLARE_EVENT_TABLE();
};