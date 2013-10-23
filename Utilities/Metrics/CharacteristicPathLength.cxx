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
#include "itkDijkstrasGraphTraits.h"
#include "itkGraph.h"
#include "itkGraphFileReader.h"
#include "itkDijkstrasDistanceMatrixGraphFilter.h"


int main( int argc, char * argv [] )
{
  typedef itk::DijkstrasGraphTraits<float,3>                 GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                        GraphType;
  typedef itk::GraphFileReader<GraphType>                    GraphReaderType;
  typedef itk::DijkstrasDistanceMatrixGraphFilter<GraphType> FilterType;

  if ( argc < 2 )
    {
    std::cout << "usage: " << argv[0] << "input.csv [1=directed] " << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  std::cout << "Reading file: " << argv[1] << std::endl;

  bool directed = false;
  if ( argc > 2 ) 
    {
    if ( atoi( argv[2] ) > 0 ) 
      {
      directed = true;
      }
    }


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
  graph->SetIsDirected( directed );
  graph->SetIncomingAndOutgoingEdges();
    
  FilterType::Pointer dijkstras = FilterType::New();
  dijkstras->SetInput( graph );
  dijkstras->Update();

  std::cout << dijkstras->GetCharacteristicPathLength() << std::endl;

  return EXIT_SUCCESS;
}
