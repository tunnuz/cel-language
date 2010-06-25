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
#include "pixelsetexpression.h"
#include "PixelSet.h"
#include "Renderer.h"
#include "targetexpression.h"
#include "targetexpressionvisitor.h"
#include "callfunction.h"

#ifndef RENDERERPSEXPRESSION_HH
#define RENDERERPSEXPRESSION_HH

namespace CEL
{

// standard renderer for the pixel sets...
	class RendererPixelSetExpression : public PixelSetExpression
	{
			TargetExpression 		*m_targetExpression;
			CallFunctionOperator 	*m_callFunction;

		public :
			// contruct with a targetExpression as argument
			RendererPixelSetExpression ( TargetExpression *t ) :m_targetExpression ( t ), m_callFunction ( NULL ) {};

			// or construct with a symbol as argument
			//RendererPixelSetExpression ( CallFunctionOperator *cfo ) :m_targetExpression ( NULL ), m_callFunction ( cfo ) {};

			virtual ~RendererPixelSetExpression() {}

			void setCallFunction ( CallFunctionOperator *op ) {m_callFunction = op;};

			virtual PixelSet *render();

			std::ostream& print(std::ostream &os) {
				os << "R(" << *m_targetExpression << ")";
				return os;
			}

	};

}
#endif
