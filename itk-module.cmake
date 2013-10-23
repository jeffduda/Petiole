set(DOCUMENTATION "This module contains a collection of classes for performing
network analysis. Its clinical motivation is the study of structural and 
and functional connectivity in the brain using graph-based methods. 
However, the functionalities developed here are usable in a wider
range of applications. The Petiole Toolkit is built upon ITK and adds to
it specific classes for computing graph and image features.")

itk_module(ITKPetioleToolkit
  DEPENDS
    ITKImageFunction
    ITKMesh
    ITKIOImageBase
    ITKIONIFTI
    ITKIOCSV
    ITKThresholding
    ITKImageLabel
    ITKMarkovRandomFieldsClassifiers
    ITKZLIB
  TEST_DEPENDS
    ITKTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)

