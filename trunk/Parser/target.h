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
#include <string>

#include "targetexpression.h"
#include "callfunction.h"

#ifndef TARGET_HH
#define TARGET_HH

namespace CEL {

class TargetExpressionVisitor;

//* class to maniupulate target names
class Target : public TargetExpression
{

    std::string m_targetName;
	CallFunctionOperator *m_callFunction;

public :
    Target(const std::string &t) : m_targetName(t), m_callFunction(NULL) {
    
    	std::cout << "Adding target " << t << std::endl;
    };
    
    Target(CallFunctionOperator *c) : m_callFunction(c) {
    
    	std::cout << "Adding CallFunction " << std::endl;
    };

    
    virtual void visit (TargetExpressionVisitor &v);
    
	CallFunctionOperator *getCallFunction() { return m_callFunction;}
    
    const std::string & getTargetName() const {
      
      std::cout << "Retreiving target " << m_targetName << std::endl;
      return m_targetName; 
    };

	std::ostream& print(std::ostream &os) {
		os << "\"" << m_targetName << "\"";
		return os;
	}
 
};

}
#endif
