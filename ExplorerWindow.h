#pragma once
#include "wx/wx.h"
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/filepicker.h>
#include <wx/splitter.h>
#include <wx/collpane.h>
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
	void onKeyPressed(wxKeyEvent& evt);
	void saveImg(wxCommandEvent& evt);
	void displayProgress(wxCommandEvent& evt);
	void onRenderResized(wxCollapsiblePaneEvent& evt);

public:
	// the main sizers (leftSizer and rightSizer are in the main sizer)
	wxBoxSizer* mainSizer;
	wxBoxSizer* leftSizer;
	wxBoxSizer* rightSizer;

	// controls
	wxStaticText* winWidthDisplay; // display the width of the window
	wxStaticText* winHeightDisplay; // display the height of the window
	wxStaticText* fpsDisplay;
	wxStaticText* progressDisplay;
	wxSpinCtrl* qualityInput;
	wxSpinCtrl* itersInput;
	wxSpinCtrlDouble* sensivityInput;
	wxSpinCtrlDouble* xCoordInput;
	wxSpinCtrlDouble* yCoordInput;
	wxSpinCtrlDouble* zoomInput;
	wxSpinCtrlDouble* colorFrequencyInput;
	wxFilePickerCtrl* colormapInput;
	wxFilePickerCtrl* kernelInput;
	wxSpinCtrl* rdrWidthInput;
	wxSpinCtrl* rdrHeightInput;
	wxSpinCtrl* rdrItersInput;
	wxSpinCtrl* rdrQualityInput;
	wxSpinCtrl* rdrFragmentsInput;
	wxButton* saveButton;
	wxBoxSizer* kernelSizer;
	wxCollapsiblePane* renderPanel;

	ExplorerGL* explorer;
	wxBoxSizer* explorerSizer;
	wxScrolledWindow* leftPanel;

	wxDECLARE_EVENT_TABLE();
};