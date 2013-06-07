PROJECT(Conversion)

INCLUDE_DIRECTORIES(${Conversion_SOURCE_DIR})

ADD_EXECUTABLE(MatlabToNetworkWorkbench MatlabToNetworkWorkbench.cxx )
TARGET_LINK_LIBRARIES(MatlabToNetworkWorkbench
${ITKNetworkToolkit_LIBRARIES} ${ITK_LIBRARIES} )
 
