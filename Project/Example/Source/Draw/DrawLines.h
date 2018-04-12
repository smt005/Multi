#pragma once

#include "Draw.h"
#include "Engine/Source/Callback/Callback.h"
#include "Classes/RandomLines.h"

#include <memory>

class Camera;
class Map;
typedef std::shared_ptr<Map> MapPtr;

class DrawLines : public Draw, public Callback
{
private:
    Camera *_camera = nullptr;
    MapPtr _map = nullptr;
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
