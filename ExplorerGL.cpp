#include "ExplorerGL.h"

wxBEGIN_EVENT_TABLE(ExplorerGL, PreviewGL)
EVT_PAINT(ExplorerGL::render)
EVT_MOUSEWHEEL(ExplorerGL::mouseWheelMoved)
EVT_LEFT_UP(ExplorerGL::leftClick)
wxEND_EVENT_TABLE()

wxDEFINE_EVENT(EVT_RENDER, wxCommandEvent);
wxDEFINE_EVENT(EVT_FRAGMENT, wxCommandEvent);
wxDEFINE_EVENT(EVT_FRAME, wxCommandEvent);

ExplorerGL::ExplorerGL(wxWindow* parent, int id, int* args)
    : PreviewGL(parent, id, args)
{

    // test which the name of the last image
    struct stat buffer;
    std::string nameToTest;
    for (int i = 0; i < 10000; i++)
    {
        nameToTest = "images/" + std::to_string(i) + ".png";
        if (FILE* file = fopen(nameToTest.c_str(), "r")) {
            fclose(file);
        }
        else {
            nbGeneratedImgs = i;
            break;
        }
    }
}

void ExplorerGL::render(wxPaintEvent& evt)
{
    glFinish();
    clock_t begin = clock();

    PreviewGL::render(evt);

    glFinish();
    clock_t end = clock();

    fpsAvg = genTimeSMA(CLOCKS_PER_SEC / double(end - begin));
    wxCommandEvent event(EVT_RENDER, GetId());
    ProcessWindowEvent(event);
}

void ExplorerGL::renderFull(int renderIters, int renderWidth, int renderHeight, int renderQuality)
{
    glUniform2f(windowSizeLocation, renderWidth, renderHeight);
    glUniform1i(iterationsLocation, renderIters);
    glUniform1i(qualityLocation, renderQuality);

    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    GLuint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderWidth, renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);

    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    prepare2DViewport(0, 0, renderWidth, renderHeight);
    glLoadIdentity();
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(-renderWidth, -renderHeight);
    glVertex2f(renderWidth, -renderHeight);
    glVertex2f(renderWidth, renderHeight);
    glVertex2f(-renderWidth, renderHeight);
    glEnd();
    glFlush();

    cv::Mat image(renderHeight, renderWidth, CV_8UC3);
    glReadPixels(0, 0, renderWidth, renderHeight, GL_BGR, GL_UNSIGNED_BYTE, image.data); // somehow this line only wants multiples of 20 as width, I HAVE NO FUCKING IDEA WHY

    cv::imwrite("images/" + std::to_string(nbGeneratedImgs) + ".png", image);
    nbGeneratedImgs++;

    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &t);

    glUniform2f(windowSizeLocation, getWidth(), getHeight());
    glUniform1i(iterationsLocation, iterations);
    glUniform1i(qualityLocation, quality);
}

void ExplorerGL::renderFullFragmented(std::string filename, int renderIters, int renderWidth, int renderHeight, int renderQuality, int nbFrags)
{
    int fragmentHeight = renderHeight / nbFrags;
    int fragmentWidth = renderWidth;

    glUniform2f(windowSizeLocation, fragmentWidth, fragmentHeight);
    glUniform1d(zoomLocation, zoom * nbFrags);
    glUniform1i(iterationsLocation, renderIters);
    glUniform1i(qualityLocation, renderQuality);

    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    GLuint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fragmentWidth, fragmentHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, cmapTexture);

    glClearColor(1.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    prepare2DViewport(0, 0, fragmentWidth, fragmentHeight);
    glLoadIdentity();
    

    cv::Mat image(renderHeight, renderWidth, CV_8UC3);
    cv::Mat fragment(fragmentHeight, fragmentWidth, CV_8UC3);
    double dstY = 1 / zoom;
    double centerFragY;

    for (int i = 0; i < nbFrags; i++)
    {
        centerFragY = centerY - dstY / 2 + dstY / (nbFrags * 2) + i * dstY / nbFrags;
        glUniform2d(centerPosLocation, centerX, centerFragY);
        glBegin(GL_QUADS);
        glVertex2f(-fragmentWidth, -fragmentHeight);
        glVertex2f(fragmentWidth, -fragmentHeight);
        glVertex2f(fragmentWidth, fragmentHeight);
        glVertex2f(-fragmentWidth, fragmentHeight);
        glEnd();
        glFlush();
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glReadPixels(0, 0, fragmentWidth, fragmentHeight, GL_BGR, GL_UNSIGNED_BYTE, fragment.data); // somehow this line only wants multiples of 20 as width, I HAVE NO FUCKING IDEA WHY
        fragment.copyTo(image(cv::Rect(0, i * fragmentHeight, fragmentWidth, fragmentHeight)));
        wxSafeYield();
        wxCommandEvent event(EVT_FRAGMENT, GetId());
        event.SetInt(i + 1);
        ProcessWindowEvent(event);
    }
    cv::flip(image, image, 0);
    cv::imwrite(filename, image);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, 0);
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &t);

    glUniform2f(windowSizeLocation, getWidth(), getHeight());
    glUniform1d(zoomLocation, zoom);
    glUniform1i(iterationsLocation, iterations);
    glUniform1i(qualityLocation, quality);
    glUniform2d(centerPosLocation, centerX, centerY);
}

void ExplorerGL::generateImg(int imgIters, int imgWidth, int imgHeight, int imgQuality, int nbFrags)
{
    renderFullFragmented("images/" + std::to_string(nbGeneratedImgs) + ".png", imgIters, imgWidth, imgHeight, imgQuality, nbFrags);
    nbGeneratedImgs++;
}

void ExplorerGL::generateVideo(std::string vidName, int vidFps, int vidNbFrames, int vidIterFactor, int vidWidth, int vidHeight, int vidQuality, int nbFrags, int firstFrame)
{
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

    if ((    _mkdir((".\\videos\\" + vidName).c_str()) != 0
         ||  _mkdir((".\\videos\\" + vidName + "\\frames").c_str()) != 0)
        && firstFrame == 0) // if first frame != 0 I assume that the user has a brain and I don't bother checking if the folder actually exists
    {
        wxMessageBox("could not create the folder, likely because a video already exists with that name");
        return;
    }

    std::ofstream paramsFile("videos/" + vidName + "/params.txt");
    paramsFile
        << std::setprecision(15)
        << vidName << "\n"
        << vidFps << "\n"
        << vidNbFrames << "\n"
        << vidIterFactor << "\n"
        << vidWidth << "\n"
        << vidHeight << "\n"
        << vidQuality << "\n"
        << nbFrags << "\n"
        << centerX << "\n"
        << centerY << "\n"
        << zoom;
    paramsFile.close();

    std::ofstream perfsFile("videos/" + vidName + "/perfs.txt");
    clock_t frameTime = std::clock();

    double oldZoom = zoom;
    zoom = 0.15;
    double zoomFactor = std::pow(oldZoom / zoom, 1.0 / (double)vidNbFrames);
    zoom *= std::pow(zoomFactor, firstFrame);

    for (int i = firstFrame; i < vidNbFrames; i++)
    {
        zoom *= zoomFactor;
        renderFullFragmented("videos/" + vidName + "/frames/frame_" + std::to_string(i) + ".png", 100 + std::log(zoom + 1) * vidIterFactor, vidWidth, vidHeight, vidQuality, nbFrags);
        nbGeneratedImgs++;
        wxCommandEvent event(EVT_FRAME, GetId());
        event.SetInt(i + 1);
        ProcessWindowEvent(event);
        perfsFile << double(std::clock() - frameTime) / CLOCKS_PER_SEC << std::endl;
        frameTime = std::clock();
    }
    zoom = oldZoom;

    system(("ffmpeg.exe -framerate " + std::to_string(vidFps)
        + " -i videos/" + vidName + "/frames/frame_%d.png -c:v libx264 -crf 17 -preset veryslow videos/" + vidName + "/video.mp4").c_str());
    //ffmpeg.exe -framerate 60 -i videos/video_name/frames/frame_%d.png -c:v libx264 -crf 17 -preset veryslow videos/video_name/video.mp4
    perfsFile.close();

    SetThreadExecutionState(ES_CONTINUOUS);
}
double ExplorerGL::getZoomSensivity()
{
    return zoomSensivity;
}
int ExplorerGL::getFPS()
{
    return fpsAvg;
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