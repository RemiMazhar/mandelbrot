#include "ExplorerWindow.h"

// ids
#define CHOICE_ID 10001
#define WIDTH_ID 10002
#define HEIGHT_ID 10003
#define QUALITY_ID 10004
#define ITERS_ID 10005
#define SENSIVITY_ID 10006
#define X_ID 10007
#define Y_ID 10008
#define ZOOM_ID 10009
#define EXPLORER_ID 10010
#define COLORMAP_ID 10011
#define KERNEL_ID 10012
#define COLOR_FREQ_ID 10013

wxBEGIN_EVENT_TABLE(ExplorerWindow, wxWindow)
// syntax: EVT_BUTTON(10001, OnBtn1Clicked)
EVT_SPINCTRL(QUALITY_ID, qualityChanged)
EVT_SPINCTRL(ITERS_ID, itersChanged)
EVT_SPINCTRLDOUBLE(SENSIVITY_ID, sensivityChanged)
EVT_SPINCTRLDOUBLE(X_ID, coordsChanged)
EVT_SPINCTRLDOUBLE(Y_ID, coordsChanged)
EVT_SPINCTRLDOUBLE(ZOOM_ID, zoomChanged)
EVT_SPINCTRLDOUBLE(COLOR_FREQ_ID, colorFrequencyChanged)
EVT_FILEPICKER_CHANGED(COLORMAP_ID, colormapChanged)
EVT_FILEPICKER_CHANGED(KERNEL_ID, kernelChanged)
EVT_COMMAND(EXPLORER_ID, EVT_RENDER, updateDisplays)
EVT_KEY_UP(onKeyPressed)
wxEND_EVENT_TABLE()

ExplorerWindow::ExplorerWindow(wxWindow* parent) : wxWindow(parent, wxID_ANY)
{
	this->SetBackgroundColour(*wxWHITE);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(mainSizer);
	
	leftPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(250, 0));
	leftSizer = new wxBoxSizer(wxVERTICAL);

	leftSizer->AddSpacer(10);

	// display for the width of the explorer
	wxBoxSizer* winWidthSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* winWidthLabel = new wxStaticText(leftPanel, wxID_ANY, "window width: ");
	winWidthSizer->Add(winWidthLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	winWidthDisplay = new wxStaticText(leftPanel, WIDTH_ID, "1920");
	winWidthSizer->Add(winWidthDisplay, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	leftSizer->Add(winWidthSizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	// display for the height of the explorer
	wxBoxSizer* winHeightSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* winHeightLabel = new wxStaticText(leftPanel, wxID_ANY, "window height: ");
	winHeightSizer->Add(winHeightLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	winHeightDisplay = new wxStaticText(leftPanel, WIDTH_ID, "1080");
	winHeightSizer->Add(winHeightDisplay, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	leftSizer->Add(winHeightSizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	// input for the quality (refer to [whatever documentation or something like that that i may do in the future] for details)
	wxBoxSizer* qualitySizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* qualityLabel = new wxStaticText(leftPanel, wxID_ANY, "quality: ");
	qualitySizer->Add(qualityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	qualityInput = new wxSpinCtrl(leftPanel, QUALITY_ID, "1");
	qualitySizer->Add(qualityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	leftSizer->Add(qualitySizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	// input for the number of iterations
	wxBoxSizer* itersSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* itersLabel = new wxStaticText(leftPanel, wxID_ANY, "number of iterations: ");
	itersSizer->Add(itersLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	itersInput = new wxSpinCtrl(leftPanel, ITERS_ID, "1000", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 100000000);
	itersSizer->Add(itersInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	leftSizer->Add(itersSizer, 0, wxEXPAND);

	// input for the mouse sensivity (when zooming)
	sensivitySizer = new wxBoxSizer(wxVERTICAL);
	sensivitySizer->AddSpacer(10);
	sensivitySizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	sensivitySizer->AddSpacer(10);
	wxBoxSizer* sensivityHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* sensivityLabel = new wxStaticText(leftPanel, wxID_ANY, "zoom sensivity: ");
	sensivityHSizer->Add(sensivityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	sensivityInput = new wxSpinCtrlDouble(leftPanel, SENSIVITY_ID, "1.2", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 5, 1.2, 0.05);
	sensivityHSizer->Add(sensivityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sensivitySizer->Add(sensivityHSizer, 0, wxEXPAND);
	leftSizer->Add(sensivitySizer, 0, wxEXPAND);

	// input/display for the x coordinate of the center of the image
	xCoordSizer = new wxBoxSizer(wxVERTICAL);
	xCoordSizer->AddSpacer(10);
	xCoordSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	xCoordSizer->AddSpacer(10);
	wxBoxSizer* xCoordHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* xCoordLabel = new wxStaticText(leftPanel, wxID_ANY, "x coordinate: ");
	xCoordHSizer->Add(xCoordLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	xCoordInput = new wxSpinCtrlDouble(leftPanel, X_ID, "0", wxDefaultPosition, wxSize(140, wxDefaultSize.y), wxSP_ARROW_KEYS, -10, 10, 0, 1e-15);
	xCoordHSizer->Add(xCoordInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	xCoordSizer->Add(xCoordHSizer, 0, wxEXPAND);
	leftSizer->Add(xCoordSizer, 0, wxEXPAND);

	// input/display for the y coordinate of the center of the image
	yCoordSizer = new wxBoxSizer(wxVERTICAL);
	yCoordSizer->AddSpacer(10);
	yCoordSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	yCoordSizer->AddSpacer(10);
	wxBoxSizer* yCoordHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* yCoordLabel = new wxStaticText(leftPanel, wxID_ANY, "y coordinate: ");
	yCoordHSizer->Add(yCoordLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	yCoordInput = new wxSpinCtrlDouble(leftPanel, Y_ID, "0", wxDefaultPosition, wxSize(140, wxDefaultSize.y), wxSP_ARROW_KEYS, -10, 10, 0, 1e-15);
	yCoordHSizer->Add(yCoordInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	yCoordSizer->Add(yCoordHSizer, 0, wxEXPAND);
	leftSizer->Add(yCoordSizer, 0, wxEXPAND);

	// input/display for the level of zoom
	zoomSizer = new wxBoxSizer(wxVERTICAL);
	zoomSizer->AddSpacer(10);
	zoomSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	zoomSizer->AddSpacer(10);
	wxBoxSizer* zoomHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* zoomLabel = new wxStaticText(leftPanel, wxID_ANY, "zoom: ");
	zoomHSizer->Add(zoomLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	zoomInput = new wxSpinCtrlDouble(leftPanel, ZOOM_ID, "0.25", wxDefaultPosition, wxSize(140, wxDefaultSize.y), wxSP_ARROW_KEYS, -0.01, 1e20, 0.25, 1e-2);
	zoomHSizer->Add(zoomInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	zoomSizer->Add(zoomHSizer, 0, wxEXPAND);
	leftSizer->Add(zoomSizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	// input for the color frequency
	colorFrequencySizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* colorFrequencyHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* colorFrequencyLabel = new wxStaticText(leftPanel, wxID_ANY, "color frequency: ");
	colorFrequencyHSizer->Add(colorFrequencyLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	colorFrequencyInput = new wxSpinCtrlDouble(leftPanel, COLOR_FREQ_ID, "3", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, -30, 30, 1, 1e-1);
	colorFrequencyHSizer->Add(colorFrequencyInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	colorFrequencySizer->Add(colorFrequencyHSizer, 0, wxEXPAND);
	leftSizer->Add(colorFrequencySizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	wxStaticText* colormapLabel = new wxStaticText(leftPanel, wxID_ANY, "colormap to use: ");
	leftSizer->Add(colormapLabel, 0, wxLEFT, 10);
	leftSizer->AddSpacer(5);
	colormapSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* colormapHSizer = new wxBoxSizer(wxHORIZONTAL);
	colormapInput = new wxFilePickerCtrl(leftPanel, COLORMAP_ID, DEFAULT_COLORMAP);
	colormapHSizer->Add(colormapInput, 1, wxLEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	colormapSizer->Add(colormapHSizer, 0, wxEXPAND);
	leftSizer->Add(colormapSizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	wxStaticText* kernelLabel = new wxStaticText(leftPanel, wxID_ANY, "kernel to use: ");
	leftSizer->Add(kernelLabel, 0, wxLEFT, 10);
	leftSizer->AddSpacer(5);
	kernelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* kernelHSizer = new wxBoxSizer(wxHORIZONTAL);
	kernelInput = new wxFilePickerCtrl(leftPanel, KERNEL_ID, DEFAULT_KERNEL);
	kernelHSizer->Add(kernelInput, 1, wxLEFT | wxALIGN_CENTER_VERTICAL | wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	kernelSizer->Add(kernelHSizer, 0, wxEXPAND);
	leftSizer->Add(kernelSizer, 0, wxEXPAND);

	leftSizer->AddSpacer(10);
	leftSizer->Add(new wxStaticLine(leftPanel, wxID_ANY), 0, wxEXPAND);
	leftSizer->AddSpacer(10);

	leftPanel->SetSizer(leftSizer);
	leftPanel->SetScrollRate(5, 5);
	mainSizer->Add(leftPanel, 0, wxEXPAND);

	rightSizer = new wxBoxSizer(wxVERTICAL);
	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	explorer = new ExplorerGL(this, EXPLORER_ID, args);
	rightSizer->Add(explorer, 1, wxEXPAND);
	mainSizer->Add(rightSizer, 1, wxEXPAND);
}

void ExplorerWindow::qualityChanged(wxSpinEvent& evt)
{
	explorer->setQuality(qualityInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::itersChanged(wxSpinEvent& evt)
{
	explorer->setIterations(itersInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::sensivityChanged(wxSpinDoubleEvent& evt)
{
	explorer->setZoomSensivity(sensivityInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::coordsChanged(wxSpinDoubleEvent& evt)
{
	explorer->setCenter(xCoordInput->GetValue(), yCoordInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::zoomChanged(wxSpinDoubleEvent& evt)
{
	explorer->setZoom(zoomInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::colorFrequencyChanged(wxSpinDoubleEvent& evt)
{
	explorer->setColorFrequency(colorFrequencyInput->GetValue());
	evt.Skip();
}

void ExplorerWindow::colormapChanged(wxFileDirPickerEvent& evt)
{
	explorer->setColormap(evt.GetPath());
	evt.Skip();
}
void ExplorerWindow::kernelChanged(wxFileDirPickerEvent& evt)
{
	explorer->setKernel(evt.GetPath());
	evt.Skip();
}

void ExplorerWindow::updateDisplays(wxCommandEvent& evt)
{
	xCoordInput->SetValue(explorer->getCenterX());
	yCoordInput->SetValue(explorer->getCenterY());
	itersInput->SetValue(explorer->getIterations());
	zoomInput->SetValue(explorer->getZoom());
	winWidthDisplay->SetLabelText(wxString::Format(wxT("%d"), explorer->getWidth()));
	winHeightDisplay->SetLabelText(wxString::Format(wxT("%d"), explorer->getHeight()));
	wxWindow::SetFocus();
	evt.Skip();
}

void ExplorerWindow::onKeyPressed(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_F1)
	{
		leftPanel->Show(!leftPanel->IsShown());
		wxWindow::Layout();
	}
	evt.Skip();
}

ExplorerWindow::~ExplorerWindow()
{
}