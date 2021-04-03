#pragma once
#include "PreviewGL.h"
#include "SMA.h"
#include <ctime>
#define FPS_ROLLING_AVG_NB_ELEMS 10

wxDECLARE_EVENT(EVT_RENDER, wxCommandEvent);
wxDECLARE_EVENT(EVT_FRAGMENT, wxCommandEvent);

class ExplorerGL :
    public PreviewGL
{
public:
    double zoomSensivity = 1.2;
    SMA<FPS_ROLLING_AVG_NB_ELEMS, unsigned int, unsigned int> genTimeSMA;
    int fpsAvg;
    int nbGeneratedImgs = 0;

public:
    ExplorerGL(wxWindow* parent, int id, int* args);

    void render(wxPaintEvent& evt);
    void renderFull(int renderIters = 1000, int renderWidth = 1920, int renderHeight = 1080, int renderQuality = 2);
    void renderFullFragmented(int renderIters = 1000, int renderWidth = 1920, int renderHeight = 1080, int renderQuality = 2, int nbFrags = 1);

    double getZoomSensivity();
    int getFPS();

    void setZoomSensivity(double zoomSensivityArg);

    // events
    void mouseWheelMoved(wxMouseEvent& event);
    void leftClick(wxMouseEvent& event);

    DECLARE_EVENT_TABLE()

    virtual ~ExplorerGL();
};

