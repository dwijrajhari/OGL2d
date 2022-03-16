#include "OGL_Shapes.h"

struct GLFWwindow;

class World
{

public:
	World() {};


	GLFWwindow* Window = nullptr;
	double dT = 0.0;
	int polyCount = 0;
	std::vector<shapes::Polygon*> m_Poly;

	void Register(shapes::Polygon* poly);

	virtual void fSetupState(int, int) = 0;
	virtual void fUpdateState() = 0;
	virtual void fKeyHandleInput(int, int, int, int);
	virtual void fCursorHandleInput(double, double);

};