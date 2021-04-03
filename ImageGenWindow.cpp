#include "ImageGenWindow.h"

#define PREVIEW_ID 10001

ImageGenWindow::ImageGenWindow(wxWindow* parent) : wxWindow(parent, wxID_ANY)
{
	this->SetBackgroundColour(*wxWHITE);

	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	this->SetSizer(mainSizer);

	leftPanel = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(250, 0));
	leftSizer = new wxBoxSizer(wxVERTICAL);

	leftSizer->AddSpacer(10);
	


	leftPanel->SetSizer(leftSizer);
	leftPanel->SetScrollRate(5, 5);
	mainSizer->Add(leftPanel, 0, wxEXPAND);

	rightSizer = new wxBoxSizer(wxVERTICAL);
	int args[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };
	preview = new PreviewGL(this, PREVIEW_ID, args);
	rightSizer->Add(preview, 1, wxEXPAND);
	mainSizer->Add(rightSizer, 1, wxEXPAND);
}

ImageGenWindow::~ImageGenWindow()
{

}