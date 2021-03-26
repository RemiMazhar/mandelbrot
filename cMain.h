#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include "explorerWindow.h"


enum modes { exploreMandelbrot, generateVideo, generateImage, NB_MODES };
typedef unsigned char uchar;

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	void modeChanged(wxCommandEvent& evt);

	ExplorerWindow* explorerWindow;

public:
	int mode;

	wxBoxSizer* mainSizer;

	// controls
	wxChoice* modeSelection;

	wxDECLARE_EVENT_TABLE();
};