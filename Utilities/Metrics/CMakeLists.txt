PROJECT(Metrics)

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

#ADD_EXECUTABLE(petioleNormalizeConnectivityMatrix NormalizeConnectivityMatrix.cxx )
#TARGET_LINK_LIBRARIES(petioleNormalizeConnectivityMatrix
# ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphDensity GraphDensity.cxx )
TARGET_LINK_LIBRARIES(petioleGraphDensity
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleMeanNodeDegree MeanNodeDegree.cxx )
TARGET_LINK_LIBRARIES(petioleMeanNodeDegree
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleMeanNodeClusteringCoefficient MeanNodeClusteringCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleMeanNodeClusteringCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphDensityThreshold GraphDensityThreshold.cxx )
TARGET_LINK_LIBRARIES(petioleGraphDensityThreshold
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleCharacteristicPathLength CharacteristicPathLength.cxx )
TARGET_LINK_LIBRARIES(petioleCharacteristicPathLength
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGlobalEfficiency GlobalEfficiency.cxx )
TARGET_LINK_LIBRARIES(petioleGlobalEfficiency
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleMeanLocalEfficiency MeanLocalEfficiency.cxx )
TARGET_LINK_LIBRARIES(petioleMeanLocalEfficiency
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphDiceCoefficient GraphDiceCoefficient.cxx )
TARGET_LINK_LIBRARIES(petioleGraphDiceCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphVolumeNormalize GraphVolumeNormalize.cxx )
TARGET_LINK_LIBRARIES(petioleGraphVolumeNormalize
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleGraphVolumeLengthNormalize GraphVolumeLengthNormalize.cxx )
TARGET_LINK_LIBRARIES(petioleGraphVolumeLengthNormalize
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleLargestConnectedComponent LargestConnectedComponent.cxx )
TARGET_LINK_LIBRARIES(petioleLargestConnectedComponent
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )

ADD_EXECUTABLE(petioleLargestConnectedComponentSize LargestConnectedComponentSize.cxx )
TARGET_LINK_LIBRARIES(petioleLargestConnectedComponentSize
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )
