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
#include "itkImageGraphTraits.h"
#include "itkGraph.h"
#include "itkGraphFileReader.h"

int itkGraphFileReaderTest( int argc, char * argv [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::ImageGraphTraits<int,3>          GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>           GraphType;
  typedef itk::GraphFileReader<GraphType>       GraphReaderType;

  if ( argc < 2 )
    {
    std::cout << "No input provided" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
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
  std::cout << "Number of Nodes = " << reader->GetOutput()->GetTotalNumberOfNodes() << std::endl;
  std::cout << "Number of Edges = " << reader->GetOutput()->GetTotalNumberOfEdges() << std::endl;

  return EXIT_SUCCESS;
}
