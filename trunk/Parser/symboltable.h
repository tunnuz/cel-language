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

#include <vector>

#include <ext/hash_map>

#include "expression.h"
#include "symbol.h"


#ifndef SYMBOLTABLE_HH
#define SYMBOLTABLE_HH

using namespace std;


using namespace __gnu_cxx;


namespace CEL {

/** defines a symbol table
    Contains a Singleton that stores all the symbols
  */

struct symboleq
{
  bool operator()(const Symbol &s1, const Symbol &s2) const
  {
    return s1.getSymbolName() == s2.getSymbolName();
  }
};

struct symbolhash
{
  size_t operator()( const Symbol& x ) const
  {
    return hash< const char* >()( x.getSymbolName().c_str() );
  }
};


typedef hash_map <Symbol, Expression *, symbolhash, symboleq > HashTable;


class SymbolTable 
{

	HashTable m_hashTable;	
	
public :
    SymbolTable() {};

    // don't delete expressions in symbole table, use the free function.
    ~SymbolTable() {};


    /** predefines a number of functions in the symbol table 
  	RESTRICTED to functions which operate over an CEL::Expression type and return a CEL::Expression
  	*/
    void initSymbolTable();

    /** adds a symbol, considers the symbol DOES NOT EXIST (check with existsSymbol) */
    void addSymbol(const Symbol &s, Expression *e) {
    	m_hashTable[s] = e;
    }

    /** checks if a symbol exists */
    bool existsSymbol(const Symbol & s) {

    	return (m_hashTable.find(s) != m_hashTable.end());

    }

    const std::vector<Symbol> getSymbols() const {
    	std::vector<Symbol> vec;
    	HashTable::const_iterator it;
    	for (it = m_hashTable.begin(); it != m_hashTable.end(); ++it) {
    		vec.push_back(it->first);
    	}
    	return vec;
    }
        
    /** gets the symbol associated with the string, consifers the symbol does exist */
    const Symbol &getSymbol(const std::string &s) {
    	return m_hashTable.find(s)->first;
    }
        
    /** retrieves an expression, considers the symbol DOES EXIST (check with existsSymbol) */
    Expression * operator[](const Symbol &s) {
    	return m_hashTable[s];
    }

    // save the symbols from  m_hashTable, and overwirte them with s
    SymbolTable pushSymbols( SymbolTable &local ) {
        HashTable::const_iterator it;

		// we save the current table
        SymbolTable temp = *this;

		// and add the local symbols
    	for (it = local.m_hashTable.begin(); it !=  local.m_hashTable.end(); it++) {
    	
			addSymbol(it->first, it->second);

    	}

    	return temp;
    }

    // temporarily add of symbols, by pushing the ones that already exist...
    SymbolTable addSymbols( SymbolTable &local ) {
        HashTable::const_iterator it;
        SymbolTable temp;
    	for (it = local.m_hashTable.begin(); it !=  local.m_hashTable.end(); it++) {
    	
    		// if symbols exist then store its old value in temp
    		if (existsSymbol(it->first)) {
    			temp.addSymbol(it->first, m_hashTable[it->first]);    			
    		}
			addSymbol(it->first, it->second);
    	}

    	return temp;
    }

    // remove all symbols from the local table that appear in the global table
    SymbolTable removeSymbols( SymbolTable &local ) {
        HashTable::const_iterator it;
        SymbolTable temp;
    	for (it = local.m_hashTable.begin(); it !=  local.m_hashTable.end(); it++) {
    	
    		if (existsSymbol(it->first)) {
				m_hashTable.erase(it->first);
    		}
    	}

    	return temp;
    }

 
    // directly copy the symbols from s into the table
    void popSymbols( SymbolTable &s ) {
    	HashTable::iterator it;

		m_hashTable = s.m_hashTable;
		// copy the symboltable s (replaces the current values)
    	//for (it = s.m_hashTable.begin(); it !=  s.m_hashTable.end(); it++) {
    	//	m_hashTable[it->first] = it->second;
    	//}
    } 

	void restoreSymbols(SymbolTable &s ) {
    	HashTable::iterator it;

		// restore the symbols locally stored in s
    	for (it = s.m_hashTable.begin(); it !=  s.m_hashTable.end(); it++) {
    		m_hashTable[it->first] = it->second;
    	}
		
    } 

	// assigns the local args from s with value in *this (if the arg is a functionName)
	// thiis modifies s (and not *this)
	SymbolTable assignLocalArgs(SymbolTable &s );

    void free() {
    	HashTable::iterator it;
    	for (it = m_hashTable.begin(); it !=  m_hashTable.end(); it++) {
    		delete it->second;
    	}
		m_hashTable.clear();
    }

	void clear() {
    	m_hashTable.clear();
	}
    
    void printSymbolTable() {
    	std::cout << "============================" << std::endl;
    	std::cout << "        SymbolTable         " << std::endl;
    	HashTable::iterator it;
    	for (it = m_hashTable.begin(); it != m_hashTable.end(); it++) {
			if (it->second) 
    			std::cout << "  symbol: " << it->first.getSymbolName() << "=\t\t" << *(it->second) << std::endl;
			else 
				std::cout << "  symbol: " << it->first.getSymbolName() << "=\t\t no value " << std::endl;
    	}
    }
    
    static SymbolTable *m_globalSymbolTable;

 
};

static SymbolTable& getSymbolTableSingleton() {return *SymbolTable::m_globalSymbolTable;}


}
#endif
