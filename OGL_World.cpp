#include "OGL_World.h"
#include "OGL_Common.h"

void World::Register(shapes::Polygon* poly)
{
	m_Poly.push_back(poly);
	++polyCount;
}

void World::fKeyHandleInput(int key, int scancode, int action, int mods)
{
}

void World::fCursorHandleInput(double, double)
{
}
