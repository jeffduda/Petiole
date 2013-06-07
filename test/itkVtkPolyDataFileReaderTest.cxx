/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphTest.cxx,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkPointSet.h"
#include "itkVtkPolyDataFileReader.h"

int itkVtkPolyDataFileReaderTest( int argc, char * argv [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::PointSet<float,3> MeshType;
  typedef itk::VtkPolyDataFileReader<MeshType> MeshReaderType;

  if ( argc < 2 )
    {
    std::cout << "No input provided" << std::endl;
    return EXIT_FAILURE;
    }

  MeshReaderType::Pointer reader = MeshReaderType::New();
  reader->SetFileName( argv[1] );
  
  std::cout << "Reading file: " << argv[1] << std::endl;

  try 
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
    
  // For now, mesh holds points only
  std::cout << "Number of Points             = " << reader->GetOutput()->GetNumberOfPoints() << std::endl;
  std::cout << "Number of Lines              = " << reader->GetLines()->Size() << std::endl;
  std::cout << "Number of Polygons           = " << reader->GetPolygons()->Size() << std::endl;
  std::cout << "Number of Vertices           = " << reader->GetVertices()->Size() << std::endl;
  std::cout << "Number of Triangular Strips  = " << reader->GetTriangularStrips()->Size() << std::endl;
  std::cout << "Success" << std::endl;
  return EXIT_SUCCESS;
}
