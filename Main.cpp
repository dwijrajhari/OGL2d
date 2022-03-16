#include "Worlds/MouseDrag.h"

int main()
{
	MouseDrag m;
	m.fSetupState(800, 800);

	Renderer rd = Renderer("Polygons!", 800, 800, m.renderBufferSize, m.polyCount);

	while (rd.IS_RUNNING)
	{
		rd.fLoadWorld(&m);

		rd.fClear();
		rd.fRender();

		m.fUpdateState();
	}
	rd.fClose();
	return 0;
}

