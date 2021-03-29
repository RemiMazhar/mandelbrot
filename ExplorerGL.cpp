#include "ExplorerGL.h"

wxBEGIN_EVENT_TABLE(ExplorerGL, PreviewGL)
EVT_PAINT(ExplorerGL::render)
EVT_MOUSEWHEEL(ExplorerGL::mouseWheelMoved)
EVT_LEFT_UP(ExplorerGL::leftClick)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(EVT_RENDER, wxCommandEvent);

ExplorerGL::ExplorerGL(wxWindow* parent, int id, int* args)
    : PreviewGL(parent, id, args)
{
    
}

void ExplorerGL::render(wxPaintEvent& evt)
{
    PreviewGL::render(evt);

    wxCommandEvent event(EVT_RENDER, GetId());
    ProcessWindowEvent(event);
}

double ExplorerGL::getZoomSensivity()
{
    return zoomSensivity;
}
void ExplorerGL::setZoomSensivity(double zoomSensivityArg)
{
    zoomSensivity = zoomSensivityArg;
}

void ExplorerGL::mouseWheelMoved(wxMouseEvent& event)
{
    const wxPoint pt = wxGetMousePosition();
    double mouseX = pt.x - this->GetScreenPosition().x;
    double mouseY = pt.y - this->GetScreenPosition().y;
    float hZoom = zoom * getHeight() / getWidth();
    double xClick = (mouseX / getWidth() - 0.5) / hZoom + centerX;
    double yClick = -(mouseY / getHeight() - 0.5) / zoom + centerY;

    bool goingdown = event.GetWheelRotation() < 0;
    if (goingdown)
    {
        setCenter((centerX + (1 / zoomSensivity - 1) * xClick) * zoomSensivity, (centerY + (1 / zoomSensivity - 1) * yClick) * zoomSensivity);
        setZoom(zoom / zoomSensivity);
    }
    else
    {
        setCenter((centerX + (zoomSensivity - 1) * xClick) / zoomSensivity, (centerY + (zoomSensivity - 1) * yClick) / zoomSensivity);
        setZoom(zoom * zoomSensivity);
    };
}
void ExplorerGL::leftClick(wxMouseEvent& event)
{
    const wxPoint pt = wxGetMousePosition();
    double mouseX = pt.x - this->GetScreenPosition().x;
    double mouseY = pt.y - this->GetScreenPosition().y;
    float hZoom = zoom * getHeight() / getWidth();
    double xClick = (mouseX / getWidth() - 0.5) / hZoom + centerX;
    double yClick = -(mouseY / getHeight() - 0.5) / zoom + centerY;
    setCenter(xClick, yClick);
}

ExplorerGL::~ExplorerGL()
{

}