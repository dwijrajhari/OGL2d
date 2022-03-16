#include <math.h>

#include "../OGL_World.h"
#include "../OGL_Common.h"
#include "../OGL_Render.h"

constexpr auto PI = 3.1415926535;

using namespace shapes;

class MouseDrag : public World
{
	Polygon* p1, *pc;
	Point ms;

public:
	int renderBufferSize = sizeof(float) * 2 * 10;

	void fSetupState(int SW_, int SH_) override
	{
		p1 = new Polygon(Point(0.0), 100.0, 4, PI / 4);
		ms.x = SW_; ms.y = SH_;

		Register(p1);
		pc = p1;
	}

	void fUpdateState() override
	{
		if (fCheckPointInPoly(pc, ms))
			fLog(WM_INFO, "True");
		else
			fLog(WM_INFO, "False");
	}

	void fKeyHandleInput(int key, int scancode, int action, int mods) override
	{
		if (key == GLFW_KEY_W && action != GLFW_RELEASE) fMovePoly(pc, 500.0);
		if (key == GLFW_KEY_S && action != GLFW_RELEASE) fMovePoly(pc, -500.0);
		if (key == GLFW_KEY_A && action != GLFW_RELEASE) fRotatePoly(pc, PI);
		if (key == GLFW_KEY_D && action != GLFW_RELEASE) fRotatePoly(pc, -PI);
	}

	void fCursorHandleInput(double x_, double y_) override
	{
		ms.x = x_; ms.y = y_;
	}

	void fMovePoly(Polygon* pl, float pt)
	{
		Point axis = Point(cos(pl->angle), sin(pl->angle));
		for (int i = 0; i < pl->count; i++) pl->p[i] += axis * pt * dT;
		pl->centre += axis * pt * dT;
	}

	void fRotatePoly(Polygon* pl, double angle)
	{
		angle *= dT;
		Mat22 rot(angle);

		for (int i = 0; i < pl->count; i++)
		{
			pl->p[i] = rot * (pl->p[i] - pl->centre) + pl->centre;
		}

		pl->angle += angle;
	}

	bool fCheckCollisionSAT(Polygon* pa, Polygon* pb)
	{
		Point resolveAxis;
		double resolve = INFINITY;

		Polygon* p1, * p2;
		p1 = pa; p2 = pb;

		for (int poly = 0; poly < 2; poly++)
		{
			if (poly == 1)
			{
				p1 = pb;
				p2 = pa;
			}

			for (int i = 0; i < p1->count; i++)
			{
				Point v1 = p1->p[(i + 1) % p1->count] - p1->p[i];
				v1 = { v1.y, -v1.x };
				v1 /= sqrt(v1 * v1);

				double p1max = -INFINITY, p2max = -INFINITY;
				double p1min = INFINITY, p2min = INFINITY;

				double result;
				for (int j = 0; j < p1->count; j++)
				{
					result = p1->p[j] * v1;
					p1max = std::max(p1max, result);
					p1min = std::min(p1min, result);
				}

				for (int j = 0; j < p2->count; j++)
				{
					result = p2->p[j] * v1;
					p2max = std::max(p2max, result);
					p2min = std::min(p2min, result);
				}

				if (p1max < p2min || p2max < p1min) return false;

				double newResolve = std::min(p1max, p2max) - std::max(p1min, p2min);
				if (newResolve < resolve)
				{
					resolve = newResolve;
					resolveAxis = v1;
				}
			}
		}
		if (resolveAxis * (p2->centre - p1->centre) > 0) resolveAxis *= -1;
		for (int i = 0; i < p1->count; i++) p1->p[i] += (resolveAxis * resolve);
		p1->centre += (resolveAxis * resolve);
		return false;
	}

	bool fCheckPointInPoly(Polygon* p1, Point pt)
	{
		for (int i = 0; i < p1->count; i++)
		{
			Point v1 = p1->p[(i + 1) % p1->count] - p1->p[i];
			v1 = { v1.y, -v1.x };
			v1 /= sqrt(v1 * v1);

			double p1max = -INFINITY, p1min = INFINITY;

			double result;
			for (int j = 0; j < p1->count; j++)
			{
				result = p1->p[j] * v1;
				p1max = std::max(p1max, result);
				p1min = std::min(p1min, result);
			}
			double ptval = pt * v1;
			if (p1max < ptval || ptval < p1min) return false;
		}
	}

	~MouseDrag()
	{
		delete p1;
	}
};