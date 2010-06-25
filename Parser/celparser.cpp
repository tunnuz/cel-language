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

#include "celparser.h"
#include "cel_yacc.h"
#include "Ogre.h"

using namespace Ogre;

int yyparse(void);

namespace CEL {



CelParser *CelParser::m_singletonParser = NULL;


CelParser::CelParser(const std::string &s)
	: m_fileName(s)
{        
	//register the singletonParser
	m_singletonParser = this;
	
	//register the global table
	m_globalSymbolTable = this;
	
	m_flexInputBuffers.addBuffer(s);
    	m_flexInputBuffers.createAndSwitchBuffer();    

}

int CelParser::parseFile() 
{
        
    int res = yyparse();
    
    return res;
}

int CelParser::evaluate() {
	std::vector <Expression*>::iterator it;
	m_evaluations.clear();
	for (it = m_celEvaluators.begin(); it != m_celEvaluators.end(); it++) {	

		
		std::cout << std::endl << " Evaluating expression " << **it << std::endl;
		m_evaluations.push_back((*it)->evaluate());		
	}
}

CelParser* CelParser::getSingletonParser() {
  return m_singletonParser;
}

CelParser::~CelParser() 
{    
    for_each (m_celEvaluators.begin(), m_celEvaluators.end(), deleteExpression );
}


unsigned int CelParser::getNbEvaluators() {
	return m_celEvaluators.size();
}
    
const std::vector<double> &CelParser::getEvaluations () {
	return m_evaluations;
}
    
void CelParser::setCurrentInputFile(const std::string &s) {
	m_flexInputBuffers.addBuffer(s);
    	m_flexInputBuffers.createAndSwitchBuffer();  
}
    
std::string CelParser::getCurrentInputFile() {
	return (m_flexInputBuffers.getFileName());
}

void CelParser::printEvaluations() {
	unsigned int i = 0;
	std::vector <double>::iterator it;
	for (it = m_evaluations.begin(); it != m_evaluations.end(); it++) {	
	
		stringstream message;
		message << "Result[" << i++ << "] = " << *it << std::endl;

		LogManager::getSingleton().logMessage(message.str());
	}
}


}

