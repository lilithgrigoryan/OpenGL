#!/bin/bash

CC=g++
LDFLAGS=`pkg-config --libs glew`
LDFLAGS="$LDFLAGS -lglut"
CPPFLAGS="$CPPFLAGS -I include -I /usr/include/stb"

$CC cube.cpp src/util.cpp src/glmath.cpp \
    src/Vector3f.cpp src/Matrix4f.cpp src/Scene.cpp src/Widget.cpp src/WidgetFactory.cpp src/Texture.cpp $LDFLAGS $CPPFLAGS -o cube
