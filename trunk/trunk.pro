######################################################################
# Automatically generated by qmake (1.07a) Fri Jun 18 00:56:11 2010
######################################################################

DEPENDPATH += "OgreConsole OgreMax Operators Parser Test tinyxml"
INCLUDEPATH += . OgreConsole OgreMax tinyxml Operators Parser Test

# Input
HEADERS += OgreConsole/OgreConsole.h \
           OgreMax/OgreMaxModel.hpp \
           OgreMax/OgreMaxPlatform.hpp \
           OgreMax/OgreMaxRenderWindowIterator.hpp \
           OgreMax/OgreMaxScene.hpp \
           OgreMax/OgreMaxTypes.hpp \
           OgreMax/OgreMaxUtilities.hpp \
           OgreMax/ProgressCalculator.hpp \
           OgreMax/Version.hpp \
           Operators/PixelSet.h \
           Operators/Renderer.h \
           Parser/aboveofpixelsetexpression.h \
           Parser/basetype.h \
           Parser/belowofpixelsetexpression.h \
           Parser/buffer.h \
           Parser/callfunction.h \
           Parser/cel_yacc.h \
           Parser/celparser.h \
           Parser/comparisionoperator.h \
           Parser/conditionaloperator.h \
           Parser/constant.h \
           Parser/count.h \
           Parser/coveredbypixelsetexpression.h \
           Parser/coveredbypixelsetoperator.h \
           Parser/cuberendererpsexpression.h \
           Parser/distancepixelsetoperator.h \
           Parser/expression.h \
           Parser/leftofpixelsetexpression.h \
           Parser/leftofpixelsetoperator.h \
           Parser/mathoperator.h \
           Parser/minmaxpixelsetoperator.h \
           Parser/operator.h \
           Parser/overlappixelsetexpression.h \
           Parser/pixelsetexpression.h \
           Parser/pixelsetoperator.h \
           Parser/rendererpsexpression.h \
           Parser/rightofpixelsetexpression.h \
           Parser/silhouettepixelsetexpression.h \
           Parser/symbol.h \
           Parser/symboltable.h \
           Parser/target.h \
           Parser/targetexpression.h \
           Parser/targetexpressionvisitor.h \
           Parser/targetnegation.h \
           Parser/tointoperator.h \
           Parser/vectoroftargets.h \
           Test/ExampleApplication.h \
           Test/Test.h \
           tinyxml/tinyxml.h
LEXSOURCES += Parser/cel.l
YACCSOURCES += Parser/cel.y
SOURCES += OgreConsole/OgreConsole.cpp \
           OgreMax/OgreMaxModel.cpp \
           OgreMax/OgreMaxScene.cpp \
           OgreMax/OgreMaxUtilities.cpp \
           OgreMax/ProgressCalculator.cpp \
           OgreMax/Version.cpp \
           Operators/PixelSet.cpp \
           Operators/Renderer.cpp \
           Parser/basetype.cpp \
           Parser/buffer.cpp \
           Parser/cel_lex.cpp \
           Parser/cel_yacc.cpp \
           Parser/celparser.cpp \
           Parser/expression.cpp \
           Parser/main.cpp \
           Parser/operator.cpp \
           Parser/symbol.cpp \
           Parser/symboltable.cpp \
           Parser/target.cpp \
           Parser/targetexpression.cpp \
           Parser/targetnegation.cpp \
           Parser/vectoroftargets.cpp \
           Test/Test.cpp \
           tinyxml/tinyxml.cpp \
           tinyxml/tinyxmlerror.cpp \
           tinyxml/tinyxmlparser.cpp
TEMPLATE = subdirs

SUBDIRS += \
Operators \
Parser \
Test