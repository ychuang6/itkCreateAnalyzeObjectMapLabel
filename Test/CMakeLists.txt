cmake_minimum_required(VERSION 2.6)

PROJECT(CreateObjMapLabelTest)
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)
SET(Libraries
${ITK_LIBRARIES}
)

INCLUDE_DIRECTORIES(${AnalyzeObjectMap_SOURCE_DIR})
INCLUDE_DIRECTORIES(${AnalyzeObjectMapIO_SOURCE_DIR})

FILE(MAKE_DIRECTORY ${ANALYZE_OBJECT_MAP_BINARY_DIR}/Temporary)
SET(TEST_DATA_ROOT     ${CreateObjMapLabelTest_SOURCE_DIR}/Data/Input)
SET(TESTING_OUTPUT_DIR ${ANALYZE_OBJECT_MAP_BINARY_DIR}/Temporary)

INCLUDE_DIRECTORIES( ${CreateObjMapLabelTest_SOURCE_DIR} )

ADD_EXECUTABLE( CreateObjMapLabelTest CreateObjMapLabelTest.cxx )
TARGET_LINK_LIBRARIES( CreateObjMapLabelTest ${Libraries}  AnalyzeObjectMapIO AnalyzeObjectMap)

ADD_TEST( ImageIOStreamingCreateObjMapLabelTest01
   ${EXECUTABLE_OUTPUT_PATH}/CreateObjMapLabelTest
  ${TEST_DATA_ROOT}/2dtest.nii
  ${TESTING_OUTPUT_DIR}/creatingObject.obj
  ${TEST_DATA_ROOT}/contentlist.txt
   )

