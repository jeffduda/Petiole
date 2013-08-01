PROJECT(Metrics)

INCLUDE_DIRECTORIES(${Metrics_SOURCE_DIR})

ADD_EXECUTABLE(petioleRichClubCoefficient RichClubCoefficient.cxx )
<<<<<<< .merge_file_P7cn2t

TARGET_LINK_LIBRARIES(petioleRichClubCoefficient ${ITKNetworkToolkit_LIBRARIES} ${ITK_LIBRARIES} )
=======
TARGET_LINK_LIBRARIES(petioleRichClubCoefficient
 ${ITKPetioleToolkit_LIBRARIES} ${ITK_LIBRARIES} )
>>>>>>> .merge_file_bVZ1E7
 
