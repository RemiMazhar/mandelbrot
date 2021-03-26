#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include "ExplorerGL.h"

typedef unsigned char uchar;

class ExplorerWindow: public wxWindow
{
public:
	ExplorerWindow(wxWindow* parent = nullptr);
	~ExplorerWindow();

private:
	void qualityChanged(wxSpinEvent& evt);
	void itersChanged(wxSpinEvent& evt);
	void sensivityChanged(wxSpinDoubleEvent& evt);
	void coordsChanged(wxSpinDoubleEvent& evt);
	void zoomChanged(wxSpinDoubleEvent& evt);
	void colorFrequencyChanged(wxSpinDoubleEvent& evt);
	void colormapChanged(wxFileDirPickerEvent& evt);
	void kernelChanged(wxFileDirPickerEvent& evt);
	void updateDisplays(wxCommandEvent& evt);

public:
	int mode;

	// the main sizers (leftSizer and rightSizer are in the main sizer)
	wxBoxSizer* mainSizer;
	wxBoxSizer* leftSizer;
	wxBoxSizer* rightSizer;

	// controls
	wxStaticText* winWidthDisplay; // display the width of the window
	wxStaticText* winHeightDisplay; // display the height of the window
	wxSpinCtrl* qualityInput;
	wxSpinCtrl* itersInput;
	wxSpinCtrlDouble* sensivityInput;
	wxSpinCtrlDouble* xCoordInput;
	wxSpinCtrlDouble* yCoordInput;
	wxSpinCtrlDouble* zoomInput;
	wxSpinCtrlDouble* colorFrequencyInput;
	wxFilePickerCtrl* colormapInput;
	wxFilePickerCtrl* kernelInput;

	// sizer for each control, these sizers are added to leftSizer (the left side is a control panel)
	// each control needs a horizontal sizer because there is a control and a label on the same line
	wxBoxSizer* sensivitySizer;
	wxBoxSizer* xCoordSizer;
	wxBoxSizer* yCoordSizer;
	wxBoxSizer* zoomSizer;
	wxBoxSizer* colorFrequencySizer;
	wxBoxSizer* colormapSizer;
	wxBoxSizer* kernelSizer;

	ExplorerGL* explorer;
	wxBoxSizer* explorerSizer;

	wxDECLARE_EVENT_TABLE();
};