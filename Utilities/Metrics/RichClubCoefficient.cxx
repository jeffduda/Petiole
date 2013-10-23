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
#include "itkGraphRichClubCoefficientCalculator.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                       GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                        GraphType;
  typedef itk::GraphFileReader<GraphType>                    GraphReaderType;
  typedef itk::GraphRichClubCoefficientCalculator<GraphType> CalculatorType;

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
    
  GraphType::Pointer graph = reader->GetOutput();
  graph->SetIsDirected( false );

  // For now, mesh holds points only
  std::cout << "Number of Nodes = " << graph->GetTotalNumberOfNodes() << std::endl;
  std::cout << "Number of Edges = " << graph->GetTotalNumberOfEdges() << std::endl;

  CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetGraph( graph );

  std::cout << std::endl << "As undirected weighted graph" << std::endl;
  calc->SetLevel( 1 );
  calc->ComputeWeighted();

  std::cout << "k=1 weighted: " << calc->GetRichClubCoefficient() << std::endl;
  calc->SetLevel( 2 );
  calc->ComputeWeighted();
  std::cout << "k=2 weighted: " << calc->GetRichClubCoefficient() << std::endl;
  calc->SetLevel( 3 );
  calc->ComputeWeighted();
  std::cout << "k=3 weighted: " << calc->GetRichClubCoefficient() << std::endl;   


  return EXIT_SUCCESS;
}
