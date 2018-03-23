#include "window/window.h"
#include "window/mesh_window.h"
#include "window/astar_window.h"
#include "astar/astar.h"
#include "../resource.h"

int main()
{
	//MeshWindow meshWindow;
	//meshWindow.Create("���񵼺�", 100, 100, 1280, 640);
	//meshWindow.Loop();

	AStarWindow astarWindow;
	astarWindow.Create("astar", 100, 100, 1280, 640);
	astarWindow.Loop();

	return 0;
}

