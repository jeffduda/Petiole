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
#include "itkNWBDataFileReader.h"

int itkNWBDataFileReaderTest( int argc, char * argv [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::PointSet<float,3> MeshType;
  typedef itk::NWBDataFileReader<MeshType> MeshReaderType;

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
  std::cout << "Number of Nodes            = " << reader->GetOutput()->GetNumberOfPoints() << std::endl;
  std::cout << "Number of Directed Edges   = " << reader->GetDirectedEdges()->Size() << std::endl;
  std::cout << "Number of Undirected Edges = " << reader->GetUndirectedEdges()->Size() << std::endl;

  std::cout << "Success" << std::endl;
  return EXIT_SUCCESS;
}
