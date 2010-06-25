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

#ifndef _dRenderer // to avoid duplicate inclusions
#define _dRenderer

#include <Ogre.h>

#include "PixelSet.h"

/** Used to set rendering mode:
 - NODE just the node passed as input is rendered
 - ALL all the scene is rendered
 - ALL_BUT_NODE we render all the scene but the node passed as input)
 */
enum RenderingMode { NODE, ALL, ALL_BUT_NODE };

	
/** @brief Defines an object that is able to render scene nodes and create a Pixel Set,
 i.e. a rendering operator in CEL.
 @remarks this class renders a subpart of a scene into a texture which is then transferred to RAM.
*/	
class Renderer
{
		
public:
	
	
	
	/** Renderer constructor.
	 @param sm					Pointer to the Ogre::SceneManager for the actual scene.
	 @param viewportWidth		Width of the textures to be created.
	 @param viewportHeight		Height of the textures to be created.
	 */
	Renderer(Ogre::SceneManager* sm, int viewportWidth, int viewportHeight);
	
	/** Renderer destructor. */
	~Renderer();
	
	PixelSet* Render(Ogre::String sceneNode, RenderingMode mode=NODE);
	
	/** Renders a SceneNode into a PixelSet of resolution viewportWidth, viewportHeight and returns a pointer to it.
	 @param sceneNode		Pointer to the Ogre::SceneNode we want to render 
	 @param mode			select between NODE (we render just sceneNode), ALL (we render all the scene),
							and ALL_BUT_NODE (we render all the scene but sceneNode)
	 */
	PixelSet* Render(Ogre::SceneNode* sceneNode, Ogre::Camera* camera, RenderingMode mode=NODE);
	
	PixelSet* CubeRender(Ogre::String sceneNode, RenderingMode mode=NODE);
	
	/** Renders a cubemap from a SceneNode into a PixelSet of resolution ??? and returns a pointer to it.
	 @param sceneNode		Pointer to the Ogre::SceneNode we want to render 
	 @param mode			select between NODE (we render just sceneNode), ALL (we render all the scene),
							and ALL_BUT_NODE (we render all the scene but sceneNode)
	 */
	PixelSet* CubeRender(Ogre::SceneNode* sceneNode, Ogre::Camera* camera, RenderingMode mode=NODE);
	
	/** Sets the camera to be used for offscreen rendering
	 @param camera the Ogre camera to be used for offscreen rendering
	 */
	void setCamera(Ogre::Camera* camera ) { this->renderingCamera = camera; }

	/** Adds a screen-aligned quad to the scene. The quad is set to non-visible. 
	 The method returns the name of the scene node to which the quad is attached.
	 @param upperLeft upper left vertex in relative screen coordinates
	 @param lowerRight lower right vertex in relative screen coordinates
	*/
	Ogre::String addQuadFrame( Ogre::Vector2 upperLeft, Ogre::Vector2 lowerRight );

	/** Adds a geometry shaped as the camera frustum to the scene. The frustum is set to non-visible. 
	 The method returns the name of the scene node to which the frustum is attached, or an empty
	 string if cameraName is not found.
	 If the method is called more than once on the same camera, it will just return the name of
	 the frustum node, and not create it again.
	 @param cameraName the camera whose frustum is created
	 @remarks once the frustum is created, it will just update its position / orientation depending
	 on the position/orientation of the related camera, but not update its geometry if there are
	 changes in the camera near/far/FOV.
	*/
	Ogre::String addViewVolume( Ogre::String cameraName );

		
	static Renderer* OgreRenderer;

	
	/** Gets width of offscreen viewport
	 */
	int getViewportWidth()  {return m_viewportWidth;}
	
	/** Gets height of offscreen viewport
	 */
	int getViewportHeight() {return m_viewportHeight;}

	/** Gets the offscreen camera
	 */
	Ogre::Camera* getOffScreenCamera() {return offScreenCamera;}

			
protected:

	/** Incremental index to generate unique frame names */
	static int frameIndex;
	static std::vector<Ogre::SceneNode* > auxiliaries;
		
private:
	
	void setVisible( Ogre::SceneNode* sceneNode, Ogre::uint32 nodeFlag);
	
	/** Pointer to the Ogre::SceneManager for the actual scene. */
	Ogre::SceneManager* sceneManager;
	
	/** Pointer to the Ogre::Camera we use for rendering. */
	Ogre::Camera* offScreenCamera;
	
	/** Pointers to the image and cube RenderTextures */
	Ogre::TexturePtr imageTexture;
	Ogre::TexturePtr cubeTexture;
		
	Ogre::uint32 defaultVisibilityMask;
	
	Ogre::Camera* renderingCamera;
	
	void setAuxiliariesInvisible(Ogre::Viewport* offScreenViewport);
	bool isAnAuxiliary(Ogre::SceneNode* toCheck);
	
	int m_viewportWidth;
	int m_viewportHeight;
	
};
	


#endif

