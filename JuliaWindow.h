#pragma once
#include "ExplorerWindow.h"

class JuliaWindow :
    public ExplorerWindow
{
public:
    JuliaWindow(ExplorerWindow* mandelbrotExplorerWindow, wxWindow* parent = nullptr);
    ~JuliaWindow();

protected:
    double juliaCReal;
    double juliaCImag;
    GLint juliaCLocation;
    ExplorerWindow* associatedMandelbrot;

public:
    double getJuliaCReal();
    double getJuliaCImag();

    void setJuliaC(double real, double imag);
    void updateJuliaC();
};

