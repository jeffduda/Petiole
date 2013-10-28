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
#include "itkGraphNodeDegreeCentralityCalculator.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                               GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                                GraphType;
  typedef itk::GraphFileReader<GraphType>                            GraphReaderType;
  typedef itk::GraphNodeDegreeCentralityCalculator<GraphType>        CalculatorType;

  if ( argc < 2 )
    {
    std::cout << "usage: " << argv[0] << "input.csv [0=all_nodes,1=non-zero nodes only] [0=undireted,1=directed-in,2=directed-out] " << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  //std::cout << "Reading file: " << argv[1] << std::endl;

  bool allnodes = true;
  bool directed = false;
  bool dirout = false;


  if ( argc > 2 ) 
    {
    if ( atoi( argv[2] ) > 0)
      {
      allnodes = false;
      }
    }

  if ( argc > 3 ) 
    {
    if ( atoi( argv[3] ) > 0)
      {
      directed = true;
      }
    if ( atoi( argv[3] ) > 1)
      {
      dirout = true;
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
  graph->SetIncomingAndOutgoingEdges();
  CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetGraph( graph );

  graph->SetIsDirected( false );
  
  if ( ! directed ) 
    {
    calc->ComputeUndirectedDegree();
    }
  else
    {
    if ( dirout )
      {
      calc->ComputeOutDegree();
      }
    else
      {
      calc->ComputeInDegree();
      }
    }
  

  float mean = 0.0;
  unsigned long nNodes = 0;
  for (unsigned int i=0; i<calc->GetDegreeCentrality()->Size(); i++)
    {
    //std::cout << calc->GetDegreeCentrality( i ) << " ";
    mean += calc->GetDegreeCentrality( i );
    if ( calc->GetDegreeCentrality(i) > 0 ) 
      {
      ++nNodes;
      }
    }
  if ( allnodes ) 
    {
    mean /= calc->GetDegreeCentrality()->Size();
    }
  else
    {
    mean /= nNodes;
    }
  std::cout << mean << std::endl;

  return EXIT_SUCCESS;
}
