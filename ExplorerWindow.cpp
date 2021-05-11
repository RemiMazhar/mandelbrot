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
#define RENDER_ID 10014
#define SAVE_BTN_ID 10015
#define VIDEO_ID 10016
#define VIDEO_BTN_ID 10017
#define LOAD_BTN_ID 10018

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
EVT_COMMAND(EXPLORER_ID, EVT_FRAGMENT, displayFragsProgress)
EVT_COMMAND(EXPLORER_ID, EVT_FRAME, displayFramesProgress)
EVT_KEY_UP(onKeyPressed)
EVT_BUTTON(SAVE_BTN_ID, saveImg)
EVT_BUTTON(VIDEO_BTN_ID, generateVid) 
EVT_COLLAPSIBLEPANE_CHANGED(RENDER_ID, onRdrPaneResized)
EVT_COLLAPSIBLEPANE_CHANGED(VIDEO_ID, onVidPaneResized)
EVT_BUTTON(LOAD_BTN_ID, onLoadParamsFromFile)
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

	wxBoxSizer* fpsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* fpsLabel = new wxStaticText(leftPanel, wxID_ANY, "FPS: ");
	fpsSizer->Add(fpsLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	fpsDisplay = new wxStaticText(leftPanel, wxID_ANY, "0");
	fpsSizer->Add(fpsDisplay, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	leftSizer->Add(fpsSizer, 0, wxEXPAND);

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
	wxBoxSizer* sensivitySizer = new wxBoxSizer(wxVERTICAL);
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
	wxBoxSizer* xCoordSizer = new wxBoxSizer(wxVERTICAL);
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
	wxBoxSizer* yCoordSizer = new wxBoxSizer(wxVERTICAL);
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
	wxBoxSizer* zoomSizer = new wxBoxSizer(wxVERTICAL);
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
	wxBoxSizer* colorFrequencySizer = new wxBoxSizer(wxVERTICAL);
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
	wxBoxSizer* colormapSizer = new wxBoxSizer(wxVERTICAL);
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

	renderPanel = new wxCollapsiblePane(leftPanel, RENDER_ID, "render", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	wxBoxSizer* renderSizer = new wxBoxSizer(wxVERTICAL);
	renderPanel->GetPane()->SetSizer(renderSizer);
		
		wxBoxSizer* rdrWidthSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* rdrWidthLabel = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "width: ");
		rdrWidthSizer->Add(rdrWidthLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		rdrWidthInput = new wxSpinCtrl(renderPanel->GetPane(), wxID_ANY, "1920", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 500000);
		rdrWidthSizer->Add(rdrWidthInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		renderSizer->Add(rdrWidthSizer, 0, wxEXPAND);

		renderSizer->AddSpacer(10);

		wxBoxSizer* rdrHeightSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* rdrHeightLabel = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "height: ");
		rdrHeightSizer->Add(rdrHeightLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		rdrHeightInput = new wxSpinCtrl(renderPanel->GetPane(), wxID_ANY, "1080", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 500000);
		rdrHeightSizer->Add(rdrHeightInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		renderSizer->Add(rdrHeightSizer, 0, wxEXPAND);

		renderSizer->AddSpacer(10);

		wxBoxSizer* rdrItersSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* rdrItersLabel = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "number of iterations: ");
		rdrItersSizer->Add(rdrItersLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		rdrItersInput = new wxSpinCtrl(renderPanel->GetPane(), wxID_ANY, "1000", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 1e10);
		rdrItersSizer->Add(rdrItersInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		renderSizer->Add(rdrItersSizer, 0, wxEXPAND);

		renderSizer->AddSpacer(10);

		wxBoxSizer* rdrQualitySizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* rdrQualityLabel = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "quality: ");
		rdrQualitySizer->Add(rdrQualityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		rdrQualityInput = new wxSpinCtrl(renderPanel->GetPane(), wxID_ANY, "2");
		rdrQualitySizer->Add(rdrQualityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		renderSizer->Add(rdrQualitySizer, 0, wxEXPAND);

		renderSizer->AddSpacer(10);

		wxBoxSizer* rdrFragmentsSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* rdrFragmentsLabel = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "number of fragments: ");
		rdrFragmentsSizer->Add(rdrFragmentsLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		rdrFragmentsInput = new wxSpinCtrl(renderPanel->GetPane(), wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000);
		rdrFragmentsSizer->Add(rdrFragmentsInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		renderSizer->Add(rdrFragmentsSizer, 0, wxEXPAND);

		renderSizer->AddSpacer(10);

		saveButton = new wxButton(renderPanel->GetPane(), SAVE_BTN_ID, "SAVE");
		renderSizer->Add(saveButton, 0, wxALL | wxALIGN_CENTER);

		renderSizer->AddSpacer(10);

		rdrProgressDisplay = new wxStaticText(renderPanel->GetPane(), wxID_ANY, "not generating");
		renderSizer->Add(rdrProgressDisplay, 0, wxALL | wxALIGN_CENTER);

	videoPanel = new wxCollapsiblePane(leftPanel, VIDEO_ID, "video", wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	wxBoxSizer* videoSizer = new wxBoxSizer(wxVERTICAL);
	videoPanel->GetPane()->SetSizer(videoSizer);

		wxBoxSizer* vidWidthSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidWidthLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "width: ");
		vidWidthSizer->Add(vidWidthLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidWidthInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "1920", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 500000);
		vidWidthSizer->Add(vidWidthInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidWidthSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidHeightSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidHeightLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "height: ");
		vidHeightSizer->Add(vidHeightLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidHeightInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "1080", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 500000);
		vidHeightSizer->Add(vidHeightInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidHeightSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidItersSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidItersLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "iteration factor: ");
		vidItersSizer->Add(vidItersLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidItersInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "400", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 1e10);
		vidItersSizer->Add(vidItersInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidItersSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidQualitySizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidQualityLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "quality: ");
		vidQualitySizer->Add(vidQualityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidQualityInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "2");
		vidQualitySizer->Add(vidQualityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidQualitySizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidFragmentsSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidFragmentsLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "number of fragments: ");
		vidFragmentsSizer->Add(vidFragmentsLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidFragmentsInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "10", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000);
		vidFragmentsSizer->Add(vidFragmentsInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidFragmentsSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidFpsSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidFpsLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "number of FPS: ");
		vidFpsSizer->Add(vidFpsLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidFpsInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "60", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000);
		vidFpsSizer->Add(vidFpsInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidFpsSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidFirstFrameSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidFirstFrameLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "first frame index: ");
		vidFirstFrameSizer->Add(vidFirstFrameLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidFirstFrameInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 1e6);
		vidFirstFrameSizer->Add(vidFirstFrameInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidFirstFrameSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidFramesSizer = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText* vidFramesLabel = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "number of frames: ");
		vidFramesSizer->Add(vidFramesLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
		vidFramesInput = new wxSpinCtrl(videoPanel->GetPane(), wxID_ANY, "1000", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000000);
		vidFramesSizer->Add(vidFramesInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidFramesSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		wxBoxSizer* vidNameSizer = new wxBoxSizer(wxHORIZONTAL);
		vidNameInput = new wxTextCtrl(videoPanel->GetPane(), wxID_ANY, "video_name");
		vidNameSizer->Add(vidNameInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
		videoSizer->Add(vidNameSizer, 0, wxEXPAND);

		videoSizer->AddSpacer(10);

		videoButton = new wxButton(videoPanel->GetPane(), VIDEO_BTN_ID, "GENERATE VIDEO");
		videoSizer->Add(videoButton, 0, wxALL | wxALIGN_CENTER);

		videoSizer->AddSpacer(10);

		vidFramesProgressDisplay = new wxStaticText(videoPanel->GetPane(), wxID_ANY, "not generating");
		videoSizer->Add(vidFramesProgressDisplay, 0, wxALL | wxALIGN_CENTER);


	leftSizer->Add(renderPanel, 1, wxEXPAND);
	leftSizer->AddSpacer(10);
	leftSizer->Add(videoPanel, 1, wxEXPAND);

	leftSizer->AddSpacer(10);

	loadFromFileButton = new wxButton(leftPanel, LOAD_BTN_ID, "LOAD PARAMETERS FROM FILE");
	leftSizer->Add(loadFromFileButton, 0, wxALL | wxALIGN_CENTER);

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
	fpsDisplay->SetLabelText(wxString::Format(wxT("%d"), explorer->getFPS()));
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

void ExplorerWindow::saveImg(wxCommandEvent& evt)
{
	rdrProgressDisplay->SetLabelText(wxString::Format(wxT("starting to generate..."), evt.GetInt(), rdrFragmentsInput->GetValue()));
	if (rdrHeightInput->GetValue() % rdrFragmentsInput->GetValue() == 0 && rdrWidthInput->GetValue() % 20 == 0)
	{
		explorer->generateImg(
			rdrItersInput->GetValue(),
			rdrWidthInput->GetValue(),
			rdrHeightInput->GetValue(),
			rdrQualityInput->GetValue(),
			rdrFragmentsInput->GetValue()
		);
	}
	else
	{
		wxMessageBox("The number of fragments must be a divisor of the height and the width must be a multiple of 20 pixels");
	}
	evt.Skip();
}

void ExplorerWindow::generateVid(wxCommandEvent& evt)
{
	vidFramesProgressDisplay->SetLabelText(wxString::Format(wxT("starting to generate..."), evt.GetInt(), vidFragmentsInput->GetValue()));
	if (vidHeightInput->GetValue() % vidFragmentsInput->GetValue() == 0 && vidWidthInput->GetValue() % 20 == 0)
	{
		explorer->generateVideo(
			std::string(vidNameInput->GetValue().mb_str()),
			vidFpsInput->GetValue(),
			vidFramesInput->GetValue(),
			vidItersInput->GetValue(),
			vidWidthInput->GetValue(),
			vidHeightInput->GetValue(),
			vidQualityInput->GetValue(),
			vidFragmentsInput->GetValue(),
			vidFirstFrameInput->GetValue()
		);
	}
	else
	{
		wxMessageBox("The number of fragments must be a divisor of the height and the width must be a multiple of 20 pixels");
	}
	evt.Skip();
}

void ExplorerWindow::displayFragsProgress(wxCommandEvent& evt)
{
	if (evt.GetInt() == rdrFragmentsInput->GetValue())
	{
		rdrProgressDisplay->SetLabelText("finished generating");
	}
	else
	{
		rdrProgressDisplay->SetLabelText(wxString::Format(wxT("generated %d / %d fragments"), evt.GetInt(), rdrFragmentsInput->GetValue()));
	}
	renderPanel->Layout();
	evt.Skip();
}

void ExplorerWindow::displayFramesProgress(wxCommandEvent& evt)
{
	if (evt.GetInt() == vidFramesInput->GetValue())
	{
		vidFramesProgressDisplay->SetLabelText("finished generating");
	}
	else
	{
		vidFramesProgressDisplay->SetLabelText(wxString::Format(wxT("generated %d / %d frames"), evt.GetInt(), vidFramesInput->GetValue()));
	}
	videoPanel->Layout();
	evt.Skip();
}

void ExplorerWindow::onRdrPaneResized(wxCollapsiblePaneEvent& evt)
{
	if (evt.GetCollapsed())
	{
		videoPanel->Show(true);
	}
	else
	{
		videoPanel->Show(false);
	}
	Layout();
	evt.Skip();
}

void ExplorerWindow::onVidPaneResized(wxCollapsiblePaneEvent& evt)
{
	if (evt.GetCollapsed())
	{
		renderPanel->Show(true);
	}
	else
	{
		renderPanel->Show(false);
	}
	Layout();
	evt.Skip();
}

void ExplorerWindow::onLoadParamsFromFile(wxCommandEvent& evt)
{
	wxFileDialog
		openFileDialog(this, _("Choose a file to load parameters from"), "", "",
			"text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;     // the user changed idea...

	std::string filename(openFileDialog.GetPath());
	std::ifstream paramsFile(filename);
	std::string line;

	std::getline(paramsFile, line);
	vidNameInput->ChangeValue(line);

	std::getline(paramsFile, line);
	vidFpsInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidFramesInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidItersInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidWidthInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidHeightInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidQualityInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	vidFragmentsInput->SetValue(std::stoi(line));

	std::getline(paramsFile, line);
	double centerX = std::stod(line);
	std::getline(paramsFile, line);
	double centerY = std::stod(line);
	explorer->setCenter(centerX, centerY);

	std::getline(paramsFile, line);
	explorer->setZoom(std::stod(line));
	wxCommandEvent a;
	updateDisplays(a);

	paramsFile.close();
	evt.Skip();
}

ExplorerWindow::~ExplorerWindow()
{
}