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

/* include file containing the buffer for flex declaration        */
/* CineCita - File Parsing - Marc Christie - IRIN - 11 May 1999 */

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <vector>

using namespace std;

#ifndef BUFFER_HH
#define BUFFER_HH

#define MAX_INCLUDE_DEPTH 20
#define YY_BUF_SIZE 16384

extern FILE *yyin;
typedef struct yy_buffer_state* CEL_BUFFER_STATE;
CEL_BUFFER_STATE yy_create_buffer(FILE *, int );
void yy_switch_to_buffer(CEL_BUFFER_STATE );
void yy_delete_buffer(CEL_BUFFER_STATE );
extern int yyerror(char *);


namespace CEL {

class Buffer
{
private :
    std::string fileName;
    CEL_BUFFER_STATE flexBuffer;
    int lineno;
    
public :    
    Buffer(const std::string &s);
    ~Buffer();    
    
    void 		createAndSwitchBuffer();
    void 		setLineNo(int line);
    CEL_BUFFER_STATE 	getFlexBuffer();
    const std::string& 	getFileName() const;    
    int 		getLineNo();
    
};




class BufferVector : public std::vector < Buffer* >
{
public:
    void 		addBuffer(const std::string &s);
    int 		bufferSize();
    CEL_BUFFER_STATE	getBuffer();
    void 		setLineNo(int );
    int 		getLineNo();
    void 		createAndSwitchBuffer();
    const std::string&	getFileName() const;    
    void 		retrieveBuffer();    
};

}

#endif
