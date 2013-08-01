PROJECT(Metrics)

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )
 
