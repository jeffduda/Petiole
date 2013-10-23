PROJECT(Metrics)

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )

TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleNormalizeConnectivityMatrix NormalizeConnectivityMatrix.cxx )

TARGET_LINK_LIBRARIES(petioleNormalizeConnectivityMatrix
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )
