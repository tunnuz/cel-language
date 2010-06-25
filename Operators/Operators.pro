

TEMPLATE = lib
DEPENDPATH += ""Parser Operators Test tinyxml""
INCLUDEPATH += . $(OGRE_HOME)/include \
 ../tinyxml

CONFIG += debug dll precompile_header

DESTDIR = build

SUBDIRS = Parser Test

LIBS += -lOgreMain

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../CEL.pch


# Input
HEADERS += ../OgreMax/OgreMaxModel.hpp \
	../OgreMax/OgreMaxModel.hpp \
	../OgreMax/OgreMaxRenderWindowIterator.hpp \
	../OgreMax/OgreMaxTypes.hpp \
	../OgreMax/ProgressCalculator.hpp \
	../OgreMax/OgreMaxPlatform.hpp \
	../OgreMax/OgreMaxScene.hpp \
	../OgreMax/OgreMaxUtilities.hpp \
	../OgreMax/Version.hpp \
	../tinyxml/tinyxml.h \
	PixelSet.h \
        Renderer.h 


SOURCES += ../OgreMax/OgreMaxModel.cpp \
	  ../OgreMax/OgreMaxScene.cpp \
	  ../OgreMax/OgreMaxUtilities.cpp \
	  ../OgreMax/ProgressCalculator.cpp \
	  ../OgreMax/Version.cpp \
	  ../tinyxml/tinyxml.cpp \
	  ../tinyxml/tinyxmlerror.cpp \
	  ../tinyxml/tinyxmlparser.cpp \
	  PixelSet.cpp \
          Renderer.cpp 

CONFIG -= release \
 qt

