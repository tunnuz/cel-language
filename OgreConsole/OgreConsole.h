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

#pragma once
#include <OgreFrameListener.h>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <list>
#include <vector>

using namespace Ogre;
using namespace std;

class OgreConsole: public Singleton<OgreConsole>, FrameListener, LogListener
{
public:
    OgreConsole();
    ~OgreConsole();
	
    void   init(Ogre::Root *root, OIS::Keyboard* keyb);
    void   shutdown();
	
    void   setVisible(bool visible);
    bool   isVisible(){return visible;}
	
    void   print(const String &text);
	
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual bool frameEnded(const Ogre::FrameEvent &evt);
	
    void onKeyPressed(const OIS::KeyEvent &arg);
	
    void addCommand(const String &command, void (*)(vector<String>&));
    void removeCommand(const String &command);

	void setLogging(bool logging);

    //log
    void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName ) {
		if (logging) print("  " + message);
	}
	
private:
    
	/** General /init usage members */
	bool				visible;
    bool				initialized;
    Root				*root;
    SceneManager		*scene;
	
	/** Overlay console members */
	OverlayContainer	*panel;
    OverlayElement		*textbox;
    Overlay				*overlay;
	float				height;
	bool				command_executed;
    bool				update_overlay;
    
	/** Text-related members */
    int					start_line;
    list<String>		lines;
    String				prompt;
	OIS::KeyCode		lastKey;
	clock_t				lastTypeTime;
	
	bool			logging;
	OIS::Keyboard*	mKeyboard;
	
	map<OIS::KeyCode, unsigned int> charMap;
    map<String,void (*)(vector<String>&)>  commands;
};