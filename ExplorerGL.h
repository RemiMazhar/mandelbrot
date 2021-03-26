#ifndef _glpane_
#define _glpane_

#include "wx/wx.h"
#include <GL/glew.h>
#include "wx/glcanvas.h"
#include <wx/log.h> 
#include <GL/glut.h>
#include <string>
#include <fstream>
#include <streambuf>
#define MAIN_SHADER "mandelbrotShader.frag"
#define DEFAULT_COLORMAP "colormaps/IDL_Black-White_Linear.frag"
#define DEFAULT_KERNEL "kernels/defaultKernel.frag"
#define NB_SHADERS 3
//#include "stb_image.h"

wxDECLARE_EVENT(EVT_RENDER, wxCommandEvent);

class ExplorerGL : public wxGLCanvas
{
	wxGLContext* m_context;

	GLuint mandelbrotProgramID;
	GLuint colormapProgramID;

	//parameters for user interface
	double zoomSensivity = 1.2;

	// parameters for rendering
	double zoom = 0.25;
	double centerX = 0.f;
	double centerY = 0.f;
	int iterations = 1000;
	float colorFrequency = 3.f;
	std::string colormapFilename = DEFAULT_COLORMAP;
	std::string kernelFilename = DEFAULT_KERNEL;

	// associated uniforms
	GLint windowSizeLocation;
	GLint zoomLocation;
	GLint centerPosLocation;
	GLint iterationsLocation;
	GLint colormapLocation;
	GLint colorFrequencyLocation;

	// shaders
	GLhandleARB mandelbrotShader;
	GLhandleARB colormapShader;

	void initUniforms();

public:
	ExplorerGL(wxWindow* parent, int id, int* args);

	GLhandleARB loadShader(const char* filename);
	GLhandleARB loadShaders(std::vector<std::string> filenames);
	GLhandleARB loadShaders(); 
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void render(wxPaintEvent& evt);

	// getters
	int getWidth();
	int getHeight();
	int getIterations();
	double getZoom();
	double getCenterX();
	double getCenterY();
	double getZoomSensivity();
	float getColorFrequency();
	
	// setters
	void setZoom(double zoomArg);
	void setCenter(double x, double y);
	void setIterations(int iterationsArg);
	void setZoomSensivity(double zoomSensivityArg);
	void setColormap(const char* filename);
	void setKernel(const char* filename);
	void setColorFrequency(float colorFrequencyArg);

	void resized(wxSizeEvent& evt);

	// events
	void mouseWheelMoved(wxMouseEvent& event);
	void leftClick(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()

	virtual ~ExplorerGL();
};
#endif

void initGlew();