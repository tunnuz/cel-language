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

// #define LOG_OPERATIONS

#include "PixelSet.h"
#include "float.h"

using namespace Ogre;
using namespace std;

PixelSet::PixelSet( Image* image, String label ) : pixels(image), PSname(label)
{

	// Which type?
	this->type = (image->getNumFaces() == 1) ? PLANAR : CUBIC;
	
	computeStatistics();
	
	#ifdef LOG_OPERATIONS
	this->logStatistics();
	#endif
}

PixelSet::PixelSet( int width, int height, PixelFormat pf, Ogre::String label, PixelSetType type ) :  PSname(label), type(type)
{
	// How many faces?
	int faces = (type == PLANAR) ? 1 : 6;
	
	// We allocate memory for the image where the pixel set will be stored
	uchar *data = OGRE_ALLOC_T(
		uchar, 
		width * height * PixelUtil::getNumElemBytes(pf) * faces, 
		MEMCATEGORY_GENERAL
	);

	// Write black color to the image
	PixelUtil::packColour(0.0f, 0.0f, 0.0f, 0.0f, pf, data);
	
	pixels = new Image();
	pixels->loadDynamicImage(data, width, height, 0, pf, true, faces, 0);
	
	computeStatistics();
	
	//#ifdef LOG_OPERATIONS
	//this->logStatistics();
	//#endif
	
	
}

PixelSet::~PixelSet() 
{
	// Free allocated memory
	// OGRE_FREE( pixels->getData(), MEMCATEGORY_GENERAL);
	delete pixels;
}

PixelSet* PixelSet::Overlap( PixelSet* ps)
{

	// Check if PixelSets are type-compatible (ie. same type)
	if (ps->type != this->type)
		throw PixelSetException("PixelSets incompatible, check PixelSet type.");
	
	// Check if PixelSets are size-compatible
	if (ps->pixels->getWidth() != this->pixels->getWidth() || ps->pixels->getHeight() != this->pixels->getHeight())
		throw PixelSetException("PixeSets incompatible, check PixelSets size.");
	
	// Get PixelFormat and PixelSet type from current image	
	PixelFormat pf = pixels->getFormat();
	PixelSetType pt = this->type;
	
	// How many faces?
	int faces = (pt == PLANAR) ? 1 : 6;
	
	// Build up label for the PixelSet representing the overlap
	String label = "Overlap(" + this->PSname + "," + ps->getName() + ")";
	
	// Create new empty PixelSet 
	PixelSet* result = new PixelSet( pixels->getWidth(), pixels->getHeight(), pf, label, pt);
	
	PixelBox pb	= result->getImage()->getPixelBox();
		
	// Get data address
	uchar * dest = static_cast<uchar*>(pb.data);
	
	for (int i = 0; i < faces; i++) {
		
		int min_x = 0, min_y = 0, max_x = pixels->getWidth(), max_y = pixels->getHeight();
		
		if ( faces ==  1 ) {
			min_x = minX; max_x = maxX;
			min_y = minY; max_y = maxY;
		}
		
		// For all the pixels of the PixelBox
		for (int x = min_x; x<= max_x; x++) {
			for (int y = min_y; y <= max_y; y++) {
				if (( this->getPixel(x,y,i).r > 0 ) && ( ps->getPixel(x,y,i).r > 0 )) {
					
					// We have found a member of the overlap
					size_t offset = 
						i *  pixels->getWidth() * pixels->getHeight() * PixelUtil::getNumElemBytes(pf) +	// Locate face
						y * pixels->getWidth() * PixelUtil::getNumElemBytes(pf) +							// Locate column
						x * PixelUtil::getNumElemBytes(pf);													// Locate row
					PixelUtil::packColour( this->getPixel(x,y,i), pixels->getFormat(), &dest[offset]);
				}
			}
		}
	}
		
	result->computeStatistics();

	#ifdef LOG_OPERATIONS
	result->logStatistics();
	#endif
	
	return result;
}


PixelSet* PixelSet::CoveredBy( PixelSet* ps)
{

	// Check if PixelSets are type-compatible (ie. same type)
	if (ps->type != this->type)
		throw PixelSetException("PixelSets incompatible, check PixelSet type.");
	
	// Check if PixelSets are size-compatible
	if (ps->pixels->getWidth() != this->pixels->getWidth() || ps->pixels->getHeight() != this->pixels->getHeight())
		throw PixelSetException("PixeSets incompatible, check PixelSets size.");
	
	// Get PixelFormat and PixelSet type from current image	
	PixelFormat pf = pixels->getFormat();
	PixelSetType pt = this->type;
	
	// How many faces?
	int faces = (pt == PLANAR) ? 1 : 6;
	
	// Build up label for the PixelSet representing the overlap
	String label = "CoveredBy(" + this->PSname + "," + ps->getName() + ")";

	// Create new empty PixelSet 
	PixelSet* result = new PixelSet( pixels->getWidth(), pixels->getHeight(), pf, label, pt);
	
	PixelBox pb	= result->getImage()->getPixelBox();
		
	// Get data address
	uchar * dest = static_cast<uchar*>(pb.data);
	
	for (int i = 0; i < faces; i++) {
		
		// For all the pixels of the PixelBox
		for (int x = minX; x<= maxX; x++) {
			for (int y = minY; y <= maxY; y++) {
				if (( this->getPixel(x,y,i).r > 0 ) && ( ps->getPixel(x,y,i).r > 0 ) && ( this->getPixel(x,y,i).r > ps->getPixel(x,y,i).r )) {
					
					// We have found a member of the overlap
					size_t offset = 
						i *  pixels->getWidth() * pixels->getHeight() * PixelUtil::getNumElemBytes(pf) +	// Locate face
						y * pixels->getWidth() * PixelUtil::getNumElemBytes(pf) +							// Locate column
						x * PixelUtil::getNumElemBytes(pf);													// Locate row
					PixelUtil::packColour( this->getPixel(x,y,i), pixels->getFormat(), &dest[offset]);
				}
			}
		}
	}

	result->computeStatistics();

	#ifdef LOG_OPERATIONS
	result->logStatistics();
	#endif
	
	return result;
}


PixelSet* PixelSet::RSL( String label, int sliceMinX, int sliceMaxX, int sliceMinY, int sliceMaxY)
{
	
	// create a new empty pixelbox, set all pixels to black 
	PixelFormat pf = pixels->getFormat();
	PixelSet* result = new PixelSet( pixels->getWidth(), pixels->getHeight(), pf, label);
	PixelBox pb = result->getImage()->getPixelBox();
	uchar * dest = static_cast<uchar*>(pb.data);
	
	// check between the given rectangular subregion if there are pixels of this pixel set, add them to returned pixel set
	for (int x = sliceMinX; x<= sliceMaxX; x++)
		for (int y = sliceMinY; y <= sliceMaxY; y++)
			if ( this->getPixel(x,y).r > 0 )
			{
				// we have found a pixel of this which is left of any pixel in ps
				size_t offset = y * pixels->getWidth() * PixelUtil::getNumElemBytes(pf) + x * PixelUtil::getNumElemBytes(pf);
				PixelUtil::packColour( this->getPixel(x,y), pixels->getFormat(), &dest[offset]);
			}
	
	result->computeStatistics();	
	
#ifdef LOG_OPERATIONS
	result->logStatistics();
#endif
	
	return result;
}


PixelSet* PixelSet::Left( PixelSet* ps)
{	
	String label = "Left(" + this->PSname + "," + ps->getName() + ")";
	// we call the RSL method such that it checks for pixels with minX <= x < ps->Min_x() 
	return this->RSL( label, minX, ps->Min_x()-1, minY, maxY );
}


PixelSet* PixelSet::Right( PixelSet* ps)
{
	String label = "Right(" + this->PSname + "," + ps->getName() + ")";
	// we call the RSL method such that it checks for pixels with ps->Max_x() < x <= maxX 
	return this->RSL( label, ps->Max_x() + 1, maxX, minY, maxY);
}


PixelSet* PixelSet::Above( PixelSet* ps)
{
	String label = "Above(" + this->PSname + "," + ps->getName() + ")";
	// we call the RSL method such that it checks for pixels with ps->Max_y() < y <= maxY 
	return this->RSL( label, minX, maxX, ps->Max_y()+1, maxY);
}


PixelSet* PixelSet::Below( PixelSet* ps)
{
	String label = "Below(" + this->PSname + "," + ps->getName() + ")";
	// we call the RSL method such that it checks for pixels with minY <= x < ps->Min_y 
	return this->RSL( label, minX, maxX, minY, ps->Min_y()-1);
}


// this method find edge pixels in a pixel set by checking the eight neighbours
void PixelSet::FindEdgePixels()
{
	
	// not super efficient, we could test the bounding box of the pixel set first,
	// then do not check if the pixel we are trying to access really exists in the image
	
	for (int x = minX; x<= maxX; x++)  
		for (int y = minY; y <= maxY; y++)
			if ( this->getPixel(x,y).r > 0 )
				{
				// we have found a pixel. Now we access the eight neighbours, and as soon as
				// one is not present, we add the present pixel to the edgePixel vector
				if (( (x>0) && (y>0) && (this->getPixel(x-1,y-1).r > 0 )) &&
					( (x>0) && (this->getPixel(x-1,y).r > 0 )) &&
					( (x>0) && (y<pixels->getHeight()-2) && (this->getPixel(x-1,y+1).r > 0 )) &&
					( (y>0) && (this->getPixel(x,y-1).r > 0 )) &&
					( (y<pixels->getHeight()-2) && (this->getPixel(x,y+1).r > 0 )) &&
					( (x<pixels->getWidth()-2) && (y>0) && (this->getPixel(x+1,y-1).r > 0 )) &&
					( (x<pixels->getWidth()-2) && (this->getPixel(x+1,y).r > 0 )) &&
					( (x<pixels->getWidth()-2) && (y<pixels->getHeight()-2) && (this->getPixel(x+1,y+1).r > 0 )))
					;
					else
						edgePixels.push_back(make_pair(x,y));
				}
}



// straighforward and probably stupid implementation
float PixelSet::Distance( PixelSet* ps)
{
	
	// we find boundary pixels for this, if we didn't compute them previously
	if ( edgePixels.size() == 0)
		FindEdgePixels();
	
	if ( ps->edgePixels.size() == 0)
		ps->FindEdgePixels();
	
	int c = 0;

	float minDistance = FLT_MAX;
	std::vector<pair<int,int> >::const_iterator fit;
	std::vector<pair<int,int> >::const_iterator sit;
	
	for ( fit = edgePixels.begin(); fit != edgePixels.end(); ++fit) 
		for ( sit = ps->edgePixels.begin(); sit != ps->edgePixels.end(); ++sit) 
		{
			c++;
			// we compute square distance	
			float sqDistance = Math::Sqr(fit->first - sit->first) + Math::Sqr(fit->second - sit->second);		
			if (sqDistance < minDistance )
				minDistance = sqDistance;
		}
	
	//LogManager::getSingleton().logMessage("count: " + StringConverter::toString(c));
	
	if (minDistance > 0)
		return Math::Sqrt(minDistance);
	else
		return 0;
	
}

PixelSet* PixelSet::Silhouette(){
	
	return 0;
	
	/// to be completed
	
}


void PixelSet::computeStatistics()
{
	
	// How many faces? How much large?
	int faces = (this->type == PLANAR) ? 1 : 6;
	
	// We compute max, min values for all x,y,z coordinates in the pixel set 
	// and we compute the number of pixels in the set.
	
	maxX = -INT_MAX; 
	minX = INT_MAX; 
	maxY = -INT_MAX; 
	minY = INT_MAX; 
	maxZ = -FLT_MAX; 
	minZ = FLT_MAX;
	
	PScount = 0;

	float totalZ = 0;
	float totalX = 0;
	float totalY = 0;
	
	// Iterate through all the faces of the PixelSet
	for (int face = 0; face < faces; face++) {
		
		// Considering all columns
		for (int x = 0; x < pixels->getWidth(); x++) {
			
			// And rows
			for (int y = 0; y < pixels->getHeight(); y++) {
			
				// Get the color-coded z coordinate of each pixel
				float z = pixels->getColourAt(x, y, face).r;
				
				// If depth greater than zero, pixel must be considered
				if ( z > 0.0 ) {
					
					PScount++;
					
					bool relevantFace = (this->type == PLANAR) || (this->type == CUBIC && face == FRONT);

					// Locate minimum-maximum coordinates, only if considering front face
					if (relevantFace) {
					
						if ( x < minX )
							minX = x;
						if (x > maxX )
							maxX = x;
						
						if ( y < minY )
							minY = y;
						if (y > maxY )
							maxY = y;
						
						if ( z < minZ )
							minZ = z;
						if (z > maxZ )
							maxZ = z;

					}
				}
			}
		}	
	}
	
	if ( PScount == 0 ) { 
		maxX = -1; minX = -1; maxY = -1; minY = -1; maxZ = -1; minZ = -1; averageZ = -1;
	} else {
	
		/** I'm not sure about what this code should mean, I don't remember 
		 *  if is mine or someone else's. -- Tommaso */
	
		averageX = totalX / PScount; averageY = totalY / PScount; averageZ = totalZ / PScount;
	}
	
}

void PixelSet::logStatistics()
{
	// How many faces?
	int faces = (type == PLANAR) ? 1 : 6;
	float height	= pixels->getHeight();
	float width	= pixels->getWidth();
	
	LogManager::getSingleton().logMessage("New pixel set created: " + PSname);
	LogManager::getSingleton().logMessage("Pixel set made up by num. faces: " + StringConverter::toString(this->pixels->getNumFaces()));
	LogManager::getSingleton().logMessage("Count: " + StringConverter::toString(PScount));
	LogManager::getSingleton().logMessage("Relative count: " + StringConverter::toString((float)PScount / (float)(faces * pixels->getWidth()*pixels->getHeight())));
	
	LogManager::getSingleton().logMessage("MinX: " + StringConverter::toString(minX) + ", MaxX: " + StringConverter::toString(maxX));
	LogManager::getSingleton().logMessage("MinY: " + StringConverter::toString(minY) + ", MaxY: " + StringConverter::toString(maxY));
	LogManager::getSingleton().logMessage("MinZ: " + StringConverter::toString(minZ) + ", MaxZ: " + StringConverter::toString(maxZ));

	LogManager::getSingleton().logMessage("Relative MinX: " + StringConverter::toString(minX/width) + ", Relative MaxX: " + StringConverter::toString(maxX/width));
	LogManager::getSingleton().logMessage("Relative MinY: " + StringConverter::toString(minY/height) + ", Relative MaxY: " + StringConverter::toString(maxY/height));
	
}


