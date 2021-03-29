#include "PreviewGL.h"

wxBEGIN_EVENT_TABLE(PreviewGL, wxGLCanvas)
EVT_PAINT(PreviewGL::render)
wxEND_EVENT_TABLE()

PreviewGL::PreviewGL(wxWindow* parent, int id, int* args) :
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

void PreviewGL::initUniforms()
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

GLhandleARB PreviewGL::loadShader(const char* filename)
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

GLhandleARB PreviewGL::loadShaders(std::vector<std::string> filenames)
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
GLhandleARB PreviewGL::loadShaders()
{
    return loadShaders({ colormapFilename, MAIN_SHADER, kernelFilename });
}

void PreviewGL::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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

void PreviewGL::render(wxPaintEvent& evt)
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
}

int PreviewGL::getWidth()
{
    return GetSize().x;
}
int PreviewGL::getHeight()
{
    return GetSize().y;
}
int PreviewGL::getIterations()
{
    return iterations;
}
int PreviewGL::getQuality()
{
    return quality;
}
double PreviewGL::getZoom()
{
    return zoom;
}
double PreviewGL::getCenterX()
{
    return centerX;
}
double PreviewGL::getCenterY()
{
    return centerY;
}
float PreviewGL::getColorFrequency()
{
    return colorFrequency;
}

void PreviewGL::setZoom(double zoomArg)
{
    zoom = zoomArg;
    glUniform1d(zoomLocation, zoom);
    Refresh();
}
void PreviewGL::setCenter(double x, double y)
{
    centerX = x;
    centerY = y;
    glUniform2d(centerPosLocation, x, y);
    Refresh();
}
void PreviewGL::setIterations(int iterationsArg)
{
    iterations = iterationsArg;
    glUniform1i(iterationsLocation, iterations);
    Refresh();
}
void PreviewGL::setQuality(int qualityArg)
{
    quality = qualityArg;
    glUniform1i(qualityLocation, quality);
    Refresh();
}
void PreviewGL::setColormap(const char* filename)
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

void PreviewGL::setKernel(const char* filename)
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
void PreviewGL::setColorFrequency(float colorFrequencyArg)
{
    colorFrequency = colorFrequencyArg;
    glUniform1f(colorFrequencyLocation, colorFrequency);

    Refresh();
}

void PreviewGL::initGlew()
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

PreviewGL::~PreviewGL()
{
    delete m_context;
}