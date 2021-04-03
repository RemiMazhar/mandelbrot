#pragma once
#include "wx/wx.h"
#include "PreviewGL.h"

class ImageGenWindow :
    public wxWindow
{
public:
	ImageGenWindow(wxWindow* parent = nullptr);
	~ImageGenWindow();

private:

public:
	wxBoxSizer* mainSizer;
	wxBoxSizer* leftSizer;
	wxBoxSizer* rightSizer;

	PreviewGL* preview;
	wxScrolledWindow* leftPanel;
};

