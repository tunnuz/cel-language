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

#include "Renderer.h"
#include <sstream>

using namespace Ogre;

// #define PRINT_SHOTS

/** Frames and view volumes */
int Renderer::frameIndex = 0;
std::vector<SceneNode* > Renderer::auxiliaries = std::vector<SceneNode* >();

Renderer *Renderer::OgreRenderer = NULL;

Renderer::Renderer(SceneManager* sm, int viewportWidth, int viewportHeight): sceneManager(sm), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight)
{
		
	OgreRenderer = this;
	
	// Create a camera for offscreen rendering
	offScreenCamera = sceneManager->createCamera("CEL_OffScreenCamera");
	
	// Create off screen camera node
	SceneNode* cameraNode = sceneManager->createSceneNode("CEL_offScreenCameraNode");
	
	// Attach camera to SceneManager
	sceneManager->getRootSceneNode()->addChild(cameraNode);
	cameraNode->attachObject(offScreenCamera); 	
		
	// Initialise render texture using PF_FLOAT32_R. The PixelFormat
	// choice is due to the fact that apparently there's no C++ data type
	// to handle 16-bit floats. Also, this is just an hint to Ogre, which 
	// in my tests will give us PF_FLOAT32_RGB or PF_FLOAT32_RGBA
	
	imageTexture = Root::getSingletonPtr()->getTextureManager()->createManual(
		"CEL_OSR_Depth_RenderTarget",		// Name of texture
		"Default",							// Name of resource group in which the texture should be created
		TEX_TYPE_2D,						// Texture type
		viewportWidth,						// Width
		viewportHeight,						// Height 
		0,									// number of default mipmaps
		PF_L16,						// Pixel format with one float component of 32 bits.
		TU_RENDERTARGET						// Usage
	);
	
	// Get texture render target, attach it, set auto update to false
	RenderTarget* offScreenRenderTarget = imageTexture->getBuffer()->getRenderTarget();
	offScreenRenderTarget->setAutoUpdated(false);
	Root::getSingletonPtr()->getRenderSystem()->attachRenderTarget(*offScreenRenderTarget);
		
	// Add a viewport to the render target
	Viewport* offScreenViewport = offScreenRenderTarget->addViewport(offScreenCamera);
	offScreenViewport->setSkiesEnabled(false);
	offScreenViewport->setShadowsEnabled(false);
	offScreenViewport->setBackgroundColour( ColourValue(0,0,0,1));
	offScreenViewport->setOverlaysEnabled( false );
	offScreenViewport->setClearEveryFrame ( true );
	
	// Associate the material scheme to the offscreen viewport
	offScreenViewport->setMaterialScheme("CEL_Depth_Scheme");
	
	// Estimate the size of the cube texture in order to have the 
	// same resolution as the planar texture
	
	int extWidth = (int) floor(viewportHeight / offScreenCamera->getFOVy().valueRadians()) * 1.5;
	int evenWidth = (extWidth % 2 == 0) ? extWidth : extWidth + 1;
	
	// Initialize cube texture with the same settings as the previous one.
	cubeTexture = Root::getSingletonPtr()->getTextureManager()->createManual(
		"CEL_OSR_Depth_CubeRenderTarget",	// Name of texture
		"Default",							// Name of resource group in which the texture should be created
		TEX_TYPE_2D,					    // Texture type
		evenWidth,							// Width
		evenWidth,							// Height 
		0,									// number of default mipmaps
		PF_L16,								// Pixel format with one float component of 32 bits.
		TU_RENDERTARGET						// Usage
	);
	

	// Get cube map RenderTargets, set-up them and attach them to the RenderSystem
	offScreenRenderTarget = cubeTexture->getBuffer(0)->getRenderTarget();
	offScreenRenderTarget->setAutoUpdated(false);
	Root::getSingletonPtr()->getRenderSystem()->attachRenderTarget(*offScreenRenderTarget);
		
	// Add a viewport to the render target
	offScreenViewport = offScreenRenderTarget->addViewport(offScreenCamera);
	offScreenViewport->setSkiesEnabled(false);
	offScreenViewport->setShadowsEnabled(false);
	offScreenViewport->setBackgroundColour( ColourValue(0,0,0,1));
	offScreenViewport->setOverlaysEnabled( false );
	offScreenViewport->setClearEveryFrame ( true );
		
	// Associate the material scheme to the offscreen viewport
	offScreenViewport->setMaterialScheme("CEL_Depth_Scheme");

	
	// Prepare materials for off-screen depth rendering, for each
	// material in the scene we add a new technique for depth
	// rendering. This is applied as a scheme.
	
	ResourceManager::ResourceMapIterator mIT = MaterialManager::getSingleton().getResourceIterator();
	
	while (mIT.hasMoreElements()) {
		MaterialPtr mat = (MaterialPtr)(mIT.getNext());
		
		// Enrich with new technique that renders depth
		Technique* depthTech = mat->createTechnique();
		depthTech->setSchemeName("CEL_Depth_Scheme");
		
		// Add a pass to the technique
		Pass* depthPass = depthTech->createPass();
		depthPass->setVertexProgram("Depth_VS");
		depthPass->setFragmentProgram("Depth_FS");
	} 
		
	// Set the visibility flag of all scene nodes to 0xFFFFFFF0
	setVisible( sceneManager->getRootSceneNode(), 0xFFFFFFF0 );
	
}


Renderer::~Renderer()
{
	
	RenderTarget* offScreenRenderTarget = imageTexture->getBuffer()->getRenderTarget();
	
	// Cleanup normal texture
	offScreenRenderTarget->removeAllViewports();
	Root::getSingletonPtr()->getRenderSystem()->destroyRenderTarget(offScreenRenderTarget->getName());
	Root::getSingletonPtr()->getRenderSystem()->destroyRenderTexture(imageTexture->getName());
	
	offScreenRenderTarget = cubeTexture->getBuffer()->getRenderTarget();
	
	// Cleanup cube texture
	offScreenRenderTarget->removeAllViewports();
	Root::getSingletonPtr()->getRenderSystem()->destroyRenderTarget(offScreenRenderTarget->getName());
	Root::getSingletonPtr()->getRenderSystem()->destroyRenderTexture(cubeTexture->getName());
		
	delete offScreenCamera;
	
}


void Renderer::setVisible( SceneNode* sceneNode, uint32 nodeFlag)
{
	
	Ogre::SceneNode::ObjectIterator objectIt = sceneNode->getAttachedObjectIterator();
	while (objectIt.hasMoreElements())
	{
		Entity* ent = static_cast<Entity*>(objectIt.getNext());
		ent->setVisibilityFlags( nodeFlag );
	}
	
	Ogre::SceneNode::ChildNodeIterator childIt = sceneNode->getChildIterator();
	while (childIt.hasMoreElements())
	{
		SceneNode* child = static_cast<SceneNode*>(childIt.getNext());
		setVisible( child, nodeFlag );
	}
}

PixelSet* Renderer::Render(Ogre::String sceneNode, RenderingMode mode)
{
	SceneNode* node = sceneManager->getSceneNode(sceneNode);
	return Render( node, this->renderingCamera, mode);
}


PixelSet* Renderer::Render(SceneNode* sceneNode, Camera* camera, RenderingMode mode)
{

	RenderTarget* offScreenRenderTarget = imageTexture->getBuffer()->getRenderTarget();
	Viewport* offScreenViewport = offScreenRenderTarget->getViewport(0);
	
	//Viewport* defaultViewport = camera->getViewport();
	//defaultVisibilityMask = defaultViewport->getVisibilityMask();

	// We allocate memory for the image where the rendering will be stored
	PixelFormat pf = imageTexture->getFormat();
	
	uchar *data = OGRE_ALLOC_T(
		uchar, 
		imageTexture->getWidth() * imageTexture->getHeight() * imageTexture->getDepth() * PixelUtil::getNumElemBytes(pf), 
		MEMCATEGORY_GENERAL
	);
	
	memset(data, 0, imageTexture->getWidth() * imageTexture->getHeight() * imageTexture->getDepth() * PixelUtil::getNumElemBytes(pf));
		
	Image* renderedImage = new Image();
	renderedImage->loadDynamicImage(
		data, 
		imageTexture->getWidth(), 
		imageTexture->getHeight(), 
		imageTexture->getDepth(), 
		pf, 
		true, 
		1, 
		0
	);
	
	
	/*	We use Ogre Visibility Mask to control what we want to render. 
		This approach has some drawbacks:
		
			-	it could interfere with scenes that use visibility masks for some purpose. 
			-	we are using original materials and lighting settings, therefore rendering 
				could be expensive
	
		The advantage, compared to using specific materials, is that we 
		don't need to create specific materials and we should perform far 
		less state changes.
	 */
	
	/*	Set visibility masks for sceneNode and viewport:
		
			-	if mode == NODE, just set the node visibility flags to 1, 
				and the viewport visibility mask to 1
			-	if mode == ALL, do nothing
			-	if mode == ALL_BUT_NODE, just set the node visibility flags to 1, 
				and keep the previous viewport visibility mask
	*/
	
	String PSName = "R";

	switch (mode) {
	case NODE: 
		
		offScreenViewport->setVisibilityMask(0x0000000F);
		
		setAuxiliariesInvisible(offScreenViewport);
		
		setVisible( sceneNode, 0x0000000F);
		

		PSName = PSName + "(" + sceneNode->getName() + ")";
		break;
				
	case ALL:
		
		setAuxiliariesInvisible(offScreenViewport);
		PSName = PSName + "(scene)";
		break;
		
	case ALL_BUT_NODE:

		if (!isAnAuxiliary(sceneNode))
			setAuxiliariesInvisible(offScreenViewport);
			
		setVisible( sceneNode, 0x0000000F);
		
		PSName = PSName + "(scene-" + sceneNode->getName() + ")";
		break;
	}
	
	// Set the parameters of offScreenCamera 
	
	offScreenCamera->setPosition(camera->getDerivedPosition());
	offScreenCamera->setOrientation(camera->getDerivedOrientation());
	offScreenCamera->setAspectRatio(camera->getAspectRatio());
	offScreenCamera->setFOVy( camera->getFOVy());
	offScreenCamera->setNearClipDistance(camera->getNearClipDistance());
	offScreenCamera->setFarClipDistance(camera->getFarClipDistance());
	offScreenCamera->setProjectionType(PT_PERSPECTIVE);
	
	/* We render the scene to the off-screen RenderTarget */	
	offScreenRenderTarget->update(true);
	
	/* We re-set SceneNode to be visible (using the default flags) */
	setVisible( sceneNode, 0xFFFFFFF0 );
	
	/* We re-set the offScreenViewport mask (using the default mask) */
	offScreenViewport->setVisibilityMask(0xFFFFFFF0);
	
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox());
		
	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("render.png");
	#endif
		
	/* we create a PixelSet from the obtained image and return it */
	return new PixelSet( renderedImage, PSName );
}


PixelSet* Renderer::CubeRender(Ogre::String sceneNode, RenderingMode mode)
{
	
	SceneNode* node = sceneManager->getSceneNode(sceneNode);
	return CubeRender( node, this->renderingCamera, mode);
	
}


PixelSet* Renderer::CubeRender(SceneNode* sceneNode, Camera* camera, RenderingMode mode)
{

	// Pointers
	RenderTarget* offScreenRenderTarget = cubeTexture->getBuffer()->getRenderTarget();
	Viewport* offScreenViewport = offScreenRenderTarget->getViewport(0);
	
	//Viewport* defaultViewport = camera->getViewport();
	//defaultVisibilityMask = defaultViewport->getVisibilityMask();

	// We allocate memory for the image where the rendering will be stored
	PixelFormat pf = cubeTexture->getFormat();
	
	uchar *data = OGRE_ALLOC_T(
		uchar, 
		cubeTexture->getWidth() * cubeTexture->getHeight() * PixelUtil::getNumElemBytes(pf) * 6, 
		MEMCATEGORY_GENERAL
	);
	
	memset(data, 0, cubeTexture->getWidth() * cubeTexture->getHeight() * PixelUtil::getNumElemBytes(pf) * 6);

		
	Image* renderedImage = new Image();
	renderedImage->loadDynamicImage(
		data,
		cubeTexture->getWidth(), 
		cubeTexture->getHeight(),
		cubeTexture->getDepth(),
		pf,
		true,
		6,
		0
	);
	
	/*	We use Ogre Visibility Mask to control what we want to render. 
		This approach has some drawbacks:
		
			-	it could interfere with scenes that use visibility masks for some purpose. 
			-	we are using original materials and lighting settings, therefore rendering 
				could be expensive
	
		The advantage, compared to using specific materials, is that we 
		don't need to create specific materials and we should perform far 
		less state changes.
	 */
	
	/*	Set visibility masks for sceneNode and viewport:
		
			-	if mode == NODE, just set the node visibility flags to 1, 
				and the viewport visibility mask to 1
			-	if mode == ALL, do nothing
			-	if mode == ALL_BUT_NODE, just set the node visibility flags to 1, 
				and keep the previous viewport visibility mask
	*/
	
	String PSName = "CR";

	switch (mode) {
	case NODE: 
		
		offScreenViewport->setVisibilityMask(0x0000000F);
		
		setAuxiliariesInvisible(offScreenViewport);
		
		setVisible( sceneNode, 0x0000000F);
		

		PSName = PSName + "(" + sceneNode->getName() + ")";
		break;
				
	case ALL:
		
		setAuxiliariesInvisible(offScreenViewport);
		PSName = PSName + "(scene)";
		break;
		
	case ALL_BUT_NODE:

		if (!isAnAuxiliary(sceneNode))
			setAuxiliariesInvisible(offScreenViewport);
			
		setVisible( sceneNode, 0x0000000F);
		
		PSName = PSName + "(scene-" + sceneNode->getName() + ")";
		break;
	}
	
	// Set the parameters of offScreenCamera 
	offScreenCamera->setPosition(camera->getDerivedPosition());
	offScreenCamera->setOrientation(camera->getDerivedOrientation());
	offScreenCamera->setFarClipDistance(camera->getFarClipDistance());
	offScreenCamera->setNearClipDistance(0.00001);	
	offScreenCamera->setAspectRatio(1.0);
	offScreenCamera->setFOVy(Degree(90)); 
	offScreenCamera->setProjectionType(PT_PERSPECTIVE);
	
	// Right   Left    Up      Down    Back    Front
	// +X (0), -X (1), +Y (2), -Y (3), +Z (4), -Z (5) 

	/* Get quaternion to rotate horizontally */
	Quaternion rotate = Quaternion(Degree(-90), offScreenCamera->getDerivedUp());
	
	// Front
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(5, 0));
	
	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_front.png");
	#endif
	
	// Right
	offScreenCamera->setOrientation(rotate * offScreenCamera->getOrientation());
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(0, 0));
	
	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_right.png");
	#endif
	
	// Back
	offScreenCamera->setOrientation(rotate * offScreenCamera->getOrientation());
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(4, 0));

	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_back.png");
	#endif
	
	// Left
	offScreenCamera->setOrientation(rotate * offScreenCamera->getOrientation());
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(1, 0));
	
	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_left.png");
	#endif
	
	// Front again
	offScreenCamera->setOrientation(rotate * offScreenCamera->getOrientation());
	
	// Up
	Quaternion look_up = Quaternion(Degree(90), offScreenCamera->getDerivedRight());
	offScreenCamera->setOrientation(look_up * offScreenCamera->getOrientation());
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(2, 0));
	
	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_up.png");
	#endif
	
	// Down
	Quaternion look_down = Quaternion(Degree(-180), offScreenCamera->getDerivedRight());
	offScreenCamera->setOrientation(look_down * offScreenCamera->getOrientation());
	offScreenRenderTarget->update(true);
	offScreenRenderTarget->copyContentsToMemory(renderedImage->getPixelBox(3, 0));

	#ifdef PRINT_SHOTS
	offScreenRenderTarget->writeContentsToFile("cube_down.png");
	#endif
	
	/* We re-set SceneNode to be visible (using the default flags) */
	setVisible( sceneNode, 0xFFFFFFF0 );
	
	/* We re-set the offScreenViewport mask (using the default mask) */
	offScreenViewport->setVisibilityMask(0xFFFFFFF0);

	/* We create a PixelSet from the obtained image and return it */
	return new PixelSet( renderedImage, PSName );

}

String Renderer::addQuadFrame(Vector2 upperLeft, Vector2 lowerRight) {
	
	/** Generate unique name*/
	std::stringstream frameId;
	frameId << "CelRendererFrame_" << ++Renderer::frameIndex;

	/** Create scene node and 2D rectangle */
	SceneNode*		frameNode = sceneManager->getRootSceneNode()->createChildSceneNode(frameId.str());
	Rectangle2D*	frame = new Rectangle2D();

	frame->setCorners(upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);
	frame->setBoundingBox(
		AxisAlignedBox(
			-100000.0 * Vector3::UNIT_SCALE, 
			 100000.0  * Vector3::UNIT_SCALE
	));
			
	frame->setMaterial("CEL/Frame");
	frameNode->attachObject(frame);
		
	Renderer::auxiliaries.push_back( frameNode );

	return frameId.str();
}

bool Renderer::isAnAuxiliary(SceneNode* toCheck) {
	
	std::vector<SceneNode* >::iterator vit;
	
	for (vit = Renderer::auxiliaries.begin(); vit != Renderer::auxiliaries.end(); vit++) {
		if (toCheck == *vit) return true;
	}
	return false;
}

void Renderer::setAuxiliariesInvisible(Viewport* offScreenViewport) {
	
	int invisibleMask = ( offScreenViewport->getVisibilityMask() == 0x0000000F ) ? 0xFFFFFFF0 : 0x0000000F;

	std::vector<SceneNode* >::iterator vit;
	
	for (vit = Renderer::auxiliaries.begin(); vit != Renderer::auxiliaries.end(); vit++) {
		setVisible( *vit, invisibleMask);
	}
}

String Renderer::addViewVolume(String cameraName) {

	std::stringstream cvvId;
	cvvId << "CelViewVolume_" << cameraName;
	
	Camera* camera;
	
	// If view volume already added, return name
	if (sceneManager->hasSceneNode( cvvId.str() ))
		return cvvId.str();
		
	// If camera not present, return error (empty string)
	if (!sceneManager->hasCamera( cameraName) )
		return "";
	else
		camera = sceneManager->getCamera(cameraName);
	

	// Otherwise draw the frustum of the camera
	Vector3 const *worldCorners = camera->getWorldSpaceCorners();

	ManualObject* frustum = sceneManager->createManualObject(cvvId.str() + "MovableObject");
	
	frustum->begin("CEL/Frustum", RenderOperation::OT_TRIANGLE_LIST);
	
	Matrix4 transform = camera->getViewMatrix(true);
	Vector3 *vertexPosition = new Vector3[8];
	
	for(int i = 0; i < 8; i++)
		vertexPosition[i] = transform * worldCorners[i];
	
	// Near
	frustum->position(vertexPosition[0]);
	frustum->position(vertexPosition[1]);
	frustum->position(vertexPosition[2]);
	
	frustum->position(vertexPosition[2]);
	frustum->position(vertexPosition[3]);
	frustum->position(vertexPosition[0]);
	
	// Right
	frustum->position(vertexPosition[0]);
	frustum->position(vertexPosition[3]);
	frustum->position(vertexPosition[4]);
	
	frustum->position(vertexPosition[4]);
	frustum->position(vertexPosition[3]);
	frustum->position(vertexPosition[7]);
	
	// Left
	frustum->position(vertexPosition[1]);
	frustum->position(vertexPosition[5]);
	frustum->position(vertexPosition[2]);
	
	frustum->position(vertexPosition[2]);
	frustum->position(vertexPosition[5]);
	frustum->position(vertexPosition[6]);
	
	// Top
	frustum->position(vertexPosition[4]);
	frustum->position(vertexPosition[5]);
	frustum->position(vertexPosition[0]);
	
	frustum->position(vertexPosition[5]);
	frustum->position(vertexPosition[1]);
	frustum->position(vertexPosition[0]);
	
	// Bottom
	frustum->position(vertexPosition[3]);
	frustum->position(vertexPosition[2]);
	frustum->position(vertexPosition[6]);
	
	frustum->position(vertexPosition[6]);
	frustum->position(vertexPosition[7]);
	frustum->position(vertexPosition[3]);
	
	// Far
	frustum->position(vertexPosition[6]);
	frustum->position(vertexPosition[5]);
	frustum->position(vertexPosition[4]);
	
	frustum->position(vertexPosition[4]);
	frustum->position(vertexPosition[7]);
	frustum->position(vertexPosition[6]);
	
	frustum->end();
	
	// Attach CVV to sceneNode
	SceneNode* camSceneNode = camera->getParentSceneNode();
	SceneNode* frustumNode = 
		camSceneNode->createChildSceneNode(cvvId.str());
		
	frustumNode->attachObject(frustum);
	frustumNode->translate(camera->getDerivedDirection()*0.00001, Node::TS_LOCAL);
		
	Renderer::auxiliaries.push_back( frustumNode );
	
	return cvvId.str();

}




