PROJECT(Metrics)

INCLUDE_DIRECTORIES(${Metrics_SOURCE_DIR})

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )

TARGET_LINK_LIBRARIES(petioleRichClubCoefficient ${ITKNetworkToolkit_LIBRARIES} ${ITK_LIBRARIES} )
 
