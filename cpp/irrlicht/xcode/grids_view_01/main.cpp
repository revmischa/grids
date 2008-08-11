#include <OpenGL/OpenGL.h>
#include <irrlicht.h>
#include <iostream>
#include "GridsOcto.h"
#include <grids/queue.h>
#include <grids/interface.h>

using namespace irr;
//using namespace Grids;
using namespace std;

#pragma comment(lib, "libMacOSX.a")

void drawGL();

int main(int argc, char** argv)
{
	//Interface * interface = new Interface( "1234" ); // "1234" is the address of the Grids node
	
	IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<s32>(640, 480), 16, false);
		
	if (device == 0)
	{
		return 1; // could not create selected driver.
	}

	// create engine and camera
	device->setWindowCaption(L"Grids Test");

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	smgr->addCameraSceneNodeFPS();

	GOcto *myNode = new GOcto(smgr->getRootSceneNode(), smgr, 666);

	myNode->drop();
	myNode = 0; // As I shouldn't refer to it again, ensure that I can't

	u32 frames=0;
	
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		smgr->drawAll();
		
		drawGL();

		driver->endScene();
		
		
		if (++frames==100)
		{
			core::stringw str = L"Grids Test [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames=0;
		}
	}

	device->drop();
	
	return 0;
}


void drawGL( )
{
	





}
