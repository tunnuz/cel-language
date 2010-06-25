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

#include <iostream>
#include <stdlib.h>
#include "Ogre.h"

#include "operator.h"
#include "target.h"

#include "Renderer.h"

#ifndef VIEWVOLUME_HH
#define VIEWVOLUME_HH

using namespace std;

namespace CEL {

class ViewVolume : public Target
{

	std::string m_viewVolumeName;

public:
  // Constructors

  ViewVolume() : Target("NO_VOLUME"), m_viewVolumeName("NO_VOLUME")
  {		
		//m_viewVolumeName = Renderer::OgreRenderer->addViewVolume(Renderer::OgreRenderer->getOffScreenCamera()->getName());
		
		// Temporary: until the parametrized operator is not ready
		m_viewVolumeName = Renderer::OgreRenderer->addViewVolume("PlayerCam");
		
  }

  const std::string &getViewVolumeName() { 
	return m_viewVolumeName;
  }

  virtual double evaluate() { return INFINITY; }

  virtual void visit(TargetExpressionVisitor &v);

  std::ostream& print(std::ostream &os) {
	os << "ViewVolume";
	return os;
  }
  

};

}

#endif

