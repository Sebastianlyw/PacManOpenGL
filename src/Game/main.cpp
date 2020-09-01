#include "Window.h"
#include "gameHelper.h"

int main(int argc, char* argv[])
{
	Window* window = new Window(WINDOWSIZE.x, WINDOWSIZE.y);
	window->Mainloop();
	delete window;

	return 0;
}
