itk_module_test()
set(ITKNetworkToolkitTests
itkDijkstrasDistanceMatrixGraphFilterTest.cxx
itkDijkstrasPathGraphFilterTest.cxx
itkDijkstrasGraphTraitsTest.cxx
itkDijkstrasUniquePathsGraphFilterTest.cxx
itkExtractKCoreGraphFilterTest.cxx
itkExtractSCoreGraphFilterTest.cxx
itkExtractRichClubGraphFilterTest.cxx
itkExtractSubgraphFilterTest.cxx
itkGraphFileReaderTest.cxx
#itkGraphFileWriterTest.cxx
itkGraphNodeAssortativityCalculatorTest.cxx
itkGraphNodeClusteringCoefficientCalculatorTest.cxx
itkGraphNodeDegreeCentralityCalculatorTest.cxx
itkGraphDensityCalculatorTest.cxx
itkGraphNodeJointDegreeCalculatorTest.cxx
itkGraphNodeNeighborhoodOverlapCalculatorTest.cxx
itkGraphRichClubCoefficientCalculatorTest.cxx
itkGraphNodeStrengthCalculatorTest.cxx
itkGraphTest.cxx
itkGraphTopologicalOverlapCalculatorTest.cxx
itkMatlabLevel5FileReaderTest.cxx
itkMatrixToGraphFilterTest.cxx
itkNWBDataFileReaderTest.cxx
itkNWBDataFileWriterTest.cxx
itkVtkPolyDataFileReaderTest.cxx
)

set(ITK_TEST_DRIVER itkTestDriver)

CreateTestDriver(ITKNetworkToolkit "${ITKNetworkToolkit-Test_LIBRARIES}" "${ITKNetworkToolkitTests}")

set(TEST_DATA_ROOT "${ITKNetworkToolkit_SOURCE_DIR}/Data")
set(TEMP ${ITK_TEST_OUTPUT_DIR})

itk_add_test(NAME itkDijkstrasDistanceMatrixGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkDijkstrasDistanceMatrixGraphFilterTest)
itk_add_test(NAME itkDijkstrasPathGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkDijkstrasPathGraphFilterTest)
itk_add_test(NAME itkDijkstrasGraphTraitsTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkDijkstrasGraphTraitsTest)
itk_add_test(NAME itkDijkstrasUniquePathsGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkDijkstrasUniquePathsGraphFilterTest)
itk_add_test(NAME itkGraphFileReaderTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.vtk)
#itk_add_test(NAME itkGraphFileWriterTest COMMAND
#                   ITKNetworkToolkitTestDriver
#                   itkGraphFileReaderTest
#                   ${ITK_TEST_OUTPUT_DIR}/itkGraphFileWriteTest.vtk)
itk_add_test(NAME itkExtractSubgraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkExtractSubgraphFilterTest)
itk_add_test(NAME itkExtractKCoreGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkExtractKCoreGraphFilterTest)
itk_add_test(NAME itkExtractSCoreGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkExtractSCoreGraphFilterTest)
itk_add_test(NAME itkGraphNodeAssortativityCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeAssortativityCalculatorTest)
itk_add_test(NAME itkGraphNodeClusteringCoefficientCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeClusteringCoefficientCalculatorTest)
itk_add_test(NAME itkGraphNodeDegreeCentralityCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeDegreeCentralityCalculatorTest)
itk_add_test(NAME itkGraphDensityCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphDensityCalculatorTest)
itk_add_test(NAME itkGraphNodeJointDegreeCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeJointDegreeCalculatorTest)
itk_add_test(NAME itkGraphNodeNeighborhoodOverlapCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeNeighborhoodOverlapCalculatorTest)
itk_add_test(NAME itkGraphRichClubCoefficientCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphRichClubCoefficientCalculatorTest)
itk_add_test(NAME itkGraphNodeStrengthCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphNodeStrengthCalculatorTest)
itk_add_test(NAME itkGraphTest COMMAND ITKNetworkToolkitTestDriver
                    itkGraphTest)
itk_add_test(NAME itkGraphTopologicalOverlapCalculatorTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkGraphTopologicalOverlapCalculatorTest)
itk_add_test(NAME itkExtractRichClubGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkExractRichClubGraphFilterTest)
itk_add_test(NAME itkMatlabLevel5FileReaderTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkMatlabLevel5FileReaderTest
                    ${TEST_DATA_ROOT}/cat.mat )
itk_add_test(NAME itkMatrixToGraphFilterTest COMMAND
                    ITKNetworkToolkitTestDriver
                    itkMatrixToGraphFilterTest)
itk_add_test(NAME itkNWBDataFileReaderTest COMMAND
                    ITKNetworkToolkitTestDriver itkNWBDataFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.nwb)
itk_add_test(NAME itkNWBDataFileWriterTest COMMAND
                    ITKNetworkToolkitTestDriver itkNWBDataFileWriterTest
                    ${ITK_TEST_OUTPUT_DIR}/NWBDataFileWriterTest.nwb)
itk_add_test(NAME itkVtkPolyDataFileReaderTest COMMAND
                    ITKNetworkToolkitTestDriver itkVtkPolyDataFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.vtk)

#itk_add_test(NAME itkBoykovGraphCutFilterTest COMMAND ITKNetworkToolkitTestDriver itkBoykovGraphCutFilterTest)


