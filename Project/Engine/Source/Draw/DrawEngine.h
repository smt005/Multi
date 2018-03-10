#pragma once

class Map;
class Object;
class Shape;

class ObjectUnited;
class ModelUnited;
class Mesh;

class DrawEngine
{
private:
	static float _backgroundColor[4];
	static unsigned int _programBase;
	static unsigned int _cuttrentBufer;
	static unsigned int _cuttrentTexture;

	static unsigned int _programLine;

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

	static void drawObject(ObjectUnited& object);
	static void drawModel(ModelUnited& model, const float* matrix);
	static void drawMesh(Mesh& mesh);

	// DrawLines
	static void initDrawLines();
	static void prepareDrawLine();
	static void drawLine(float* point0, float* point1, float* color = nullptr);
};
