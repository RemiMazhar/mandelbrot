#pragma once

#ifndef _glpane_
#define _glpane_
#endif

#include "wx/wx.h"
#include <GL/glew.h>
#include "wx/glcanvas.h"
#include <wx/log.h> 
#include <GL/glut.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#define MAIN_SHADER "mandelbrotShader.frag"
#define DEFAULT_COLORMAP "colormaps/IDL_Black-White_Linear.frag"
#define DEFAULT_KERNEL "kernels/defaultKernel.frag"
#define NB_SHADERS 3
//#include "stb_image.h"

class PreviewGL : public wxGLCanvas
{
protected:
	wxGLContext* m_context;

	// parameters for rendering
	double zoom = 0.25;
	double centerX = 0.f;
	double centerY = 0.f;
	int iterations = 1000;
	int quality = 1;
	float colorFrequency = 3.f;
	std::string colormapFilename = DEFAULT_COLORMAP;
	std::string kernelFilename = DEFAULT_KERNEL;

	// associated uniforms
	GLint windowSizeLocation;
	GLint zoomLocation;
	GLint centerPosLocation;
	GLint iterationsLocation;
	GLint qualityLocation;
	GLint colormapLocation;
	GLint colorFrequencyLocation;

	// shaders
	GLhandleARB mandelbrotShader;
	GLhandleARB colormapShader;

	GLuint cmapTexture;

	void initUniforms();

public:
	GLuint mandelbrotProgramID;
	GLuint colormapProgramID;

	PreviewGL(wxWindow* parent, int id, int* args);

	GLuint loadColormap(std::string cmapName);

	GLhandleARB loadShader(const char* filename);
	GLhandleARB loadShaders(std::vector<std::string> filenames);
	GLhandleARB loadShaders();
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void render(wxPaintEvent& evt);

	// getters
	int getWidth();
	int getHeight();
	int getIterations();
	int getQuality();
	double getZoom();
	double getCenterX();
	double getCenterY();
	float getColorFrequency();

	// setters
	void setZoom(double zoomArg);
	void setCenter(double x, double y);
	void setIterations(int iterationsArg);
	void setQuality(int qualityArg);
	void setColormap(const char* filename);
	void setKernel(const char* filename);
	void setColorFrequency(float colorFrequencyArg);

	void initGlew();

	DECLARE_EVENT_TABLE()

	virtual ~PreviewGL();
};