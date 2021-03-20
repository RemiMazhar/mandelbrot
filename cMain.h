#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include "ExplorerWindow.h"

enum modes { exploreMandelbrot, generateVideo, generateImage, NB_MODES };
typedef unsigned char uchar;

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:
	bool exploring = false;
	ExplorerWindow explorerWindow;

	void modeChanged(wxCommandEvent &evt);
	void widthChanged(wxSpinEvent &evt);
	void heightChanged(wxSpinEvent &evt);
	void qualityChanged(wxSpinEvent &evt);
	void itersChanged(wxSpinEvent &evt);
	void sensivityChanged(wxSpinDoubleEvent &evt);
	void xChanged(wxSpinDoubleEvent &evt);
	void yChanged(wxSpinDoubleEvent &evt);
	void onGenerateBtnClick(wxCommandEvent &evt);
	void onIdle(wxIdleEvent &evt);
	void updateDisplays();

public:
	int mode;
	wxBoxSizer *sizer;
	wxChoice *modeSelection;
	wxSpinCtrl *winWidthInput;
	wxSpinCtrl *winHeightInput;
	wxSpinCtrl *qualityInput;
	wxSpinCtrl *itersInput;
	wxSpinCtrl *nbFramesInput;
	wxSpinCtrl *fpsInput;
	wxSpinCtrlDouble *sensivityInput;
	wxSpinCtrlDouble *xCoordInput;
	wxSpinCtrlDouble *yCoordInput;
	wxTextCtrl *vidNameInput;
	wxButton *generateButton;
	wxStaticText *itersLabel;
	wxBoxSizer *sensivitySizer;
	wxBoxSizer *nbFramesSizer;
	wxBoxSizer *xCoordSizer;
	wxBoxSizer *yCoordSizer;
	wxBoxSizer *fpsSizer;
	wxBoxSizer *vidNameSizer;

	wxDECLARE_EVENT_TABLE();
};