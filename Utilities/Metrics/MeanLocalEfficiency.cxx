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
#include "itkLargestConnectedComponentGraphFilter.h"


int main( int argc, char * argv [] )
{
  typedef itk::ImageGraphTraits<float,3>                           ImageGraphTraits;
  typedef itk::DijkstrasGraphTraits<float,3>                       GraphTraitsType;
  typedef itk::Graph<ImageGraphTraits>                             GraphType;
  typedef itk::GraphFileReader<GraphType>                          GraphReaderType;
  typedef itk::Graph<GraphTraitsType>                              SearchGraphType;
  typedef itk::DijkstrasDistanceMatrixGraphFilter<SearchGraphType> FilterType;

  typedef itk::LargestConnectedComponentGraphFilter<GraphType,GraphType> ConnectedType;

  if ( argc < 2 )
    {
    std::cout << "usage: " << argv[0] << "input.csv [0=unweighted,1=weighted] " << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  //std::cout << "Reading file: " << argv[1] << std::endl;

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
  graph->SetIsDirected( false );
  graph->SetIncomingAndOutgoingEdges();

  ConnectedType::Pointer connected = ConnectedType::New();
  connected->SetInput( graph );
  connected->Update();

  SearchGraphType::Pointer dgraph = SearchGraphType::New();
  for (unsigned long i=0; i<connected->GetOutput()->GetTotalNumberOfNodes(); i++)
    {
    dgraph->CreateNewNode();
    }
  for (unsigned long i=0; i<connected->GetOutput()->GetTotalNumberOfEdges(); i++)
    {
    SearchGraphType::EdgePointerType edge = dgraph->CreateNewEdge();
    edge->SourceIdentifier = connected->GetOutput()->GetEdgePointer(i)->SourceIdentifier;
    edge->TargetIdentifier = connected->GetOutput()->GetEdgePointer(i)->TargetIdentifier;
    if ( weighted ) 
      {
      edge->Weight = connected->GetOutput()->GetEdgePointer(i)->Weight;
      }
    else
      {
      edge->Weight = 1.0;
      }
    }
    
  FilterType::Pointer dijkstras = FilterType::New();
  dijkstras->SetInput( dgraph );
  dijkstras->Update();

  FilterType::MatrixType distanceMatrix = dijkstras->GetDistanceMatrix();

  float meanNodalEff = 0.0;
  for ( unsigned long i=0; i<connected->GetOutput()->GetTotalNumberOfNodes(); i++) 
    {
    float nodalEff = 0.0;
    float count = 0.0;
    for ( unsigned long j=0; j<connected->GetOutput()->GetTotalNumberOfNodes(); j++)
      {
      if ( j != i ) 
        {
        nodalEff += 1.0 / distanceMatrix(i,j);
        }
      }
    nodalEff /= (connected->GetOutput()->GetTotalNumberOfNodes()-1.0);
    meanNodalEff += nodalEff;
    }

  meanNodalEff /= connected->GetOutput()->GetTotalNumberOfNodes();
  

  std::cout << meanNodalEff << std::endl;

  return EXIT_SUCCESS;
}
