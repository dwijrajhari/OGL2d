#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

class World;

class Renderer
{
	World* m_World;
	float m_Zoom = 1.0;
	const char* m_Title;
	const unsigned int m_SW, m_SH;
	unsigned int VaID, VbID, SpID;
	double pDeltaTime = 0.0f, pLastTime = 0.0f, pThisTime = 0.0;

	int* PolyCount;
	float* RenderData;
	int RenderDataPoints = 0;

	void _OGL_Init();

public:

	GLFWwindow* m_Window;
	bool IS_RUNNING = true;

	Renderer(const char* Title = "OGL2d", const GLuint SW = 800, const GLuint SH = 800, int RenderBuffer = 512, int PolyCount = 64);
	static void fKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void fCursorCallback(GLFWwindow* window, double xPos, double yPos);
	void fKeyHandleInput(int key, int scancode, int action, int mods);
	void fCursorHandleInput(double xPos, double yPos);
	void fLoadWorld(World* world);
	void fClear();
	void fRender();

	void fClose();

	~Renderer();
};