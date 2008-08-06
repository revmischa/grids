#include <OpenGL/OpenGL.h>
#include <irrlicht.h>
#include <iostream>
#include "GridsOcto.cpp"

using namespace irr;

#pragma comment(lib, "libMacOSX.a")


int main(int argc, char** argv)
{
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

		CSampleSceneNode *myNode = 
		new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666);

	scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.8f, 0, 0.8f));

	if(anim)
	{
		myNode->addAnimator(anim);
	
		anim->drop();
		anim = 0; // As I shouldn't refer to it again, ensure that I can't
	}

	myNode->drop();
	myNode = 0; // As I shouldn't refer to it again, ensure that I can't

	u32 frames=0;
	
	while(device->run())
	{
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		smgr->drawAll();

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

