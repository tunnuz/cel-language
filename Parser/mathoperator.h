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

#ifndef MATHOPERATOR_HH
#define MATHOPERATOR_HH

namespace CEL {

class MathOperator : public Operator
{
	
public :	
	enum MathOperatorType {
	    MATH_UNARY_MINUS,
	    MATH_UNARY_PLUS,
	    MATH_PLUS,
	    MATH_MINUS,
	    MATH_MULTIPLICATION,
	    MATH_DIVISION,
	    MATH_POWER,
	    MATH_SQRT,
	    MATH_SQR,
	    MATH_SIN,
	    MATH_COS,
	    MATH_TAN,
	    MATH_ACOS,
	    MATH_ASIN,
	    MATH_ATAN,
	    MATH_ATAN2,
	    MATH_EXP	    	 
	};
    
    
    MathOperator(MathOperatorType m) : m_type(m)  {
        
    };
        
    virtual double evaluate() {
    	switch (m_type) {
    	   case MATH_UNARY_MINUS : return -(*begin())->evaluate(); break;
    	   case MATH_UNARY_PLUS : return (*begin())->evaluate(); break;
    	   case MATH_PLUS : return (*begin())->evaluate() + (*(++begin()))->evaluate(); break;
    	   case MATH_MINUS : return (*begin())->evaluate() - (*(++begin()))->evaluate(); break;
    	   case MATH_MULTIPLICATION : return (*begin())->evaluate() * (*(++begin()))->evaluate(); break;
    	   case MATH_DIVISION : return (*begin())->evaluate() / (*(++begin()))->evaluate(); break;
    	   case MATH_SQRT: return sqrt((*begin())->evaluate()); break;
    	   case MATH_SQR: return ((*begin())->evaluate()*(*begin())->evaluate()); break;
    	   case MATH_SIN: return sin((*begin())->evaluate()); break;
    	   case MATH_COS: return cos((*begin())->evaluate()); break;
    	   case MATH_TAN: return tan((*begin())->evaluate()); break;
    	   case MATH_ASIN: return asin((*begin())->evaluate()); break;
    	   case MATH_ACOS: return acos((*begin())->evaluate()); break;
    	   case MATH_ATAN: return atan((*begin())->evaluate()); break;
    	   case MATH_ATAN2: return atan2((*begin())->evaluate(),(*(++begin()))->evaluate()); break;
    	   case MATH_EXP: return exp((*begin())->evaluate()); break;
    	
    	   // TODO : implement power with doubles...
    	   //case POWER : return ((*begin())->evaluate() ^ (*(++begin()))->evaluate(); break;
    	   case MATH_POWER: return INFINITY;

    	}
    	return INFINITY;
    }
 
	std::ostream& print(std::ostream &os) {
		switch (m_type) {
    	   case MATH_UNARY_MINUS : os <<  "-" << **begin(); break;
    	   case MATH_UNARY_PLUS : os << **begin(); break;
    	   case MATH_PLUS : os << **begin() << "+" << **++begin(); break;
    	   case MATH_MINUS : os << **begin() <<  "-" << **(++begin()); break;
    	   case MATH_MULTIPLICATION : os << **begin() << "*" << **(++begin()); break;
    	   case MATH_DIVISION : os << **begin() << "/"  << **(++begin()); break;
    	   case MATH_SQRT: os << "sqrt(" << *(*begin()) << ")"; break;
    	   case MATH_SQR: os << "sqr(" << *(*begin()) << ")"; break;
    	   case MATH_SIN: os << "sin(" << *(*begin()) << ")"; break;
    	   case MATH_COS: os << "cos(" << *(*begin()) << ")"; break;
    	   case MATH_TAN: os << "tan(" << *(*begin()) << ")"; break;
    	   case MATH_ASIN: os << "asin(" << *(*begin()) << ")"; break;
    	   case MATH_ACOS: os << "acos(" << *(*begin()) << ")"; break;
    	   case MATH_ATAN: os << "atan(" << *(*begin()) << ")"; break;
    	   case MATH_ATAN2: os << "atan2(" << *(*begin()) << "," << **(++begin()) << ")"; break;
    	   case MATH_EXP: os << "exp(" << *(*begin()) << ")"; break;
    	
    	   // TODO : implement power with doubles...
    	   //case POWER : return ((*begin())->evaluate() ^ (*(++begin()))->evaluate(); break;
    	   case MATH_POWER: os << endl << "(power not implemented)" << endl;
    	}
		return os;
	}

 private:
     MathOperatorType m_type;

};

}
#endif
