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

  if ( argc < 3 )
    {
    std::cout << "No input provided" << std::endl;
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
    
  unsigned int k = atoi( argv[2] );

  GraphType::Pointer graph = reader->GetOutput();
  graph->SetIsDirected( false );
  graph->SetIncomingAndOutgoingEdges();

  CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetGraph( graph );
  calc->SetLevel( k );

  bool weighted = false;

  if ( argc > 3 ) 
    {
    if ( atoi(argv[3]) > 0 ) 
      {
      weighted = true;
      }
    }

  calc->SetLevel( k );
  if ( weighted ) 
    {
    calc->ComputeWeighted();
    }
  else 
    {
    calc->ComputeUnweighted();
    }

  std::cout << calc->GetRichClubCoefficient() << std::endl;

  return EXIT_SUCCESS;
}
