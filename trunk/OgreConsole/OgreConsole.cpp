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

#include "OgreConsole.h"
#include <ctime>
 
template<> OgreConsole *Singleton<OgreConsole>::ms_Singleton=0;
 
#define CONSOLE_LINE_LENGTH 85
#define CONSOLE_LINE_COUNT 15
 
OgreConsole::OgreConsole() : start_line(0) {
 	
	/** Populate character map */
	charMap[OIS::KC_0] = '0';
	charMap[OIS::KC_1] = '1';
	charMap[OIS::KC_2] = '2';
	charMap[OIS::KC_3] = '3';
	charMap[OIS::KC_4] = '4';
	charMap[OIS::KC_5] = '5';
	charMap[OIS::KC_6] = '6';
	charMap[OIS::KC_7] = '7';
	charMap[OIS::KC_8] = '8';
	charMap[OIS::KC_9] = '9';
	charMap[OIS::KC_A] = 'a';
	charMap[OIS::KC_B] = 'b';
	charMap[OIS::KC_C] = 'c';
	charMap[OIS::KC_D] = 'd';
	charMap[OIS::KC_E] = 'e';
	charMap[OIS::KC_F] = 'f';
	charMap[OIS::KC_G] = 'g';
	charMap[OIS::KC_H] = 'h';
	charMap[OIS::KC_I] = 'i';
	charMap[OIS::KC_L] = 'l';
	charMap[OIS::KC_M] = 'm';
	charMap[OIS::KC_N] = 'n';
	charMap[OIS::KC_O] = 'o';
	charMap[OIS::KC_P] = 'p';
	charMap[OIS::KC_Q] = 'q';
	charMap[OIS::KC_R] = 'r';
	charMap[OIS::KC_S] = 's';
	charMap[OIS::KC_T] = 't';
	charMap[OIS::KC_U] = 'u';
	charMap[OIS::KC_V] = 'v';
	charMap[OIS::KC_Z] = 'z';
	charMap[OIS::KC_X] = 'x';
	charMap[OIS::KC_Y] = 'y';
	charMap[OIS::KC_J] = 'j';
	charMap[OIS::KC_W] = 'w';
	charMap[OIS::KC_K] = 'k';
	charMap[OIS::KC_RETURN] = '\n';
	charMap[OIS::KC_SPACE] = ' ';
	charMap[OIS::KC_PERIOD] = '.';
	charMap[OIS::KC_BACK] = '\b';
	charMap[OIS::KC_PGUP] = 0;
	charMap[OIS::KC_PGDOWN] = 0;
}
 
OgreConsole::~OgreConsole(){
}
 
void OgreConsole::init(Ogre::Root *root, OIS::Keyboard* mKeyboard){
    
	if(!root->getSceneManagerIterator().hasMoreElements())
       OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "No scene manager found!", "init" );
 
	this->mKeyboard = mKeyboard;
    this->root=root;
    scene=root->getSceneManagerIterator().getNext();
    root->addFrameListener(this);
 
    height = 0.0;
 
    /** Overlay */

	overlay = OverlayManager::getSingleton().getByName("Console");	
	textbox = OverlayManager::getSingleton().getOverlayElement("Console/Text");
	panel = static_cast<Ogre::OverlayContainer*>(OverlayManager::getSingleton().getOverlayElement("Console/Background"));
	
	overlay->show();
	
	lastTypeTime = 0;
	lastKey = OIS::KC_UNASSIGNED;
	
	initialized = true;
	logging = false;
	
	LogManager::getSingleton().getDefaultLog()->addListener(this);
}
 
void OgreConsole::shutdown(){
	if(!initialized)
		return;

	// Remove console from listeners
	root->removeFrameListener(this);
	
 }
 
void OgreConsole::onKeyPressed(const OIS::KeyEvent &arg){
    
	if(!visible)
		return;
	   
    if (arg.key == OIS::KC_RETURN && prompt.length() > 0) {
	
		// Split the parameter list
		const char *str = prompt.c_str();
		vector<String> params;
		String param="";
		for(int c=0;c<prompt.length();c++) {
			if(str[c]==' '){
				if(param.length())
					params.push_back(param);
				param="";
			} 
			else
				param+=str[c];
		}
		if(param.length())
			params.push_back(param);

		
		if (params[0] == "clear") {
			lines.clear();
		} else {
			// Try to execute the command
			map<String,void(*)(vector<String>&)>::iterator i;
			for(i = commands.begin(); i != commands.end(); i++){
				if((*i).first == params[0]){
					if((*i).second) {
						(*i).second(params);
						command_executed = true;
					}
					break;
				}
				command_executed = false;
			}
		}
		
		print("> " + prompt);
		prompt="";

	}
	
	/** Char removal */
    if (arg.key == OIS::KC_BACK)
       prompt=prompt.substr(0,prompt.length()-1);
    
	/** Page up and down */
	if (arg.key == OIS::KC_PGUP) {
       if(start_line > 0)
          start_line--;
    }
	
    if (arg.key == OIS::KC_PGDOWN) {
       if(start_line < lines.size())
          start_line++;
    }
	
	/** Otherwise just write */
    else {
		char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
		for(int c=0;c<sizeof(legalchars);c++){
			if(legalchars[c]==arg.text){					
				prompt += arg.text;
				break;
			}
		}
    }
	
    update_overlay = true;
 }
 
 bool OgreConsole::frameStarted(const Ogre::FrameEvent &evt){
 
	/** Manage visibility */
    if( visible && height < 1) {
       height+=evt.timeSinceLastFrame*2;
       textbox->show();
       if(height>=1){
          height=1;
       }
    }
	
    else if(!visible && height > 0){
       height-=evt.timeSinceLastFrame*2;
       if(height<=0){
          height=0;
          textbox->hide();
       }
    }

			
    textbox->setPosition(0,(height-1)*0.5);
    panel->setPosition(0,(height-1)*0.5);
 
	if (visible) {
 
		// If console is visible, write commands on it
		for (std::map<OIS::KeyCode, unsigned int>::const_iterator mit = charMap.begin(); mit != charMap.end(); mit++) {
		
			clock_t now = clock();
			int wait = 50000;
			
			// Ignore double type if too early
			if ( lastKey == mit->first && (now - lastTypeTime) < 100000 && lastKey != OIS::KC_BACK)
				continue;
				
			if (mit->first == OIS::KC_BACK)
				wait = 100000;
					
			// Pass keyevent to the shell
			if (mKeyboard->isKeyDown(mit->first)) {
				
				// Generate keyevent
				OIS::KeyEvent kevt (
					mKeyboard,					
					mit->first,
					mit->second
				);
				
				lastKey = mit->first;
				lastTypeTime = now;
				
				OgreConsole::getSingleton().onKeyPressed( kevt );
				usleep(wait);
				break;
			}
		}
	}

	if( update_overlay ) {
		String text;
		list<String>::iterator i,start,end;
 
		// Make sure is in range
		if(start_line>lines.size())
			start_line=lines.size();
 
		start=lines.begin();
		for(int c=0;c<start_line;c++)
			start++;
		
		end=start;
		
		for(int c=0;c<CONSOLE_LINE_COUNT;c++){
			if(end==lines.end())
				break;
			end++;
		}
       
		for(i=start;i!=end;i++)
			text+=(*i)+"\n";
 
		if (command_executed)
			text += "  please wait ...";
		else
			text += "$ "+prompt;
 	
		command_executed = false;
	
		textbox->setCaption(text);
		
		update_overlay=false;
	}
    return true;
 }
 
 void OgreConsole::print(const String &text){
    // Subdivide it into lines
    const char *str=text.c_str();
    int len=text.length();
    String line;
    for(int c=0;c<len;c++){
       if(str[c]=='\n'||line.length()>=CONSOLE_LINE_LENGTH){
          lines.push_back(line);
          line="";
       }
       if(str[c]!='\n')
          line+=str[c];
    }
    if(line.length())
       lines.push_back(line);
    if(lines.size()>CONSOLE_LINE_COUNT)
       start_line=lines.size()-CONSOLE_LINE_COUNT;
    else
       start_line=0;
    update_overlay=true;
 }
 
 bool OgreConsole::frameEnded(const Ogre::FrameEvent &evt){
 
    return true;
 }
 
 void OgreConsole::setVisible(bool visible){
    this->visible=visible;
 }
 
 void OgreConsole::setLogging(bool logging){
    this->logging=logging;
 }
 
 void OgreConsole::addCommand(const String &command, void (*func)(vector<String>&)){
    commands[command]=func;
 }
 
 void OgreConsole::removeCommand(const String &command){
    commands.erase(commands.find(command));
 }