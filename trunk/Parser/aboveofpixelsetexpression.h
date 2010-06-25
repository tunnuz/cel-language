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

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

#include "pixelsetexpression.h"

#ifndef ABOVEOFPIXELSETEXPRESSION_HH
#define ABOVEOFPIXELSETEXPRESSION_HH

namespace CEL {


class AboveOfPixelSetExpression : public PixelSetExpression
{
	
	PixelSetExpression *m_leftPixelSet;
	PixelSetExpression *m_rightPixelSet;

public :
    AboveOfPixelSetExpression(PixelSetExpression *l, PixelSetExpression *r) : m_leftPixelSet(l), m_rightPixelSet(r) {};
        
        
    // counts the number of pixels in the expression
    virtual PixelSet *render() {
    	
    	PixelSet *leftPixelSet = m_leftPixelSet->render();
    	PixelSet *rightPixelSet = m_rightPixelSet->render();
    	    	    	
		PixelSet *result = leftPixelSet->Above( rightPixelSet );
		
		delete leftPixelSet;
		delete rightPixelSet;
		
    	return  result;
    }    
 
  	std::ostream& print(std::ostream &os) {
		os << "Above(" << *m_leftPixelSet << "," << *m_rightPixelSet << ")";
		return os;
	}

};

}
#endif
