/*
--------------------------------------------------------------------------------
This source file is part of CEL (Camera Evaluation Language), a research project
by Marc Christie (INRIA-Rennes, France), Roberto Ranon and Tommaso Urli (both
from HCI-Lab, University of Udine, Italy).

For the latest info, see http://www.cameracontrol.org/language

Copyright (c) 2010 INRIA Rennes, France and University of Udine, Italy
Also see acknowledgements in Readme.txt

If you use, modify, or simply find this code interesting please let us know
at info@cameracontrol.org.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
--------------------------------------------------------------------------------
*/

#ifndef __Test_h_
#define __Test_h_

#include "ExampleApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#include "../res/resource.h"
#endif

using namespace Ogre;
using Ogre::Node;

#include "OgreNode.h"
#include <OIS/OIS.h>
#include "OgreConsole.h"
#include <iostream>
#include <OgreMaxScene.hpp>
#include <Renderer.h>
#include <celparser.h>
#include <map>

using namespace CEL;

/** Console-related global variables */
String celFile, oldCelFile;	

bool evaluate, quit, load, reload, initial = true, help;
CelParser *cp = 0;

void reloadFile(std::vector<String>& args) {
	reload = true;
}

void helpMe(std::vector<String>& args) {
	help = true;
}

void loadCelFile(std::vector<String>& args) {

	if (args.size() > 1) {
		
		oldCelFile = celFile;
		celFile = args[1];
		
	} else {
		celFile = "";
	}
	
	load = true;
}

void evaluateFile(std::vector<String>& args) {
	evaluate = true;
}

void quitTest(std::vector<String>& args) {
	quit = true;
}
class TestFrameListener : public ExampleFrameListener {

private:	
	
	SceneManager* mSceneMgr;
	Renderer* offScreenR;
	Ogre::Rectangle2D *miniScreen;
	PixelSet* ps1; PixelSet* ps2; PixelSet* ps3;
	Texture* texturePS;
	
	/** Console related */
	bool consoleVisible;
	std::map<OIS::KeyCode, unsigned int> charMap;
	
public:

	
	TestFrameListener(SceneManager *sceneMgr, RenderWindow* win, Camera* cam, Rectangle2D *miniScr, Renderer* rdr, Texture* tx) : 
		ExampleFrameListener(win, cam), mSceneMgr(sceneMgr), miniScreen(miniScr), offScreenR(rdr), texturePS(tx), consoleVisible(false) { 
		
		
		new OgreConsole;
		OgreConsole::getSingleton().init( Ogre::Root::getSingletonPtr(), mKeyboard);
		OgreConsole::getSingleton().addCommand("load", &loadCelFile);
		OgreConsole::getSingleton().addCommand("evaluate", &evaluateFile);
		OgreConsole::getSingleton().addCommand("help", &helpMe);
		OgreConsole::getSingleton().addCommand("reload", &reloadFile);
		OgreConsole::getSingleton().addCommand("quit", &quitTest);
		OgreConsole::getSingleton().addCommand("exit", &quitTest);

		OgreConsole::getSingleton().setVisible(consoleVisible);

		// Enable logging
		OgreConsole::getSingleton().setLogging(true);

		
		oldCelFile = "omniscript";
		celFile = "omniscript";
		
		load = true, help = true;
		
	}
	
	~TestFrameListener() {
		// Disable logging, shutdown
		OgreConsole::getSingleton().setLogging(false);
		OgreConsole::getSingleton().shutdown();
	}
	
	
	bool frameStarted(const FrameEvent& evt) {
	
		if (quit) {
			return false;
		}
	
		processConsoleCommands();
		
		mMoveScale = 0.01;

		// Follow default ExampleFrameListener behavior
		return ExampleFrameListener::frameStarted(evt);
	}
	
	
	bool processUnbufferedMouseInput(const FrameEvent& evt) {
	
		// If console is visible, don't manage mouse movement
		if ( !OgreConsole::getSingleton().isVisible() ) 
			return ExampleFrameListener::processUnbufferedMouseInput(evt);
		else
			return true;
	}	
				
	bool processUnbufferedKeyInput(const FrameEvent& evt) {

		mKeyboard->capture();

		// Enable or disable console
		if(mKeyboard->isKeyDown(OIS::KC_TAB)) {
			if(consoleVisible == false)
				consoleVisible = true;
			else
				consoleVisible = false;
			
			OgreConsole::getSingleton().setVisible(consoleVisible);
			usleep(200000);	
		}
		
		if (!OgreConsole::getSingleton().isVisible()) {
			
			if (mKeyboard->isKeyDown(OIS::KC_E) ){
				evaluate = true;
				OgreConsole::getSingleton().print("> evaluate\n");
				usleep(200000);
				
				return true;
			}
			
			if (mKeyboard->isKeyDown(OIS::KC_R) ){
				reload = true;
				OgreConsole::getSingleton().print("> reload\n");
				usleep(200000);
				
				return true;
			}
			
			return ExampleFrameListener::processUnbufferedKeyInput(evt);
		
		}

		return true;
	}	
	
	void processConsoleCommands() {
		
		if (help) {
			OgreConsole::getSingleton().print("\nCEL Console Commands\n");
			OgreConsole::getSingleton().print("--------------------\n");
			OgreConsole::getSingleton().print("  \"help\" prints this help text,\n");
			OgreConsole::getSingleton().print("  \"load <filename>\" loads a new CEL script from CEL-scripts dir,\n");
			OgreConsole::getSingleton().print("  \"clear\" clears the CEL console,\n");
			OgreConsole::getSingleton().print("  \"evaluate\" or keypress [E] (console closed) evaluates the current CEL script,\n");
			OgreConsole::getSingleton().print("  \"reload\" or keypress [R] (console closed) reloads the current CEL script,\n");
			OgreConsole::getSingleton().print("  \"exit\" or \"quit\" terminates the application,\n");
			OgreConsole::getSingleton().print("  [TAB] toggles the console (console closed/open).\n\n");	
			
			help = false;
		}	
	
		if (load && celFile == "") {
		
			celFile = oldCelFile;
			
			LogManager::getSingleton().logMessage("invalid file name\n");
			LogManager::getSingleton().logMessage("reverted to CEL script " + oldCelFile + "\n");

			load = false;
		
		} else if (load) {
		
			try {
				
				delete cp;
				cp = new CelParser("../Scripts/" + celFile + ".cel");
				cp->initSymbolTable();
				cp->parseFile();
				
				
				LogManager::getSingleton().logMessage("new CEL script set to " + celFile + "\n");
						
			} catch (CelParserException e) {
			
				LogManager::getSingleton().logMessage("CEL parser error: " + e.getError());
			
				celFile = oldCelFile;
				cp = new CelParser("../Scripts/" + celFile + ".cel");
				cp->initSymbolTable();
				cp->parseFile();
			
				LogManager::getSingleton().logMessage("reverted to CEL script " + oldCelFile + "\n");
				
			}
			
			load = false;
		
		}

	
		if (evaluate) {
	
			evaluate = false;
			
			offScreenR->setCamera(mCamera);
					
			try {
				
				if (cp == 0) {
					LogManager::getSingleton().logMessage("invalid CEL script");
				} else {
					cp->evaluate();
					cp->printEvaluations();
					
					if (!OgreConsole::getSingleton().isVisible()) {
						consoleVisible = true;
						OgreConsole::getSingleton().setVisible(true);
					}
					
					LogManager::getSingleton().logMessage("done evaluating.");
				}
			} 
			catch (CelParserException e) {
				LogManager::getSingleton().logMessage("CEL parser error: " + e.getError());
			}
		}
		
		if (reload) {
			
			reload = false;
		
			try {
		
				delete cp;
				cp = new CelParser("../Scripts/" + celFile + ".cel");
				cp->parseFile();
				cp->initSymbolTable();
				
				LogManager::getSingleton().logMessage("new CEL script set to " + celFile + "\n");
						
			} catch (CelParserException e) {
			
				LogManager::getSingleton().logMessage("CEL parser error: " + e.getError());
				
				cp = 0;
			}
		}
	}

	/** We need a sceneNode in order to make frustums inherit the camera's 
	 * direction/position. For this reason the ExampleFrameListener's 
	 * moveCamera method has been redefined in order to handle a SceneNode,
	 * not a Camera. */
	void moveCamera() {
		
		SceneNode* camSceneNode = mCamera->getParentSceneNode();		
	
		camSceneNode->translate(mTranslateVector, Node::TS_LOCAL);
	
		camSceneNode->yaw(mRotX, Node::TS_WORLD);
		camSceneNode->pitch(mRotY);
		
		
	}
};



class TestApp : public ExampleApplication {



public:
	TestApp() {

	}

	~TestApp() {

	}

protected:
		
	Renderer* offScreenR;
	Ogre::Rectangle2D* miniScreen;
	Texture* texturePS;
	
	
	void chooseSceneManager(void) {
		mSceneMgr = mRoot->createSceneManager(ST_EXTERIOR_CLOSE);
    }

	virtual void createCamera(void) {
		
		// Create the camera
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mSceneMgr->getRootSceneNode()->createChildSceneNode("PlayerCam_Node")->attachObject(mCamera);
		mCamera->setNearClipDistance(0.2);
		mCamera->setFarClipDistance(2000.0);
		mSceneMgr->getSceneNode("PlayerCam_Node")->translate(0.0, 4.0, 0.0);

	}

    virtual bool configure(void) {
		if(mRoot->showConfigDialog()) {
            mWindow = mRoot->initialise(true);
			#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				HWND hwnd;
				mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
				LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
				SetClassLong( hwnd, GCL_HICON, iconID );
			#endif
            return true;
        } else {
            return false;
        }
    }


	// Just override the mandatory create scene method
	virtual void createScene(void)	{	
			
		OgreMax::OgreMaxScene().Load("city.scene", mWindow, OgreMax::OgreMaxScene::NO_OPTIONS, mSceneMgr, mSceneMgr->getRootSceneNode());

		int windowWidth, windowHeight;
		
		RenderWindow* rW = Root::getSingleton().getAutoCreatedWindow();
		if (rW){
			windowWidth = rW->getWidth();
			windowHeight = rW->getHeight();
		}
		else {
			windowWidth = 800;
			windowHeight = 600;
		}

		offScreenR = new Renderer( mSceneMgr, windowWidth, windowHeight );
		
		miniScreen = new Ogre::Rectangle2D(true);
		miniScreen->setCorners(0.0, 0.0, 1.0, -1.0);
		miniScreen->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE)); 
		
		// Screen aligned quad to display pixel sets for debug purposes
		Ogre::SceneNode *miniScreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("MiniScreenNode");
		miniScreenNode->attachObject(miniScreen);
		miniScreenNode->setVisible(false);
		
		// Material with off screen rendering as texture, for debug purposes
		MaterialPtr materialR = MaterialManager::getSingleton().create("RttMat", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::Technique *techniqueR = materialR->createTechnique();
		techniqueR->createPass();
		materialR->getTechnique(0)->getPass(0)->setLightingEnabled(false);		
		materialR->getTechnique(0)->getPass(0)->createTextureUnitState("CEL_OSR_Depth_RenderTarget");
				
		// Second material with manual texture, to visualize pixel sets that result from pixel set operators
		texturePS = TextureManager::getSingleton().createManual("PSTexture",
		   "Default",
		   TEX_TYPE_2D, 
		   512, 
		   512, 
		   0, 
		   PF_FLOAT32_RGBA, 
		   Ogre::TU_DYNAMIC
		).getPointer();
		
		MaterialPtr materialPS = MaterialManager::getSingleton().create("PixelSetMat", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::Technique *techniquePS = materialPS->createTechnique();
		techniquePS->createPass();
		materialPS->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		materialPS->getTechnique(0)->getPass(0)->createTextureUnitState("PSTexture");
		miniScreen->setMaterial("PixelSetMat");
		
	}
	

   // Create new frame listener
	void createFrameListener(void) {
		mFrameListener= new TestFrameListener(mSceneMgr, mWindow, mCamera, miniScreen, offScreenR, texturePS);
		mRoot->addFrameListener(mFrameListener);
		mFrameListener->showDebugOverlay(false);
	}
};

#endif // #ifndef __Test_h_