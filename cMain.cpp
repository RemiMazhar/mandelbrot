#include "cMain.h"

// ids
#define CHOICE_ID 10001

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
// syntax: EVT_BUTTON(10001, OnBtn1Clicked)
EVT_CHOICE(CHOICE_ID, modeChanged)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "some name here", wxPoint(30, 30), wxSize(765, 562))
{
	mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);

	// choice widget to select the mode (explore/video/single image)
	const wxString choices[NB_MODES] = { wxT("explore mandelbrot"), wxT("explore julia"), wxT("watch video") };
	modeSelection = new wxChoice(this, CHOICE_ID, wxDefaultPosition, wxDefaultSize, NB_MODES, choices);
	mainSizer->Add(modeSelection, 0, wxEXPAND);

	explorerWindow = new ExplorerWindow(this);
	mainSizer->Add(explorerWindow, 1, wxEXPAND);

	playerWindow = new PlayerWindow(this);
	mainSizer->Add(playerWindow, 1, wxEXPAND);

	juliaWindow = new JuliaWindow(explorerWindow, this);
	mainSizer->Add(juliaWindow, 1, wxEXPAND);

	modeSelection->SetSelection(0);
	wxCommandEvent a;
	modeChanged(a);
}

void cMain::modeChanged(wxCommandEvent& evt)
{
	mode = modeSelection->GetSelection();
	switch (mode)
	{
	case exploreMandelbrot:
		explorerWindow->Show(true);
		playerWindow->Show(false);
		juliaWindow->Show(false);
		if (!IsMaximized())
		{
			SetSize(wxSize(765, 562));
		}
		break;
	case watch:
		explorerWindow->Show(false);
		playerWindow->Show(true);
		juliaWindow->Show(false);
		if (!IsMaximized())
		{
			SetSize(wxSize(300, 562));
		}
		break;
	case julia:

		explorerWindow->Show(false);
		playerWindow->Show(false);
		juliaWindow->Show(true);
		if (!IsMaximized())
		{
			SetSize(wxSize(765, 562));
		}
		juliaWindow->updateJuliaC();
		break;
	}
	evt.Skip();
}

cMain::~cMain()
{
}