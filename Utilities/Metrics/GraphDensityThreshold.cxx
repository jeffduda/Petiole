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
#include "itkDensityThresholdGraphFilter.h"

int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<float,3>                 GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;
  typedef itk::Array<unsigned long>                      ArrayType;

  typedef itk::DensityThresholdGraphFilter<GraphType,GraphType>    FilterType;

  if ( argc < 4 )
    {
    std::cout << "USAGE: " << argv[0] << "input.csv density output.csv" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  std::cout << "Reading graph file: " << argv[1] << std::endl;
  
  float density = atof( argv[2] );
  
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

  FilterType::Pointer filter = FilterType::New();
  filter->SetDensity( density );
  filter->SetInput( graph );

  unsigned long nNodes = graph->GetTotalNumberOfNodes();
  unsigned long iEdges = graph->GetTotalNumberOfEdges();
  float indensity =  2.0 * iEdges / ( nNodes * (nNodes-1) );
  if ( indensity < density ) 
    {
    std::cerr << "Input density is less than threshold level, no output written" << std::endl;
    return EXIT_FAILURE;
    }

  filter->Update();
  std::cout << "filter size = " << filter->GetOutput()->GetTotalNumberOfNodes() << std::endl;

   
  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( filter->GetOutput() );
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

  std::cout << "DONE" << std::endl;
  return EXIT_SUCCESS;
}
