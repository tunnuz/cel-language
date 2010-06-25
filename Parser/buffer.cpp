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

#include "buffer.h"

namespace CEL {


Buffer::Buffer(const std::string &s)
{      
    fileName = strdup(s.c_str());   
    lineno = 0;
}

void Buffer::createAndSwitchBuffer()
{
    char txt[80];    
    
    if ((yyin = fopen(fileName.c_str(),"r")) == NULL) {
        sprintf(txt,"unable to open include file '%s'",fileName.c_str());        
        yyerror(txt);
    } else {
        flexBuffer = yy_create_buffer(yyin, YY_BUF_SIZE);
        yy_switch_to_buffer(flexBuffer);        
    }    
}


void Buffer::setLineNo(int line) {
  lineno = line;
}

int Buffer::getLineNo() {
  return(lineno);
}



const std::string &Buffer::getFileName() const
{
    return(fileName);    
}

CEL_BUFFER_STATE Buffer::getFlexBuffer() 
{
    return(flexBuffer);    
}


Buffer::~Buffer() 
{

}


void BufferVector::addBuffer (const std::string &s) 
{    
    if (bufferSize() < MAX_INCLUDE_DEPTH)
        this->insert(this->end(),new Buffer(s));
    else yyerror(" Max nested 'include' reached");
    
}

void BufferVector::retrieveBuffer() 
{
    this->pop_back();
}


int BufferVector::bufferSize() 
{
    return this->size();    
}

void BufferVector::createAndSwitchBuffer() 
{
    back()->createAndSwitchBuffer();    
}

void BufferVector::setLineNo(int line)
{
  back()->setLineNo(line);
}

int BufferVector::getLineNo() 
{
  return(back()->getLineNo());
}

CEL_BUFFER_STATE BufferVector::getBuffer()
{
    return(back()->getFlexBuffer());    
}

const std::string &BufferVector::getFileName() const
{
    return(back()->getFileName());    
}

}
