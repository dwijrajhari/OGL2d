#include "OGL_Shader.h"
#include "OGL_Common.h"
#include "OGL_World.h"

#include "OGL_Render.h"

void Renderer::_OGL_Init()
{
	if (!glfwInit())
	{
		fLog(WM_ERROR, "Failed to initialise GLFW!");
	}

	else
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwSetErrorCallback(fLog);
		m_Window = glfwCreateWindow(m_SW, m_SH, m_Title, NULL, NULL);

		if (m_Window == NULL)
		{
			glfwTerminate();
			fLog(WM_ERROR, "Failed to create new window!");
		}
		else
		{
			glfwMakeContextCurrent(m_Window);
			if (glewInit() != GLEW_OK)
			{
				fLog(WM_ERROR, "Failed to initialise GLEW!");
			}
			else
			{
				fLog(WM_INFO, glGetString(GL_RENDERER));
				fLog(WM_INFO, "OGL2d Setup Successful");
			}
		}
	}
	glfwSwapInterval(1);
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, fKeyCallback);
	glfwSetCursorPosCallback(m_Window, fCursorCallback);

	glPointSize(1);

}

Renderer::Renderer(const char* Title, const GLuint SW, const GLuint SH, int RenderBuffer, int PolyCount_)
	:m_Window(NULL), m_Title(Title), m_SW(SW), m_SH(SH), m_World(NULL)
{
	_OGL_Init();
	pLastTime = glfwGetTime();

	const char* VS = "#version 330 core\n"
		"layout(location = 0) in vec2 posAttr;\n"
		"uniform float rx;\n"
		"uniform float ry;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(posAttr.x/rx, posAttr.y/ry, 0.0, 1.0);\n"
		"}\n";
	const char* FS = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0);\n"
		"}\n";

	SpID = ShaderProgram(VS, FS).ID;

	glGenVertexArrays(1, &VaID);
	glGenBuffers(1, &VbID);

	glBindVertexArray(VaID);
	glBindBuffer(GL_ARRAY_BUFFER, VbID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	RenderData = new float[RenderBuffer/sizeof(float)];
	PolyCount = new int[PolyCount_];
}

void Renderer::fKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static_cast<Renderer*>(glfwGetWindowUserPointer(window))->fKeyHandleInput(key, scancode, action, mods);
}

void Renderer::fCursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	static_cast<Renderer*>(glfwGetWindowUserPointer(window))->fCursorHandleInput(xPos, yPos);
}

void Renderer::fKeyHandleInput(int key, int scancode, int action, int mods)
{
	//Renderer level input handling
	if (key == GLFW_KEY_MINUS && action != GLFW_RELEASE) m_Zoom -= m_Zoom<=0.5? 0:0.1;
	if (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE) m_Zoom += m_Zoom>=2.5? 0:0.1;

	//world level input handling
	if (m_World != NULL) m_World->fKeyHandleInput(key, scancode, action, mods);
}

void Renderer::fCursorHandleInput(double xPos, double yPos)
{
	if (m_World != NULL) m_World->fCursorHandleInput(xPos-m_SW/2, -yPos+m_SH/2);
}


void Renderer::fLoadWorld(World* world)
{
	m_World = world;
	m_World->Window = m_Window;
	_ASSERT(m_World != NULL);

	RenderDataPoints = 0;  // To make sure only one world is loaded at a time

	

	for (int i = 0; i < m_World->polyCount; ++i)
	{
		RenderData[2 * RenderDataPoints] = m_World->m_Poly[i]->centre.x;
		RenderData[(2 * RenderDataPoints) + 1] = m_World->m_Poly[i]->centre.y;
		++RenderDataPoints;

		PolyCount[i] = m_World->m_Poly[i]->count;
		for (int j = 0; j < PolyCount[i]; ++j)
		{
			RenderData[2 * RenderDataPoints] = m_World->m_Poly[i]->p[j].x;
			RenderData[(2 * RenderDataPoints) + 1] = m_World->m_Poly[i]->p[j].y;
			++RenderDataPoints;
		}
	}
	glBindVertexArray(VaID);
	glBindBuffer(GL_ARRAY_BUFFER, VbID);
	glBufferData(GL_ARRAY_BUFFER, RenderDataPoints * sizeof(float) * 2, RenderData, GL_DYNAMIC_DRAW);
	glBindVertexArray(0);
}

void Renderer::fClear()
{
	glClearColor(0x00, 0x00, 0x00, 0x01);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::fRender()
{
	int framewidth, frameheight;
	glfwGetFramebufferSize(m_Window, &framewidth, &frameheight);
	glViewport(0, 0, framewidth, frameheight);

	glUseProgram(SpID);
	glBindVertexArray(VaID);
	glUniform1f(glGetUniformLocation(SpID, "rx"), (float)framewidth/(2 * m_Zoom));
	glUniform1f(glGetUniformLocation(SpID, "ry"), (float)frameheight/(2 * m_Zoom));

	int i = 0, j = 0;
	while(i < RenderDataPoints)
	{
		glDrawArrays(GL_LINE_LOOP, i, 2);
		glDrawArrays(GL_LINE_LOOP, i+1, PolyCount[j]);
		i += PolyCount[j] +1;
		++j;
	}

	glfwSwapBuffers(m_Window);
	glfwPollEvents();

	pThisTime = glfwGetTime();
	pDeltaTime = pThisTime - pLastTime;
	pLastTime = pThisTime;
	m_World->dT = pDeltaTime;

	IS_RUNNING = !glfwWindowShouldClose(m_Window);
}

void Renderer::fClose()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

Renderer::~Renderer()
{
	delete[] RenderData;
	delete[] PolyCount;
}
