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

#include "target.h"
#include "vectoroftargets.h"
#include "targetnegation.h"
#include "targetexpression.h"
#include "targetfunction.h"
#include "quadframe.h"
#include "celparserexception.h"
#include "viewvolume.h"

#ifndef TARGETEXPRESSIONVISITOR_HH
#define TARGETEXPRESSIONVISITOR_HH

namespace CEL {

//* abstract class to maniupulate the Syntaxic tree of PixelSetExpression
class TargetExpressionVisitor
{
 
public :
    TargetExpressionVisitor() {};
    
    virtual ~TargetExpressionVisitor() {}

    virtual void visit(Expression &t) = 0;

    virtual void visit(TargetExpression &t) = 0;
    
    virtual void visit(CallFunctionOperator &t) = 0;

    virtual void visit(QuadFrame &t) = 0;

    virtual void visit(ViewVolume &t) = 0;

    virtual void visit(Target &t) = 0;
    
    virtual void visit(VectorOfTargets &t) = 0;
 
    virtual void visit(TargetNegation &t) = 0;

	virtual void visit(TargetFunction &t) = 0;
 
};


class CELTargetExpressionVisitor : public TargetExpressionVisitor {

	std::vector<std::string> m_targets;
	bool m_negated;

public :
    CELTargetExpressionVisitor() {
		m_negated = false;
	};
    
    virtual ~CELTargetExpressionVisitor() {
	}

    virtual void visit(Expression &t) {
    	
    };

    virtual void visit(CallFunctionOperator &t) {
		std::cout << "Visiting a callfunctionoperator behind symbol " << t.getFunctionName() << std::endl;
    	(getSymbolTableSingleton()[t.getFunctionName()])->visit(*this);
    };


    virtual void visit(TargetExpression &t) {
    	
    };

	virtual void visit(QuadFrame &t) {
		m_targets.push_back(t.getFrameName());
	}

	virtual void visit(ViewVolume &t) {
		m_targets.push_back(t.getViewVolumeName());
	}

    
    virtual void visit(Target &t) {    	
		CallFunctionOperator *cfo = t.getCallFunction();
		if (cfo) {
			std::cout << "Visiting the TargetExpression behind symbol " << cfo->getFunctionName() << std::endl;
			
			(getSymbolTableSingleton()[cfo->getFunctionName()])->visit(*this);

		} else 
			m_targets.push_back(t.getTargetName());
	}
    
    virtual void visit(VectorOfTargets &t) {
		VectorOfTargets::iterator it;		

		for (it = t.begin(); it != t.end(); ++it) {
			(*it)->visit(*this);
		}
	}
 
    virtual void visit(TargetNegation &t) {
    	std::cout << " TargetNegation" << std::endl;
		m_negated = true;
		(t.getExpression())->visit(*this);
	}

	virtual void visit(TargetFunction &t) {
    	

		// extract the TargetExpression behind the function, and visit it!
		CallFunctionOperator *cfo = t.getCallFunction();
		TargetExpression *targetExpression = dynamic_cast<TargetExpression*>(getSymbolTableSingleton()[cfo->getFunctionName()]);
		if (targetExpression) {
			// then visit this expression .... (which in turn will add all the targets in targetExpression)
			targetExpression->visit(*this);
		} else {
			throw CelParserException((std::string("Expression ") + cfo->getFunctionName() + " needs to ba a TargetExpression ").c_str());
		}
	}

	const std::vector<std::string> &getVectorOfTargets() { return m_targets;}
	
	bool isNegated() { return m_negated;}
	
};



}
#endif
