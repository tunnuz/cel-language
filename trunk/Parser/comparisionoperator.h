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

#include "operator.h"

#ifndef COMPARISIONOPERATOR_HH
#define COMPARISIONOPERATOR_HH

namespace CEL {

/** models a comparision between Expressions: exp1 OP exp2
    exp1, and exp2 are all stored in the (inherited) vector of expressions */

class ComparisonOperator : public Operator
{
        
public :        
        
        enum ComparisionOperatorType {
	    EQ,
	    GEQ,
	    LEQ,
	    GREATER,
	    LOWER,
	    DIFFERENT,
	    IN,
	    OUT
	};
    
    	ComparisionOperatorType m_type;
        

    ComparisonOperator(ComparisionOperatorType c) : m_type(c) {};
        
        
        
    // a conditionnal operator returns 0.0 for false, and 1.0 for true
    virtual double evaluate() {
    	switch (m_type) {
    	   case EQ : return (*(begin()))->evaluate() == (*(++begin()))->evaluate(); break;
    	   case GEQ : return (*(begin()))->evaluate() >= (*(++begin()))->evaluate(); break;
    	   case LEQ : return (*(begin()))->evaluate() <= (*(++begin()))->evaluate(); break;
    	   case GREATER : return (*(begin()))->evaluate() > (*(++begin()))->evaluate(); break;
    	   case LOWER : return (*(begin()))->evaluate() < (*(++begin()))->evaluate(); break;
    	   case DIFFERENT : return (*(begin()))->evaluate() != (*(++begin()))->evaluate(); break;
    	   case IN : return ( (*(begin()))->evaluate() >= (*(++begin()))->evaluate() ) &&  ( (*(begin()))->evaluate() <= (*(++(++begin())))->evaluate() ); break;
    	   case OUT : return ( (*(begin()))->evaluate() <= (*(++begin()))->evaluate() ) || ( (*(begin()))->evaluate() >= (*(++(++begin())))->evaluate() ); break;
    	}
    }

	std::ostream& print(std::ostream &os) {
    	switch (m_type) {
    	   case EQ : os << *(*(begin())) << "==" <<  *(*(++begin())) ; break;
    	   case GEQ : os << *(*(begin())) <<  ">=" <<  *(*(++begin())) ; break;
    	   case LEQ : os << *(*(begin())) << "<=" <<  *(*(++begin())) ; break;
    	   case GREATER : os << *(*(begin())) <<  ">" << *(*(++begin())); break;
    	   case LOWER : os <<  *(*(begin())) << "<" << *(*(++begin())); break;
    	   case DIFFERENT : os << *(*(begin())) <<  "!=" << *(*(++begin())); break;
    	   case IN : os <<  *(*(begin())) << " IN [" << *(*(++begin())) << "," << *(*(++(++begin()))); break;
    	   case OUT : os <<  *(*(begin())) << " OUT [" << *(*(++begin())) << "," << *(*(++(++begin()))); break;
		}
		return os;
	}
 
};

}
#endif
