#include "ExplorerGL.h"

wxBEGIN_EVENT_TABLE(ExplorerGL, wxGLCanvas)
EVT_PAINT(ExplorerGL::render)
EVT_MOUSEWHEEL(ExplorerGL::mouseWheelMoved)
EVT_LEFT_UP(ExplorerGL::leftClick)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(EVT_RENDER, wxCommandEvent);

ExplorerGL::ExplorerGL(wxWindow* parent, int id, int* args) :
    wxGLCanvas(parent, id, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    wxGLCanvas::SetCurrent(*m_context);
    glLoadIdentity();

    initGlew();

    // load mandelbrot shader and create mandelbrot shader program
    mandelbrotProgramID = glCreateProgramObjectARB();
    mandelbrotShader = loadShaders();
    glAttachObjectARB(mandelbrotProgramID, mandelbrotShader);
    glLinkProgram(mandelbrotProgramID);
    glUseProgram(mandelbrotProgramID);
    
    initUniforms();

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

void ExplorerGL::initUniforms()
{
    zoomLocation = glGetUniformLocation(mandelbrotProgramID, "zoom");
    windowSizeLocation = glGetUniformLocation(mandelbrotProgramID, "windowSize");
    centerPosLocation = glGetUniformLocation(mandelbrotProgramID, "centerPos");
    iterationsLocation = glGetUniformLocation(mandelbrotProgramID, "iterations");
	qualityLocation = glGetUniformLocation(mandelbrotProgramID, "quality");
    colorFrequencyLocation = glGetUniformLocation(mandelbrotProgramID, "colorFrequency");
    glUniform2f(windowSizeLocation, getWidth(), getHeight());
    glUniform1d(zoomLocation, zoom);
    glUniform2d(centerPosLocation, centerX, centerY);
    glUniform1i(iterationsLocation, iterations);
	glUniform1i(qualityLocation, quality);
    glUniform1f(colorFrequencyLocation, colorFrequency);
}

GLhandleARB ExplorerGL::loadShader(const char* filename)
{
    std::ifstream ft(filename);
    std::string fragShaderSource((std::istreambuf_iterator<char>(ft)),
        std::istreambuf_iterator<char>());
    GLhandleARB fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    GLchar const* fragFiles[] = { fragShaderSource.c_str() };
    GLint fragLengths[] = { fragShaderSource.size() };
    glShaderSourceARB(fragShader, 1, fragFiles, fragLengths);
    glCompileShaderARB(fragShader);
    return fragShader;
}

GLhandleARB ExplorerGL::loadShaders(std::vector<std::string> filenames)
{
    GLchar const* fragFiles[NB_SHADERS];
    GLint fragLengths[NB_SHADERS];

    std::ifstream ft;
    std::string* fragShaderSource;

    for (int i = 0; i < NB_SHADERS; i++)
    {
        ft.open(filenames[i]);
        fragShaderSource = new std::string((std::istreambuf_iterator<char>(ft)),
            std::istreambuf_iterator<char>());
        ft.close();
        fragFiles[i] = fragShaderSource->c_str();
        fragLengths[i] = fragShaderSource->size();
    }

    GLhandleARB fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(fragShader, NB_SHADERS, fragFiles, fragLengths);
    glCompileShaderARB(fragShader);

    delete fragShaderSource;

    return fragShader;
}
GLhandleARB ExplorerGL::loadShaders()
{
    return loadShaders({colormapFilename, MAIN_SHADER, kernelFilename });
}

void ExplorerGL::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glDisable(GL_DEPTH_TEST);

    glViewport(topleft_x, topleft_y, bottomrigth_x - topleft_x, bottomrigth_y - topleft_y);
    glMatrixMode(GL_2D);
    glLoadIdentity();
     
    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ExplorerGL::render(wxPaintEvent& evt)
{
    if (!IsShown()) return;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    prepare2DViewport(0, 0, getWidth(), getHeight());
    glUniform2f(windowSizeLocation, getWidth(), getHeight());
    // ------------- draw some 2D ----------------
    glLoadIdentity();
    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(-getWidth(), -getHeight());
    glVertex2f(getWidth(), -getHeight());
    glVertex2f(getWidth(), getHeight());
    glVertex2f(-getWidth(), getHeight());
    glEnd();

    glFlush();
    SwapBuffers();

    wxCommandEvent event(EVT_RENDER, GetId());
    ProcessWindowEvent(event);
}

int ExplorerGL::getWidth()
{
    return GetSize().x;
}
int ExplorerGL::getHeight()
{
    return GetSize().y;
}
int ExplorerGL::getIterations()
{
    return iterations;
}
int ExplorerGL::getQuality()
{
	return quality;
}
double ExplorerGL::getZoom()
{
    return zoom;
}
double ExplorerGL::getCenterX()
{
    return centerX;
}
double ExplorerGL::getCenterY()
{
    return centerY;
}
double ExplorerGL::getZoomSensivity()
{
    return zoomSensivity;
}
float ExplorerGL::getColorFrequency()
{
    return colorFrequency;
}

void ExplorerGL::setZoom(double zoomArg)
{
    zoom = zoomArg;
    glUniform1d(zoomLocation, zoom);
    Refresh();
}
void ExplorerGL::setCenter(double x, double y)
{
    centerX = x;
    centerY = y;
    glUniform2d(centerPosLocation, x, y);
    Refresh();
}
void ExplorerGL::setIterations(int iterationsArg)
{
    iterations = iterationsArg;
    glUniform1i(iterationsLocation, iterations);
    Refresh();
}
void ExplorerGL::setQuality(int qualityArg)
{
	quality = qualityArg;
	glUniform1i(qualityLocation, quality);
	Refresh();
}
void ExplorerGL::setZoomSensivity(double zoomSensivityArg)
{
    zoomSensivity = zoomSensivityArg;
}
void ExplorerGL::setColormap(const char* filename)
{
    colormapFilename = filename;
    mandelbrotShader = loadShaders();
    mandelbrotProgramID = glCreateProgramObjectARB();
    glAttachObjectARB(mandelbrotProgramID, mandelbrotShader);
    glLinkProgram(mandelbrotProgramID);
    glUseProgram(mandelbrotProgramID);

    initUniforms();

    Refresh();
}

void ExplorerGL::setKernel(const char* filename)
{
    kernelFilename = filename;
    mandelbrotShader = loadShaders();
    mandelbrotProgramID = glCreateProgramObjectARB();
    glAttachObjectARB(mandelbrotProgramID, mandelbrotShader);
    glLinkProgram(mandelbrotProgramID);
    glUseProgram(mandelbrotProgramID);

    initUniforms();

    Refresh();
}
void ExplorerGL::setColorFrequency(float colorFrequencyArg)
{
    colorFrequency = colorFrequencyArg;
    glUniform1f(colorFrequencyLocation, colorFrequency);

    Refresh();
}

void ExplorerGL::resized(wxSizeEvent& evt)
{
    glMatrixMode(GL_2D);
    glViewport(0, 0, getWidth(), getHeight());
    glUniform2f(windowSizeLocation, getWidth(), getHeight());
    Refresh();
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
    delete m_context;
}

void initGlew()
{
    glutInitDisplayMode(GLUT_RGB);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        
        wxPrintf("GLEW init failed: %s!\n", glewGetErrorString(err));
        wxSafeShowMessage("42", glewGetErrorString(err));
        exit(2);
    }
    else
    {
        printf("GLEW init success!\n");
    }
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
    {
        printf("GLSL support\n");
    }
    else
    {
        printf("no GLSL support\n");
        exit(3);
    }
}