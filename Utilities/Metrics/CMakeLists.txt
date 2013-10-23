PROJECT(Metrics)

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

#ADD_EXECUTABLE(petioleNormalizeConnectivityMatrix NormalizeConnectivityMatrix.cxx )
#TARGET_LINK_LIBRARIES(petioleNormalizeConnectivityMatrix
# ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

#ADD_EXECUTABLE(petioleGraphDensity GraphDensity.cxx )
#TARGET_LINK_LIBRARIES(petioleGraphDensity
# ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

#ADD_EXECUTABLE(petioleMeanNodeDegree MeanNodeDegree.cxx )
#TARGET_LINK_LIBRARIES(petioleMeanNodeDegree
# ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleMeanNodeClusteringCoefficient MeanNodeClusteringCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleMeanNodeClusteringCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphDensityThreshold GraphDensityThreshold.cxx )
TARGET_LINK_LIBRARIES(petioleGraphDensityThreshold
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

#ADD_EXECUTABLE(petioleCharacteristicPathLength CharacteristicPathLength.cxx )
#TARGET_LINK_LIBRARIES(petioleCharacteristicPathLength
# ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )
