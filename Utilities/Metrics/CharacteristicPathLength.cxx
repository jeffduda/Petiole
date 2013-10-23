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
#include "itkDijkstrasGraphTraits.h"
#include "itkGraph.h"
#include "itkGraphFileReader.h"
#include "itkDijkstrasDistanceMatrixGraphFilter.h"


int main( int argc, char * argv [] )
{
  typedef itk::ImageGraphTraits<float,3>                           ImageGraphTraits;
  typedef itk::DijkstrasGraphTraits<float,3>                       GraphTraitsType;
  typedef itk::Graph<ImageGraphTraits>                             GraphType;
  typedef itk::GraphFileReader<GraphType>                          GraphReaderType;
  typedef itk::Graph<GraphTraitsType>                              SearchGraphType;
  typedef itk::DijkstrasDistanceMatrixGraphFilter<SearchGraphType> FilterType;

  if ( argc < 2 )
    {
    std::cout << "usage: " << argv[0] << "input.csv [0=unweighted,1=weighted] " << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  std::cout << "Reading file: " << argv[1] << std::endl;

  bool weighted = false;
  if ( argc > 2 ) 
    {
    if ( atoi( argv[2] ) > 0 ) 
      {
      weighted = true;
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

  SearchGraphType::Pointer dgraph = SearchGraphType::New();
  for (unsigned long i=0; i<graph->GetTotalNumberOfNodes(); i++)
    {
    dgraph->CreateNewNode();
    }
  for (unsigned long i=0; i<graph->GetTotalNumberOfEdges(); i++)
    {
    SearchGraphType::EdgePointerType edge = dgraph->CreateNewEdge();
    edge->SourceIdentifier = graph->GetEdgePointer(i)->SourceIdentifier;
    edge->TargetIdentifier = graph->GetEdgePointer(i)->TargetIdentifier;
    if ( weighted ) 
      {
      edge->Weight = graph->GetEdgePointer(i)->Weight;
      }
    else
      {
      edge->Weight = 1.0;
      }
    }

    }
    
  FilterType::Pointer dijkstras = FilterType::New();
  dijkstras->SetInput( dgraph );
  dijkstras->Update();

  std::cout << dijkstras->GetCharacteristicPathLength() << std::endl;

  return EXIT_SUCCESS;
}
