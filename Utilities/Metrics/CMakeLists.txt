PROJECT(Metrics)

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
${ITKNetworkToolkit_LIBRARIES} ${ITK_LIBRARIES} )
 
