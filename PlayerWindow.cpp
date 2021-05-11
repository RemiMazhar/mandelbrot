#include "PlayerWindow.h"

wxBEGIN_EVENT_TABLE(PlayerWindow, wxWindow)
EVT_BUTTON(PLAY_BUTTON_ID, onPlay)
wxEND_EVENT_TABLE()

PlayerWindow::PlayerWindow(wxWindow* parent) : wxWindow(parent, wxID_ANY)
{
	this->SetBackgroundColour(*wxWHITE);
	mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mainSizer);

	wxStaticText* videoLabel = new wxStaticText(this, wxID_ANY, "video to play:");
	mainSizer->Add(videoLabel, 0, wxALL, 10);

	videoInput = new wxFilePickerCtrl(this, TOPLAY_ID, "");
	mainSizer->Add(videoInput, 0, wxEXPAND);

	mainSizer->AddSpacer(10);

	playButton = new wxButton(this, PLAY_BUTTON_ID, "PLAY");
	mainSizer->Add(playButton, 0, wxALIGN_CENTER_HORIZONTAL, 10);
}

void PlayerWindow::onPlay(wxCommandEvent& evt)
{
	system("ffplay.exe \"" + videoInput->GetPath() + "\"");
	evt.Skip();
}

PlayerWindow::~PlayerWindow()
{
}