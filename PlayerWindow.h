#pragma once
#include "wx/wx.h"
#include <wx/filepicker.h>
#define PLAY_BUTTON_ID 20001
#define TOPLAY_ID 20002

class PlayerWindow :
    public wxWindow
{
public:
    PlayerWindow(wxWindow* parent = nullptr);
    ~PlayerWindow();

private:
    void onPlay(wxCommandEvent &evt);

public:
    wxBoxSizer* mainSizer;
    wxFilePickerCtrl* videoInput;
    wxButton* playButton;

    wxDECLARE_EVENT_TABLE();
};

