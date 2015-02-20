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
#include "itkDefaultGraphTraits.h"
#include "itkGraph.h"
#include "itkMatrixToGraphFilter.h"

int itkMatrixToGraphFilterTest( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{

  typedef itk::DefaultGraphTraits<float>        GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>           GraphType;
  typedef itk::MatrixToGraphFilter<GraphType>   GraphSourceType;
  typedef GraphSourceType::MatrixType MatrixType;

  MatrixType matrix(6,6);
  matrix.Fill(0);
  matrix(0,1) = matrix(1,0) = 7;
  matrix(0,2) = matrix(2,0) = 9;
  matrix(0,5) = matrix(5,0) = 14;
  matrix(1,3) = matrix(3,1) = 15;
  matrix(1,2) = matrix(2,1) = 10;
  matrix(2,5) = matrix(5,2) = 2;
  matrix(2,3) = matrix(3,2) = 11;
  matrix(3,4) = matrix(4,3) = 6;
  matrix(4,5) = matrix(5,4) = 9;

  GraphSourceType::Pointer filter = GraphSourceType::New();
  filter->SetMatrix( matrix );
  filter->Update();

  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  // For now, mesh holds points only
  std::cout << "Number of Nodes = " << filter->GetOutput()->GetTotalNumberOfNodes() << std::endl;
  std::cout << "Number of Edges = " << filter->GetOutput()->GetTotalNumberOfEdges() << std::endl;

  return EXIT_SUCCESS;
}
