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

#ifndef _dPixelSet // to avoid duplicate inclusions
#define _dPixelSet

#include "Ogre.h"

/** Use to discern between regular textures and cube maps:
 - PLANAR		regular 2D texture
 - CUBIC		2D texture made up of 6 buffers which represent 
				the six faces of a cube map
 */
enum PixelSetType { PLANAR, CUBIC };

/** Face indexing, follows Ogre cube rendering convention

		Right   Left    Up      Down    Back    Front
	   +X (0), -X (1), +Y (2), -Y (3), +Z (4), -Z (5)
*/	   

enum PixelSetFace {
	RIGHT	= 0,
	LEFT	= 1,
	UP		= 2,
	DOWN	= 3,
	BACK	= 4,
	FRONT	= 5
};

/** @brief Defines a Pixel Set and its methods  
*/	
class PixelSet {
	
public:
	

	
	/** Constructor from a ogre image
	 @param image Pointer to the image containing the pixel set
	 @param label name of the pixel set
	 */
	PixelSet( Ogre::Image* image, Ogre::String label ); 
	
	/** Constructor that creates an empty pixel set, given width, height and pixel format.
	 @param width width of the image containing the pixel set
	 @param height height of the image containing the pixel set
	 @param pf pixel format of the image containing the pixel set
	 @param label name of the pixel set
	*/
	PixelSet( int width, int height, Ogre::PixelFormat pf, Ogre::String label, PixelSetType = PLANAR );
	
	/** Destructor */
	~PixelSet();
	
	/** Returns the z value for the pixel (x,y,face) in the pixel set. 
		If the returned value is 0, there is no pixel with those coordinates 
		in the pixel set.
		
	 @param x x coordinate of the pixel
	 @param y y coordinate of the pixel
	 @param face face of the pixelset, mostly 0, but can be in [0..5]
			if the pixelset is cubic
	 */
	Ogre::ColourValue getPixel(int x, int y, int face = 0) { return this->pixels->getColourAt(x, y, face); }
	
	/** Returns the name of the pixel set */
	Ogre::String getName() { return this->PSname; }
	
	/** Returns a pointer to the image associated to the pixel set */
	Ogre::Image* getImage() { return this->pixels; }
	
	/** Returns the number of pixels in the pixel set */
	int Count() { return PScount; }
	
	/** Returns the max x coordinate of any pixel in the pixel set */
	int Max_x() { return maxX; }
	
	/** Returns the min x coordinate of any pixel in the pixel set */
	int Min_x() { return minX; }
	
	/** Returns the max y coordinate of any pixel in the pixel set */
	int Max_y() { return maxY; }
	
	/** Returns the min y coordinate of any pixel in the pixel set */
	int Min_y() { return minY; }
	
	/** Returns the max z coordinate of any pixel in the pixel set */
	float Max_z() { return maxZ; }
	
	/** Returns the min z coordinate of any pixel in the pixel set */
	float Min_z() { return minZ; }
	
	/** Returns the minimum distance between two pixel sets */
	float Distance( PixelSet* ps);
	
	/** Returns the type of the PixelSet */
	PixelSetType Type() { return type; }
	
	/** Returns Overlap(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 have the same x, y, side coordinates of some pixels in the argument.
	 @param ps reference to the other pixel set with which overlap is computed 
	 */
	PixelSet* Overlap( PixelSet* ps);
	
	/** Returns CoveredBy(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 would be covered by pixels of the argument if we rendered together the subscenes 
	 that produced 'this' and the argument.
	 @param ps reference to the other pixel set with which CoveredBy is computed 
	 */
	PixelSet* CoveredBy( PixelSet* ps);
	
	/** Returns Left(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 are left of any pixels of the argument.
	 @param ps reference to the other pixel set with which Left is computed 
	 */
	PixelSet* Left( PixelSet* ps);
	
	/** Returns Right(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 are right of any pixels of the argument.
	 @param ps reference to the other pixel set with which Right is computed 
	 */
	PixelSet* Right( PixelSet* ps);
	
	/** Returns Above(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 are above of any pixels of the argument.
	 @param ps reference to the other pixel set with which Above is computed 
	 */
	PixelSet* Above( PixelSet* ps);
	
	/** Returns Below(this, ps), i.e. a PixelSet of pixels in 'this' that 
	 are below of any pixels of the argument.
	 @param ps reference to the other pixel set with which Below is computed 
	 */
	PixelSet* Below( PixelSet* ps);
	
	/** Returns a PixelSet which is the silhouette of this pixel set
	 */
	PixelSet* Silhouette();
	
protected:
	
	/** vector of edge pixels, to compute distances efficiently */
	std::vector <std::pair<int,int> > edgePixels;
	
	/** A Ogre::Image object used to store the rendering and access the pixels of the set */
	Ogre::Image* pixels;
	
	/** name of the pixel set */
	Ogre::String PSname;
	
	/** cardinality of the pixel set */
	int PScount;
	
	/** max x value for the pixel set */
	int maxX;
	/** min x value for the pixel set */
	int minX;
	/** max y value for the pixel set */
	int maxY;
	/** min y value for the pixel set */
	int minY;
	/** max z value for the pixel set */
	float maxZ;
	/** min z value for the pixel set */
	float minZ;
	/** average x value for the pixel set */
	float averageX;
	/** average y value for the pixel set */
	float averageY;
	/** average z value for the pixel set */
	float averageZ;
	
	/** PixelSet type (regular or cube map) */
	PixelSetType type;
	
	/** computes edge pixels */
	void FindEdgePixels();
	
	
private:
	
	/** computes pixel set cardinality and min, max values for x, y, z coordinates. */
	void computeStatistics();
	
	/** writes pixel set cardinality and min, max values for x, y, z coordinates to the default log */
	void logStatistics();
	
	/** Computes a relative spatial relation, e.g. Right, Left, Above, Below. More generally, this method
	 computes a new PixelSet with all pixels of this that are inside the given rectangular slice (extremes
	 included)
	 @param label The name of the resulting PixelSet
	 @param sliceMinX minimum X coordinate of the rectangular region
	 @param sliceMaxX maximum X coordinate of the rectangular region
	 @param sliceMinY minimum Y coordinate of the rectangular region
	 @param sliceMaxY maximum Y coordinate of the rectangular region
	 */
	PixelSet* RSL( Ogre::String label, int sliceMinX, int sliceMaxX, int sliceMinY, int sliceMaxY);

};

/** Exception class representing the error which occurs when
	operating with pixelsets.	
*/

class PixelSetException {

public:

	PixelSetException( const char * error ) : error(error) {}
	Ogre::String getError() { return error; }

private:

	Ogre::String error;
};
	

#endif

