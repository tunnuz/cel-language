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

#include "rendererpsexpression.h"
#include "celparserexception.h"


namespace CEL
{


PixelSet *RendererPixelSetExpression::render()
{
				// handle the case when an argument is a Symbol (functionCall)
				if ( m_callFunction )
				{
					std::cout << "***************************  TEST **********************" << std::endl;  
					// then its a symbol, so we extract the 'TargetExpression' linked to the symbol, and assign it to  m_targetExpression
					Expression *expression = getSymbolTableSingleton() [m_callFunction->getFunctionName() ];
					//std::cout << " expression is " << expression << std::endl;
					//std::cout << " type is " << typeid(*expression).name() << std::endl;

					// we check its really a TargetExpression...
					if ( dynamic_cast<TargetExpression *> ( expression ) )
					{
						m_targetExpression = ( TargetExpression * ) expression;

					}
					else
					{
						throw CelParserException("Argument of Render() needs to be a symbol on a TargetExpression");
					}


				}

				if ( m_targetExpression )
				{
					std::cout << "RendererPixelSetExpression::render()" << std::endl;
					// visit the TargetExpression to get the list of Targets
					CELTargetExpressionVisitor visitor;
					m_targetExpression->visit ( visitor );
					const std::vector<std::string> & vec = visitor.getVectorOfTargets();
					
					bool negated = visitor.isNegated();

					std::cout << " ... nb targets=" << vec.size() << std::endl;
					std::cout << " ...    negated=" << negated << std::endl;

					// TODO: we need to handle a vector as argument to Render
					//	PixelSet *px = Renderer::OgreRenderer->Render ( vec, negated? ALL_BUT_NODE : NODE );
					//
	
					PixelSet *px = Renderer::OgreRenderer->Render ( vec[0], negated? ALL_BUT_NODE : NODE );

					return px;
				}
				else 
				{
					throw CelParserException("Argument of Render() needs to be a TargetExpression");
				}
			}

}
