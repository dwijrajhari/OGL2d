#include <math.h>
#include <algorithm>

#include "../OGL_World.h"
#include "../OGL_Common.h"
#include "../OGL_Render.h"

constexpr auto PI = 3.1415926535;

// Simulation parameters
constexpr auto NosParticles = 1000;
constexpr auto AllRadius = 2.0;

using namespace shapes;

struct Particle : public Polygon
{
	double radius;
	Point velocity;

	Particle(Point centre_, double radius_, Point velocity_ = { 0.0 })
		:radius(radius_), velocity(velocity_), Polygon(centre_, radius_, 0) {}
};

class Particles : public World
{

	std::vector<Particle*> Particles;
	Particle* pc;
	int SW, SH;
	Point Cursor;

public:

	int renderbuffer = NosParticles * 2 * sizeof(float);
	int polycount = NosParticles;

	void fSetupState(int SW_, int SH_) override
	{
		SW = SW_, SH = SH_;

		for (int i = 0; i < NosParticles; i++)
		{
			Particle* c = new Particle({ (double)(rand() % SW) - SW / 2, (double)(rand() % SH) - SH / 2 }, AllRadius, { (rand() % 400) - 200.0, (rand() % 400) - 200.0 });
			Register(c);
			Particles.push_back(c);
		}

	}

	void fUpdateState() override
	{
		fUpdateScreenSize();

		for (int i = 0; i < Particles.size(); i++)
		{
			Particles[i]->centre += Particles[i]->velocity * dT;
		}
		fnSweepAndPrune();
		for (int i = 0; i < Particles.size()/2; i++)
		{
			for (int j = 0; j < Particles.size()/2; j++)
			{
				if (i != j) fResolveCollisionParticle(Particles[i], Particles[j]);
			}
			fResolveCollisionBoundary(Particles[i]);
		}

		for (int i = Particles.size() / 2; i < Particles.size(); i++)
		{
			for (int j = Particles.size() / 2; j < Particles.size(); j++)
			{
				if (i != j) fResolveCollisionParticle(Particles[i], Particles[j]);
			}
			fResolveCollisionBoundary(Particles[i]);
		}

		//if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pc) fSnapToLocation(pc, Cursor);
	}

	void fMovePoly(Polygon* pl, float pt)
	{
		Point axis = Point(cos(pl->angle), sin(pl->angle));
		pl->centre += axis * pt * dT;
	}


	void fResolveCollisionParticle(Particle* pa, Particle* pb)
	{
		Point axis = (pa->centre - pb->centre);
		double dist = sqrt(axis * axis);
		if (dist < (pa->radius + pb->radius))
		{
			axis /= dist;
			dist = pa->radius + pb->radius - dist;
			
			pa->centre += axis * (dist / 2);
			pb->centre -= axis * (dist / 2);

			Point tempVelocity = pa->velocity;
			pa->velocity += axis * ((pb->velocity - pa->velocity) * axis);
			pb->velocity += axis * ((tempVelocity - pb->velocity) * axis);

		}
	}

	void fResolveCollisionBoundary(Particle* pa)
	{
		if (pa->centre.x - pa->radius < -SW / 2) { fSnapToLocation(pa, { -SW / 2 + pa->radius, pa->centre.y }); pa->velocity.x *= -1; }
		if (pa->centre.x + pa->radius >  SW / 2) { fSnapToLocation(pa, {  SW / 2 - pa->radius, pa->centre.y }); pa->velocity.x *= -1; }
		if (pa->centre.y - pa->radius < -SH / 2) { fSnapToLocation(pa, { pa->centre.x, -SH / 2 + pa->radius }); pa->velocity.y *= -1; }
		if (pa->centre.y + pa->radius >  SH / 2) { fSnapToLocation(pa, { pa->centre.x,  SH / 2 - pa->radius }); pa->velocity.y *= -1; }
	}

	void fCursorHandleInput(Particle* pa, double xPos_, double yPos_)
	{
		pc = NULL;
		Cursor = { xPos_ - SW/2, SH/2 - yPos_ };
		Point axis = pa->centre - Cursor;
		double dist = sqrt(axis * axis);
		if (dist < pa->radius)
		{
			pc = pa;
		}
	}

	void fSnapToLocation(Particle* pa, const Point& p)
	{
		pa->centre = p;
	}

	void fUpdateScreenSize()
	{
		glfwGetFramebufferSize(Window, &SW, &SH);
	}

	~Particles()
	{
		for (int i = 0; i < Particles.size(); i++)
		{
			delete Particles[i];
		}
	}

	void fnSweepAndPrune()
	{
		std::sort(Particles.begin(), Particles.begin(), 
			[](Particle* p1, Particle* p2) {return p1->centre.x >= p2->centre.x ? p2 : p1; });
	}
};