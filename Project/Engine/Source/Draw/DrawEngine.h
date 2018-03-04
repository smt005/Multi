#pragma once

class Map;
class Object;
class Shape;
class Mesh;

class DrawEngine
{
private:
	static float _backgroundColor[4];
	static unsigned int _programBase;
	static unsigned int _cuttrentBufer;
	static unsigned int _cuttrentTexture;

public:
	static void setBackgroundColor(const float *color);
	static void setBackgroundColor(const float &red, const float &green, const float &blue, const float &alpha);
	static const float* backgroundColor();

public:
	static void clearColor();
	static void clearColor(const float &red, const float &green, const float &blue, const float &alpha);
	static void prepareDraw(bool clear);

	// DrawMap
	static void initDrawMap();
	static void drawMap(Map& map);
	static void drawModel(Object& object);
	static void drawModelTemp(Object& object);
	static void drawShape(Shape& shape);
	static void drawMesh(Mesh& mesh);
};
