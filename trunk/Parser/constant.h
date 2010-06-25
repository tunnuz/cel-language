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

#include "operator.h"
#include "Renderer.h"

#ifndef CONSTANT__OPERATOR_HH
#define CONSTANT__OPERATOR_HH

namespace CEL {

class Constant : public Operator
{
	
public :	
	enum ConstantType {
	   IMAGE_WIDTH,
	   IMAGE_HEIGHT
	};
    
    
    Constant(ConstantType m) : m_type(m)  {
        
    };
        
    virtual double evaluate() {
    	
    	switch (m_type) {
    	   case IMAGE_WIDTH : return Renderer::OgreRenderer->getViewportWidth(); break;
    	   case IMAGE_HEIGHT : return Renderer::OgreRenderer->getViewportHeight(); break;
    	}
    	return INFINITY;
    }
 
	std::ostream& print(std::ostream &os) {
    	switch (m_type) {
    	   case IMAGE_WIDTH : os << "[ImageWidth=" << evaluate() << "]"; break;
    	   case IMAGE_HEIGHT : os << "[ImageHeight" << evaluate() << "]"; break;
    	}
		return os;
	}

 private:
     ConstantType m_type;

};

}
#endif
