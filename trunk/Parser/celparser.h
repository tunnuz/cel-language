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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "expression.h"
#include "basetype.h"
#include "callfunction.h"
#include "operator.h"
#include "comparisionoperator.h"
#include "conditionaloperator.h"
#include "mathoperator.h"
#include "symbol.h"
#include "symboltable.h"
#include "buffer.h"
#include "pixelsetexpression.h"
#include "count.h"
#include "coveredbypixelsetexpression.h"
#include "overlappixelsetexpression.h"
#include "cuberendererpsexpression.h"
#include "leftofpixelsetexpression.h"
#include "rightofpixelsetexpression.h"
#include "aboveofpixelsetexpression.h"
#include "belowofpixelsetexpression.h"
#include "silhouettepixelsetexpression.h"
#include "minmaxpixelsetoperator.h"
#include "distancepixelsetoperator.h"
#include "pixelsetoperator.h"
#include "rendererpsexpression.h"
#include "targetexpression.h"
#include "target.h"
#include "targetnegation.h"
#include "vectoroftargets.h"
#include "constant.h"
#include "quadframe.h"


//enum CMPOperator;
using namespace std;

int yyerror(char *msg);
int yyparse();


namespace CEL {




/** @brief The Parser class is used to parse a CEL file. A file contains one to many evaluators. 
    A vector of scalars contains the result of each evaluation
    */
	
class CelParser : public SymbolTable
{  
    std::vector <Expression*> 	m_celEvaluators;
    std::vector <double> 	m_evaluations;
    std::string  		m_fileName;    
    
    
    static CelParser* 		m_singletonParser;
    
public :    
    
    BufferVector	 	m_flexInputBuffers;
    
    /** creates a parser from a file (but does not parse) 
    	@param s : file to parse
    	*/
    CelParser(const std::string &s);  
    
    /** deletes the parser */    
    ~CelParser();  
    
    /** parses the file 
    	@return 0 if fails, 1 if ok
    	*/
    int parseFile();
        
    /** evaluates the camera shots. Evaluations are placed in m_evaluations */
    int evaluate();
    
    unsigned int getNbEvaluators();
    
    const std::vector<double> &getEvaluations ();
    
    void addEvaluator(Expression *e) {m_celEvaluators.push_back(e);};
    
    void setCurrentInputFile(const std::string &s);
    
    std::string getCurrentInputFile();    
            
    static CelParser* getSingletonParser();	
    
	void printEvaluations();
            
};

void SymbolTableInit(CelParser &parser);



}
