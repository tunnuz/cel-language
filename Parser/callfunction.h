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

#include "operator.h"
#include "symboltable.h"

#ifndef CALLFUNCTION_HH
#define CALLFUNCTION_HH

namespace CEL {

/** models a call function : functionName(exp1,...,expn)
    and a call function declaration: functionName(sym1,...symn) = exp
    exp1,...expn are all stored in the vector of expressions 
    sym1,...symn are stored in the localSymbolTable
    */

class TargetExpressionVisitor;

class CallFunctionOperator : public Operator
{

   std::string	m_functionName;
	
   SymbolTable  m_localSymbolTable;
	
public :
    CallFunctionOperator(const std::string & s) : m_functionName(s) {};
        
    /** assigns the function parameter to the expression, + stores it in the vector of expressions */
    void assignParameterInLocalSymbolTable(const std::string &s, Expression *e) {
       m_localSymbolTable.addSymbol(s,e);
       std::cout << " and Storing the expression " << e << std::endl;
       push_back(e);
    }
    
        
    // calls the function, by :
    //  -pushing the parameters on the SymbolTable,
    //  -evaluating the function
    //  -poping the parameters on the SymbolTable
    virtual double evaluate() {
    	
		std::cout << "local symbol table is " << std::endl;
		m_localSymbolTable.printSymbolTable();

		std::cout << "assign values in local table " << std::endl;
		// make the assignments in the local table: replace the right arg (if its a FunctionCall) by the corresponding expression in the table..
		SymbolTable assignedArgs = getSymbolTableSingleton().assignLocalArgs(m_localSymbolTable);
		assignedArgs.printSymbolTable();

    	SymbolTable savedSymbols = getSymbolTableSingleton().pushSymbols(assignedArgs);
		std::cout << " We saved the following symbols " << std::endl;
		savedSymbols.printSymbolTable();
    	
    	std::cout << " Pushing local symbols  ...the context is " << std::endl; 
    	getSymbolTableSingleton().printSymbolTable();
    	
		std::cout << " Call to function " << m_functionName << std::endl;
    	double result = getSymbolTableSingleton()[Symbol(m_functionName)]->evaluate();
    	
		std::cout << " We restore the symbols... " << std::endl; 
    	getSymbolTableSingleton().popSymbols( savedSymbols );
		std::cout << " Restored table is " << std::endl; 
		getSymbolTableSingleton().printSymbolTable();
    	
    	return result;
    }
    
    const std::string& getFunctionName() const {return m_functionName;};
 
	std::ostream& print(std::ostream &os) {
		os << getFunctionName();

		std::vector<Symbol> vec = m_localSymbolTable.getSymbols();
		std::vector<Symbol>::iterator it;
		if (vec.size() > 0) {
			os << "(";
			for (it = vec.begin(); it != vec.end(); it++) {
				(*it).print(os); 
			}
			os << ")";
		}
		return os;
	}

    virtual SymbolTable getSymbolTable() {return m_localSymbolTable;};

	void visit(TargetExpressionVisitor &v);

};

}
#endif
