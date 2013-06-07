itk_module_test()
set(ITKPetioleToolkitTests
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

CreateTestDriver(ITKPetioleToolkit "${ITKPetioleToolkit-Test_LIBRARIES}" "${ITKPetioleToolkitTests}")

set(TEST_DATA_ROOT "${ITKPetioleToolkit_SOURCE_DIR}/Data")
set(TEMP ${ITK_TEST_OUTPUT_DIR})

itk_add_test(NAME itkDijkstrasDistanceMatrixGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkDijkstrasDistanceMatrixGraphFilterTest)
itk_add_test(NAME itkDijkstrasPathGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkDijkstrasPathGraphFilterTest)
itk_add_test(NAME itkDijkstrasGraphTraitsTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkDijkstrasGraphTraitsTest)
itk_add_test(NAME itkDijkstrasUniquePathsGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkDijkstrasUniquePathsGraphFilterTest)
itk_add_test(NAME itkGraphFileReaderTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.vtk)
#itk_add_test(NAME itkGraphFileWriterTest COMMAND
#                   ITKPetioleToolkitTestDriver
#                   itkGraphFileReaderTest
#                   ${ITK_TEST_OUTPUT_DIR}/itkGraphFileWriteTest.vtk)
itk_add_test(NAME itkExtractSubgraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkExtractSubgraphFilterTest)
itk_add_test(NAME itkExtractKCoreGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkExtractKCoreGraphFilterTest)
itk_add_test(NAME itkExtractSCoreGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkExtractSCoreGraphFilterTest)
itk_add_test(NAME itkGraphNodeAssortativityCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeAssortativityCalculatorTest)
itk_add_test(NAME itkGraphNodeClusteringCoefficientCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeClusteringCoefficientCalculatorTest)
itk_add_test(NAME itkGraphNodeDegreeCentralityCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeDegreeCentralityCalculatorTest)
itk_add_test(NAME itkGraphDensityCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphDensityCalculatorTest)
itk_add_test(NAME itkGraphNodeJointDegreeCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeJointDegreeCalculatorTest)
itk_add_test(NAME itkGraphNodeNeighborhoodOverlapCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeNeighborhoodOverlapCalculatorTest)
itk_add_test(NAME itkGraphRichClubCoefficientCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphRichClubCoefficientCalculatorTest)
itk_add_test(NAME itkGraphNodeStrengthCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphNodeStrengthCalculatorTest)
itk_add_test(NAME itkGraphTest COMMAND ITKPetioleToolkitTestDriver
                    itkGraphTest)
itk_add_test(NAME itkGraphTopologicalOverlapCalculatorTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkGraphTopologicalOverlapCalculatorTest)
itk_add_test(NAME itkExtractRichClubGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkExractRichClubGraphFilterTest)
itk_add_test(NAME itkMatlabLevel5FileReaderTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkMatlabLevel5FileReaderTest
                    ${TEST_DATA_ROOT}/cat.mat )
itk_add_test(NAME itkMatrixToGraphFilterTest COMMAND
                    ITKPetioleToolkitTestDriver
                    itkMatrixToGraphFilterTest)
itk_add_test(NAME itkNWBDataFileReaderTest COMMAND
                    ITKPetioleToolkitTestDriver itkNWBDataFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.nwb)
itk_add_test(NAME itkNWBDataFileWriterTest COMMAND
                    ITKPetioleToolkitTestDriver itkNWBDataFileWriterTest
                    ${ITK_TEST_OUTPUT_DIR}/NWBDataFileWriterTest.nwb)
itk_add_test(NAME itkVtkPolyDataFileReaderTest COMMAND
                    ITKPetioleToolkitTestDriver itkVtkPolyDataFileReaderTest
                    ${TEST_DATA_ROOT}/WikipediaDisjkstrasMesh.vtk)

#itk_add_test(NAME itkBoykovGraphCutFilterTest COMMAND ITKPetioleToolkitTestDriver itkBoykovGraphCutFilterTest)


