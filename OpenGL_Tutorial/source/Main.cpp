#include "game/Game.h"
#include "renderer/DrawFrameBuffer.h"
#include "window/Window.h"

int main(int argc, char* argv[])
{
	Window *win = Window::getInstance();
	Game *game = new Game();
	game->setCameraAspectRatio((float)win->getWidth()/ (float)win->getHeight());
	DrawFrameBuffer drb(win->getWidth(), win->getHeight());
	game->setFrameBuffer(drb.bufferId, drb.drawTextureId);
	
	while(win->isWindowRunning())
	{
		game->mainCycleTick();
		
		win->swapBuffer();
		win->pollEvents();
	}
	win->terminateWindow();
}
