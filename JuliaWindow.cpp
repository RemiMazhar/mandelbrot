#include "JuliaWindow.h"

JuliaWindow::JuliaWindow(ExplorerWindow* mandelbrotExplorerWindow, wxWindow* parent) : ExplorerWindow(parent)
{
	associatedMandelbrot = mandelbrotExplorerWindow;

	explorer->setKernel("kernels/juliaKernel.frag");

	juliaCLocation = glGetUniformLocation(explorer->mandelbrotProgramID, "juliaC");

	updateJuliaC();
}

double JuliaWindow::getJuliaCReal()
{
	return juliaCReal;
}

double JuliaWindow::getJuliaCImag()
{
	return juliaCImag;
}

void JuliaWindow::setJuliaC(double real, double imag)
{
	juliaCReal = real;
	juliaCImag = imag;
	glUniform2d(juliaCLocation, juliaCReal, juliaCImag);
	//explorer->setKernel("kernels/juliaKernel.frag");
	//Refresh();
	//wxWindow::Refresh();
}

void JuliaWindow::updateJuliaC()
{
	setJuliaC(
		juliaCReal = associatedMandelbrot->explorer->getCenterX(),
		juliaCImag = associatedMandelbrot->explorer->getCenterY()
	);
}

JuliaWindow::~JuliaWindow()
{

}