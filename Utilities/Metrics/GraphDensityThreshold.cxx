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
#include "itkGraphFileWriter.h"
#include "itkArray.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<float,3>                 GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;
  typedef itk::Array<unsigned long>                      ArrayType;

  if ( argc < 4 )
    {
    std::cout << "USAGE: " << argv[0] << "input.csv density output.csv" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  std::cout << "Reading graph file: " << argv[1] << std::endl;

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
  graph->SetIsDirected(false);
  graph->SetIncomingAndOutgoingEdges();

  float density = atof( argv[2] );
  unsigned int nNodes =  graph->GetTotalNumberOfNodes();
  unsigned int iEdges =  graph->GetTotalNumberOfEdges();
  std::cout << nNodes << " nodes, connected by " << iEdges << " edges" << std::endl;
  
  unsigned int nEdges = density * nNodes * (nNodes-1) / 2.0;
  float indensity =  2.0 * iEdges / ( nNodes * (nNodes-1) );
  if ( indensity < density ) 
    {
    std::cerr << "Input density is less than threshold level, no output written" << std::endl;
    return EXIT_FAILURE;
    }
   
  // sort pairs via
  // http://stackoverflow.com/questions/10287924/c-fastest-way-to-sort-a-list-of-number-and-their-index

  std::vector<std::pair<float,unsigned long> > vp;
  vp.reserve( iEdges );
  for (unsigned long i = 0; i < iEdges; i++) 
    {
    vp.push_back(std::make_pair( graph->GetEdgePointer(i)->Weight, i));
    }

  std::sort(vp.begin(), vp.end());

  GraphType::Pointer bingraph = GraphType::New();
  for ( unsigned long i=0; i<nNodes; i++)
    {
    bingraph->CreateNewNode();
    }

  for (unsigned long i = 0; i != vp.size(); i++) 
    {
    if ( i > ( vp.size() - 1 - nEdges) ) 
      {   
      GraphType::EdgePointerType edge = bingraph->CreateNewEdge();
      edge->SourceIdentifier = graph->GetEdgePointer(vp[i].second)->SourceIdentifier;
      edge->TargetIdentifier = graph->GetEdgePointer(vp[i].second)->TargetIdentifier;
      edge->Weight = 1;
      }
    }
    
  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( bingraph );
  writer->SetColumnHeaders( reader->GetColumnHeaders() ); 
  std::cout << "Output has " << bingraph->GetTotalNumberOfEdges() << " edges" << std::endl;

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "DONE" << std::endl;
  return EXIT_SUCCESS;
}
