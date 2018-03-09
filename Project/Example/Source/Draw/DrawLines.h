#pragma once

#include "Draw.h"
#include "Engine/Source/Callback/Callback.h"
#include "Classes/RandomLines.h"

class CameraGLM;
class ShapeUnited;
class Map;

class DrawLines : public Draw, public Callback
{
private:
    CameraGLM *_camera = nullptr;
    ShapeUnited* _shapeUnited = nullptr;
    Map *_map = nullptr;
	RandomLines _lines;

public:
    DrawLines();
    ~DrawLines();
    
    void init();
    void initDraw();
    void initCallback();
    void prepareDraw(bool clear);
    
    void draw(bool clear = true);
	void drawLine(RandomLines& line);
    
private:
    bool closeGame(void *data);
    bool rotateCamera(void *data);
};
