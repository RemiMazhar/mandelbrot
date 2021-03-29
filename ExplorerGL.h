#pragma once
#include "PreviewGL.h"

wxDECLARE_EVENT(EVT_RENDER, wxCommandEvent);

class ExplorerGL :
    public PreviewGL
{
    double zoomSensivity = 1.2;

public:
    ExplorerGL(wxWindow* parent, int id, int* args);

    void render(wxPaintEvent& evt);

    double getZoomSensivity();
    void setZoomSensivity(double zoomSensivityArg);

    // events
    void mouseWheelMoved(wxMouseEvent& event);
    void leftClick(wxMouseEvent& event);

    DECLARE_EVENT_TABLE()

    virtual ~ExplorerGL();
};

