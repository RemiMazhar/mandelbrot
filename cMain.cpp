#include "cMain.h"

// ids
#define CHOICE_ID 10001
#define WIDTH_ID 10002
#define HEIGHT_ID 10003
#define QUALITY_ID 10004
#define ITERS_ID 10005
#define SENSIVITY_ID 10006
#define X_ID 10007
#define Y_ID 10008
#define GENERATE_ID 10009

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	// syntax: EVT_BUTTON(10001, OnBtn1Clicked)
	EVT_CHOICE(CHOICE_ID, modeChanged)
	EVT_SPINCTRL(WIDTH_ID, widthChanged)
	EVT_SPINCTRL(HEIGHT_ID, heightChanged)
	EVT_SPINCTRL(QUALITY_ID, qualityChanged)
	EVT_SPINCTRL(ITERS_ID, itersChanged)
	EVT_SPINCTRLDOUBLE(SENSIVITY_ID, sensivityChanged)
	EVT_SPINCTRLDOUBLE(X_ID, xChanged)
	EVT_SPINCTRLDOUBLE(Y_ID, yChanged)
	EVT_BUTTON(GENERATE_ID, onGenerateBtnClick)
	EVT_IDLE(onIdle)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "some name here", wxPoint(30, 30), wxSize(300, 600))
{
	this->SetBackgroundColour(*wxWHITE);

	sizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(sizer);

	const wxString choices[NB_MODES] = { wxT("explore mandelbrot"), wxT("generate a video"), wxT("generate a single image") };
	modeSelection = new wxChoice(this, CHOICE_ID, wxDefaultPosition, wxDefaultSize, NB_MODES, choices);
	sizer->Add(modeSelection, 0, wxEXPAND);

	sizer->AddSpacer(10);

	wxBoxSizer *winWidthSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *winWidthLabel = new wxStaticText(this, wxID_ANY, "window width: ");
	winWidthSizer->Add(winWidthLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	winWidthInput = new wxSpinCtrl(this, WIDTH_ID, "1920", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 100000);
	winWidthSizer->Add(winWidthInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sizer->Add(winWidthSizer, 0, wxEXPAND);

	sizer->AddSpacer(10);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sizer->AddSpacer(10);

	wxBoxSizer *winHeightSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *winHeightLabel = new wxStaticText(this, wxID_ANY, "window height: ");
	winHeightSizer->Add(winHeightLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	winHeightInput = new wxSpinCtrl(this, HEIGHT_ID, "1080", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 100000);
	winHeightSizer->Add(winHeightInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sizer->Add(winHeightSizer, 0, wxEXPAND);

	sizer->AddSpacer(10);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sizer->AddSpacer(10);

	wxBoxSizer *qualitySizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *qualityLabel = new wxStaticText(this, wxID_ANY, "quality: ");
	qualitySizer->Add(qualityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	qualityInput = new wxSpinCtrl(this, QUALITY_ID);
	qualitySizer->Add(qualityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sizer->Add(qualitySizer, 0, wxEXPAND);

	sizer->AddSpacer(10);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sizer->AddSpacer(10);

	wxBoxSizer *itersSizer = new wxBoxSizer(wxHORIZONTAL);
	itersLabel = new wxStaticText(this, wxID_ANY, "number of iterations: ");
	itersSizer->Add(itersLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	itersInput = new wxSpinCtrl(this, ITERS_ID, "10000", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 100000000);
	itersSizer->Add(itersInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sizer->Add(itersSizer, 0, wxEXPAND);

	sensivitySizer = new wxBoxSizer(wxVERTICAL);
	sensivitySizer->AddSpacer(10);
	sensivitySizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sensivitySizer->AddSpacer(10);
	wxBoxSizer *sensivityHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *sensivityLabel = new wxStaticText(this, wxID_ANY, "zoom sensivity: ");
	sensivityHSizer->Add(sensivityLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	sensivityInput = new wxSpinCtrlDouble(this, SENSIVITY_ID, "1.2", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 5, 1.2, 0.05);
	sensivityHSizer->Add(sensivityInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	sensivitySizer->Add(sensivityHSizer, 0, wxEXPAND);
	sizer->Add(sensivitySizer, 0, wxEXPAND);

	nbFramesSizer = new wxBoxSizer(wxVERTICAL);
	nbFramesSizer->AddSpacer(10);
	nbFramesSizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	nbFramesSizer->AddSpacer(10);
	wxBoxSizer *nbFramesHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *nbFramesLabel = new wxStaticText(this, wxID_ANY, "number of frames: ");
	nbFramesHSizer->Add(nbFramesLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	nbFramesInput = new wxSpinCtrl(this, wxID_ANY, "3000", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 1000000);
	nbFramesHSizer->Add(nbFramesInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	nbFramesSizer->Add(nbFramesHSizer, 0, wxEXPAND);
	sizer->Add(nbFramesSizer, 0, wxEXPAND);

	xCoordSizer = new wxBoxSizer(wxVERTICAL);
	xCoordSizer->AddSpacer(10);
	xCoordSizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	xCoordSizer->AddSpacer(10);
	wxBoxSizer *xCoordHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *xCoordLabel = new wxStaticText(this, wxID_ANY, "x coordinate: ");
	xCoordHSizer->Add(xCoordLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	xCoordInput = new wxSpinCtrlDouble(this, X_ID, "0", wxDefaultPosition, wxSize(140, wxDefaultSize.y), wxSP_ARROW_KEYS, -10, 10, 0, 1e-16);
	xCoordHSizer->Add(xCoordInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	xCoordSizer->Add(xCoordHSizer, 0, wxEXPAND);
	sizer->Add(xCoordSizer, 0, wxEXPAND);

	yCoordSizer = new wxBoxSizer(wxVERTICAL);
	yCoordSizer->AddSpacer(10);
	yCoordSizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	yCoordSizer->AddSpacer(10);
	wxBoxSizer *yCoordHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *yCoordLabel = new wxStaticText(this, wxID_ANY, "y coordinate: ");
	yCoordHSizer->Add(yCoordLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	yCoordInput = new wxSpinCtrlDouble(this, Y_ID, "0", wxDefaultPosition, wxSize(140, wxDefaultSize.y), wxSP_ARROW_KEYS, -10, 10, 0, 1e-16);
	yCoordHSizer->Add(yCoordInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	yCoordSizer->Add(yCoordHSizer, 0, wxEXPAND);
	sizer->Add(yCoordSizer, 0, wxEXPAND);

	fpsSizer = new wxBoxSizer(wxVERTICAL);
	fpsSizer->AddSpacer(10);
	fpsSizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	fpsSizer->AddSpacer(10);
	wxBoxSizer *fpsHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *fpsLabel = new wxStaticText(this, wxID_ANY, "FPS: ");
	fpsHSizer->Add(fpsLabel, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	fpsInput = new wxSpinCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(60, wxDefaultSize.y), wxSP_ARROW_KEYS, 0, 240);
	fpsHSizer->Add(fpsInput, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 10);
	fpsSizer->Add(fpsHSizer, 0, wxEXPAND);
	sizer->Add(fpsSizer, 0, wxEXPAND);

	vidNameSizer = new wxBoxSizer(wxVERTICAL);
	vidNameSizer->AddSpacer(10);
	vidNameSizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	vidNameSizer->AddSpacer(10);
	wxBoxSizer *vidNameHSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *vidNameLabel = new wxStaticText(this, wxID_ANY, "name of the video: ");
	vidNameHSizer->Add(vidNameLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
	vidNameInput = new wxTextCtrl(this, wxID_ANY, "interesting_title");
	vidNameHSizer->AddSpacer(10);
	vidNameHSizer->Add(vidNameInput, 1, wxEXPAND);
	vidNameHSizer->AddSpacer(10);
	vidNameSizer->Add(vidNameHSizer, 0, wxEXPAND);
	sizer->Add(vidNameSizer, 0, wxEXPAND);

	sizer->AddSpacer(10);
	sizer->Add(new wxStaticLine(this, wxID_ANY), 0, wxEXPAND);
	sizer->AddSpacer(10);

	generateButton = new wxButton(this, GENERATE_ID, "GENERATE", wxDefaultPosition, wxSize(100, 50));
	sizer->Add(generateButton, 0, wxCENTRE);
	generateButton->Show(false);

	modeSelection->SetSelection(0);
	wxCommandEvent a;
	modeChanged(a);
}

void cMain::modeChanged(wxCommandEvent &evt)
{
	mode = modeSelection->GetSelection();
	switch (mode)
	{
	case exploreMandelbrot:
		winWidthInput->SetValue(500);
		winHeightInput->SetValue(500);
		qualityInput->SetValue(1);
		itersLabel->SetLabel("number of iterations: ");
		itersInput->SetValue(10000);
		sensivitySizer->ShowItems(true);
		nbFramesSizer->ShowItems(false);
		fpsSizer->ShowItems(false);
		vidNameSizer->ShowItems(false);
		generateButton->SetLabel("EXPLORE");
		generateButton->Show(true);
		break;
	case generateVideo:
		winWidthInput->SetValue(1920);
		winHeightInput->SetValue(1080);
		qualityInput->SetValue(2);
		itersLabel->SetLabel("iteration factor: ");
		itersInput->SetValue(400);
		sensivitySizer->ShowItems(false);
		nbFramesSizer->ShowItems(true);
		fpsSizer->ShowItems(true);
		vidNameSizer->ShowItems(true);
		generateButton->SetLabel("GENERATE");
		generateButton->Show(true);
		break;
	case generateImage:
		winWidthInput->SetValue(1920);
		winHeightInput->SetValue(1080);
		qualityInput->SetValue(2);
		itersLabel->SetLabel("number of iterations: ");
		itersInput->SetValue(10000);
		sensivitySizer->ShowItems(false);
		nbFramesSizer->ShowItems(false);
		fpsSizer->ShowItems(false);
		vidNameSizer->ShowItems(false);
		generateButton->SetLabel("GENERATE");
		generateButton->Show(true);
		break;
	}
	sizer->Layout();
	evt.Skip();
}

void cMain::widthChanged(wxSpinEvent &evt)
{
	explorerWindow.width = winWidthInput->GetValue();
	explorerWindow.update();
	evt.Skip();
}

void cMain::heightChanged(wxSpinEvent &evt)
{
	explorerWindow.height = winHeightInput->GetValue();
	explorerWindow.update();
	evt.Skip();
}

void cMain::qualityChanged(wxSpinEvent &evt)
{
	explorerWindow.quality = qualityInput->GetValue();
	explorerWindow.update();
	evt.Skip();
}

void cMain::itersChanged(wxSpinEvent &evt)
{
	explorerWindow.iterations = itersInput->GetValue();
	evt.Skip();
}

void cMain::sensivityChanged(wxSpinDoubleEvent &evt)
{
	explorerWindow.zoomSensivity = sensivityInput->GetValue();
	evt.Skip();
}

void cMain::xChanged(wxSpinDoubleEvent &evt)
{
	explorerWindow.x = xCoordInput->GetValue();
	evt.Skip();
}

void cMain::yChanged(wxSpinDoubleEvent &evt)
{
	explorerWindow.y = yCoordInput->GetValue();
	evt.Skip();
}

void cMain::onGenerateBtnClick(wxCommandEvent &evt)
{
	switch (mode)
	{
	case exploreMandelbrot:
		cv::namedWindow("yeth");
		explorerWindow.update();
		break;
	default:
		throw;
	}
	exploring = !exploring;
	evt.Skip();
}

void cMain::onIdle(wxIdleEvent &evt)
{
	if (exploring && mode == exploreMandelbrot)
	{
		explorerWindow.nextFrame();
		cv::imshow("yeth", explorerWindow.frame);
		cv::waitKey(1);
		updateDisplays();
	}
	evt.Skip();
}

void cMain::updateDisplays()
{
	xCoordInput->SetValue(explorerWindow.x);
	yCoordInput->SetValue(explorerWindow.y);
}

cMain::~cMain()
{
}