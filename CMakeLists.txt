cmake_minimum_required(VERSION 2.6)
PROJECT(im)
SET(_ ${PROJECT_SOURCE_DIR})
SET(SRC_THIRD ${_}/third/src)
SET(INC_THIRD ${_}/third/include)

SET(EGNET ${_}/egnet)
SET(JSON ${SRC_THIRD}/jsoncpp)
SET(PROTOCAL ${SRC_THIRD}/protocol)
SET(NET ${SRC_THIRD}/net)
SET(UTIL ${SRC_THIRD}/util)

AUX_SOURCE_DIRECTORY(${_} SRC_)

AUX_SOURCE_DIRECTORY(${JSON} SRC_JSON)
AUX_SOURCE_DIRECTORY(${PROTOCAL} SRC_PROTOCAL)
AUX_SOURCE_DIRECTORY(${NET} SRC_NET)
AUX_SOURCE_DIRECTORY(${UTIL} SRC_UTIL)
AUX_SOURCE_DIRECTORY(${EGNET} SRC_EGNET)


#Do not add / behind util cuz there will be util//test.cpp 
#list(REMOVE_ITEM SRC_LIST2 ${_}/../util/test.cpp)
#MESSAGE(STATUS "1....${SRC_LIST1}")
#MESSAGE(STATUS "2....${SRC_LIST2}")
#MESSAGE(STATUS "3....${SRC_LIST3}")

#SET(LIBRARY_OUTPUT_PATH ${_})
#SET(EXECUTABLE_OUTPUT_PATH  ${_})
#if(APPLE)
#    SET(PB_LIB_DIR ${_}/../../base/pb/lib/mac)
#    #list(REMOVE_ITEM SRC_LIST ${_}/playsound.c)
#elseif(UNIX)
#    SET(PB_LIB_DIR ${_}/../../base/pb/lib/linux)
#    #list(REMOVE_ITEM SRC_LIST ${_}/playsound.mm)
#endif()

ADD_DEFINITIONS( -g -Wall -Wno-deprecated -Wno-unused-function -Wno-unused-variable 
-fdiagnostics-color=always -D_REENTRANT -D_FILE_OFFSET_BITS=64 -DAC_HAS_INFO
-DAC_HAS_WARNING -DAC_HAS_ERROR -DAC_HAS_CRITICAL -DTIXML_USE_STL
-DAC_HAS_DEBUG -DLINUX_DAEMON -std=c++11)

INCLUDE_DIRECTORIES(${_} ${INC_THIRD} ${SRC_THIRD} ${JSON} ${PROTOCAL} ${NET} ${UTIL} ${EGNET})
LINK_DIRECTORIES(${_} ${_}/third/lib)

#ADD_XXXX必须在TARGET_LINK_LIBRARIES前面，否则会报错
#ADD_LIBRARY(${PROJECTNAME} SHARED/STATIC ${SRC_LIST})
ADD_EXECUTABLE(im ${SRC_} ${SRC_PROTOCAL} ${SRC_JSON} ${SRC_UTIL} ${SRC_NET} ${SRC_EGNET})

TARGET_LINK_LIBRARIES(im protobuf-lite log4cxx slog pthread curl ssl)
