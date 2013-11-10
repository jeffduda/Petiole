/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: GraphDensitycxx,v $
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
#include "itkGraphNodeAssortativityCalculator.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                               GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                                GraphType;
  typedef itk::GraphFileReader<GraphType>                            GraphReaderType;
  typedef itk::GraphNodeAssortativityCalculator<GraphType>           CalculatorType;

  if ( argc < 2 )
    {
    std::cout << "usage: " << argv[0] << "input.csv" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );

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
  graph->SetIncomingAndOutgoingEdges();

  CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetGraph( graph );
  calc->Compute();

  std::cout << calc->GetAssortativity() << std::endl;

  return EXIT_SUCCESS;
}
