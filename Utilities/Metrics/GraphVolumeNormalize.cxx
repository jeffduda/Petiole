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
#include "itkGraphFileWriter.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;

  if ( argc < 3 )
    {
    std::cout << "usage: " << argv[0] << "input1.csv input2.csv [overlap_graph.csv]" << std::endl;
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
  GraphType::Pointer graph1 = reader->GetOutput();

  GraphReaderType::Pointer reader2 = GraphReaderType::New();
  reader2->SetFileName( argv[2] );
  try 
    {
    reader2->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  GraphType::Pointer graph2 = reader2->GetOutput();

  GraphType::Pointer overlapGraph;
  bool overlap = false;
  if ( argc > 3 ) 
    {
    overlap = true;
    overlapGraph = GraphType::New();
    for (unsigned long i=0; i<graph1->GetTotalNumberOfNodes(); i++)
      {
      overlapGraph->CreateNewNode();
      }
    }

  unsigned long intersection = 0;
  for (unsigned long i=0; i<graph1->GetTotalNumberOfEdges(); i++)
    {
    bool matched = false;
    unsigned long j=0;
    const GraphType::EdgePointerType edge1 = graph1->GetEdgePointer(i);
    while ( (!matched) && (j < graph2->GetTotalNumberOfEdges()) ) 
      {
      if ( (graph2->GetEdgePointer(j)->SourceIdentifier == edge1->SourceIdentifier) &&
           (graph2->GetEdgePointer(j)->TargetIdentifier == edge1->TargetIdentifier) )
        {
        matched = true;
        ++intersection;
        }
      else if ( (graph2->GetEdgePointer(j)->SourceIdentifier == edge1->TargetIdentifier) &&
           (graph2->GetEdgePointer(j)->TargetIdentifier == edge1->SourceIdentifier) )
        {
        matched = true;
        ++intersection;
        }
      if ( matched && overlap) 
        {
        GraphType::EdgePointerType newedge = overlapGraph->CreateNewEdge();
        newedge->SourceIdentifier = edge1->SourceIdentifier;
        newedge->TargetIdentifier = edge1->TargetIdentifier;
        newedge->Weight = 1;
        }

      ++j;
      }
    }
    
  std::cout << "intesection = " << intersection << std::endl;
  float dice = 2.0 * intersection / ( graph1->GetTotalNumberOfEdges() + graph2->GetTotalNumberOfEdges() );
  std::cout << dice << std::endl;

  if ( overlap && (intersection > 0) )
    {
    GraphWriterType::Pointer writer = GraphWriterType::New();
    writer->SetFileName( argv[3] );
    writer->SetInput( overlapGraph );
    writer->SetColumnHeaders( reader->GetColumnHeaders() ); 
    
    try 
      {
      writer->Update();
      }
    catch( itk::ExceptionObject & excp )
      {
      std::cerr << excp << std::endl;
      return EXIT_FAILURE;
      }
    }
  
  return EXIT_SUCCESS;
}
