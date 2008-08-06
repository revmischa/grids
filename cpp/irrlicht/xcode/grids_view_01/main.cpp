#include <OpenGL/OpenGL.h>
#include <irrlicht.h>
#include <iostream>
#include "GridsOcto.h"
#include "GridsQueue.h"

using namespace irr;
using namespace Grids;
using namespace std;

#pragma comment(lib, "libMacOSX.a")


int main(int argc, char** argv)
{
	gridsmap_t test_map_1, test_map_2, test_map_3, test_map_4;
	
	test_map_1[ "_event" ] = "foo";
	test_map_1[ "rot" ] = "x111";
	test_map_1[ "pos" ] = "x222";
	test_map_1[ "scale" ] = "x444";
	
	test_map_2[ "_event" ] = "bar";
	test_map_2[ "rot" ] = "x555";
	test_map_2[ "pos" ] = "x666";
	test_map_2[ "scale" ] = "x777";
	
	test_map_3[ "_event" ] = "bing";
	test_map_3[ "rot" ] = "x88";
	test_map_3[ "pos" ] = "x9";
	test_map_3[ "scale" ] = "x0";
	
	test_map_4[ "_event" ] = "laa";
	test_map_4[ "rot" ] = "x23";
	test_map_4[ "pos" ] = "x45";
	test_map_4[ "scale" ] = "x56";
	
	GQueue test_queue;
	
	test_queue.push( GEvent( "event1", test_map_1 ) );
	test_queue.push( GEvent( "event2", test_map_2 ) );
	test_queue.push( GEvent( "event3", test_map_3 ) );
	test_queue.push( GEvent( "event4", test_map_4 ) );
	
	GEvent temp_event;
	gridsmap_t temp_map;
	giterator temp_iterator;
	vector<string> temp_vector;
	
	/*
	while( test_queue.size() > 0 )
	{
		temp_event = test_queue.pop();
		
		cout << temp_event.getEventType() << endl;
		
		temp_map = temp_event.getMap();
		
		for( giterator it = temp_map.begin(); it != temp_map.end(); it++)
		{
			cout << it->first << " => " << it->second << endl;
		}
			
	}
	*/
	
	while( test_queue.size() > 0 )
	{
		temp_event = test_queue.pop();
		
		cout << temp_event.getEventType() << endl;
		
		temp_map = temp_event.getMap();
		
		for( int i = 0; i < 4; i++ )
		{
			temp_vector = temp_event.pop();
			
			cout << temp_vector[0] << " => " << temp_vector[1] << endl;
		}
			
	}
	
	
	cout << endl << endl << endl;
	
	
	
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

