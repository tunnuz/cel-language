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

#include "symboltable.h"
#include "Renderer.h"
#include "tointoperator.h"
#include "mathoperator.h"
#include "constant.h"
#include "count.h"
#include "rendererpsexpression.h"
#include "minmaxpixelsetoperator.h"

namespace CEL
{


	SymbolTable *SymbolTable::m_globalSymbolTable = NULL;

	// assigns the local args from s with value in *this (if the arg is a functionName)
	// thiis modifies s (and not *this)
	SymbolTable SymbolTable::assignLocalArgs(SymbolTable &s ) {
		HashTable::iterator it;
		SymbolTable temp = s;
		for (it = temp.m_hashTable.begin(); it !=  temp.m_hashTable.end(); it++) {
			CallFunctionOperator *cfo = dynamic_cast<CallFunctionOperator*>(it->second);
			// if its a call, then replace it by the appropriate expression in the global symbole table
			if (cfo) {
				it->second = m_hashTable[cfo->getFunctionName()];
			}
		}
		return temp;
	}


	void SymbolTable::initSymbolTable()
	{
		// extend the symbol table with new functions...

		// just to illustrate the process, we add the ToInt function (that returns the integer part of scalar value)
		//   (note that it still returns the integer value stored in a floating point value)
		// First, one needs to declare the IntegerOperator that derives from CEL::Operator and implements evaluate()
		// Second, we declare the symbol with its parameters (only the number of params is important, not the names)
		// Third we add this symbol to the Expression SymbolTable
		//   (the parameters of ToInt will be automatically added in the parsing)

		{
			Symbol toInt ( "ToInt" );
			// add one paramter
			toInt.addParameter ( "val" );
			// create the Expression
			Operator *toIntOperator = new ToIntOperator();
	
			toIntOperator->push_back(new CallFunctionOperator ( "val" ));
			// and add it to the table
			addSymbol ( toInt, toIntOperator );
		}


//   // lets now extend the parser with the Area symbol which we build from existing symbols parameters
//   Symbol Area("Area");
//   // add one paramter
//   Area.addParameter("T");
//   // create the Expression
//   // Area("T") = Count(Render(T)) / (imageWidth * imageHeight)
//   Operator *e = new MathOperator(MathOperator::MATH_DIVISION);
//   // left operand
//   e->push_back(new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T"))));
//
//   // right operand
//   Operator *mult = new MathOperator(MathOperator::MATH_MULTIPLICATION);
//   mult->push_back(new Constant(Constant::IMAGE_WIDTH));
//   mult->push_back(new Constant(Constant::IMAGE_HEIGHT));
//   e->push_back(mult);
//
//   // and add it to the table
//   addSymbol(Area, e);
//
//
		{
			// WIDTH
			Symbol Width ( "Width" );
			// add one paramter
			Width.addParameter ( "T" );

			// create the Expression
			// Width("T") = (Maxx(Render(T)) - Minx(Render(T))) / imageWidth
			Operator *widthOp = new MathOperator ( MathOperator::MATH_DIVISION );
			
			Operator *minus = new MathOperator ( MathOperator::MATH_MINUS );
			minus->push_back ( new MaxXPixelSetOperator ( new RendererPixelSetExpression ( new Target(new CallFunctionOperator ( "T" ) ) ) ) );
			minus->push_back ( new MinXPixelSetOperator ( new RendererPixelSetExpression ( new Target(new CallFunctionOperator ( "T" ) ) ) ) );

			// left operand
			widthOp->push_back ( minus );

			// right operand
			widthOp->push_back ( new Constant ( Constant::IMAGE_WIDTH ) );

			// and add it to the table
			addSymbol ( Width, widthOp );
		}

//   {
//     // HEIGHT
//   Symbol Height("Height");
//   // add one paramter
//   Height.addParameter("T");
//
//   // create the Expression
//   // Width("T") = (Maxx(Render(T)) - Minx(Render(T))) / imageWidth
//   Operator *heightOp = new MathOperator(MathOperator::MATH_DIVISION);
//
//   Operator *minus = new MathOperator(MathOperator::MATH_MINUS);
//   minus->push_back( new MaxYPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T"))));
//   minus->push_back( new MinYPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T"))));
//
//   // left operand
//   heightOp->push_back( minus );
//
//   // right operand
//   heightOp->push_back(new Constant(Constant::IMAGE_HEIGHT));
//
//   // and add it to the table
//   addSymbol(Height, heightOp);
//   }
//
//
//
//   {
//   // ScreenSeparation
//   Symbol symbol("ScreenSeparation");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // ScreenSeparation(T1,T2) = Distance(Render(T1), Render(T2)) / imageWidth
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new DistancePixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2"))));
//   op->push_back( new Constant(Constant::IMAGE_WIDTH) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//   {
//   // LeftOf
//   Symbol symbol("LeftOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // LeftOf = Count(Left(R(T1), R(T2))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new LeftOfPixelSetExpression(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//
//   {
//   // RightOf
//   Symbol symbol("RightOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // RightOf = Count(Right(R(T1), R(T2))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new RightOfPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//
//   {
//   // AboveOf
//   Symbol symbol("AboveOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // AboveOf = Count(Above(R(T1), R(T2))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new RightOfPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//
//
//   {
//   // BelowOf
//   Symbol symbol("BelowOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // BelowOf = Count(Below(R(T1), R(T2))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new BelowOfPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//   {
//   // AboveOf
//   Symbol symbol("InFrontOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // InFrontOf = Count(CoverdBy(Overlap(R(T1), R(T2)), R(T1)) / Count (Overlap(R(T1),R(T2))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new CoveredByPixelSetOperator(new OverlapPixelSetExpression(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2"))), new RendererPixelSetExpression( new CallFunctionOperator("T1")))));
//
//   op->push_back( new CountPixelSetOperator(new OverlapPixelSetExpression(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }
//
//
//
//   {
//   // InsideOf
//   Symbol symbol("InsideOf");
//   // add one paramter
//   symbol.addParameter("T1");
//   symbol.addParameter("T2");
//
//   // create the Expression
//   // InsideOf = Count(CoveredBy(R(T1), R(T2))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new CoveredByPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }


// ARG: HOW to encode negation of a symbol??
//   {
//   // Occluded
//   Symbol symbol("Occluded");
//   // add one paramter
//   symbol.addParameter("T");
//
//   // create the Expression
//   // Occluded = Count(CoveredBy(R(T), R(!T))) / Count (R(T1))
//   Operator *op = new MathOperator(MathOperator::MATH_DIVISION);
//
//   op->push_back( new CountPixelSetOperator(new CoveredByPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T")), new RendererPixelSetExpression( new CallFunctionOperator("T2")))));
//   op->push_back( new CountPixelSetOperator(new RendererPixelSetExpression( new CallFunctionOperator("T1"))) );
//
//   // and add it to the table
//   addSymbol(symbol, op);
//   }



	}

}

