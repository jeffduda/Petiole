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
#include "itkGraphIntersectionGraphFilter.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;

  typedef itk::GraphIntersectionGraphFilter<GraphType,GraphType> FilterType;

  if ( argc < 3 )
    {
    std::cout << "usage: " << argv[0] << "input1.csv input2.csv [overlap_graph.csv]" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader1 = GraphReaderType::New();
  reader1->SetFileName( argv[1] );
  try 
    {
    reader1->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  GraphType::Pointer graph1 = reader1->GetOutput();

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

  FilterType::Pointer intersection = FilterType::New();
  intersection->SetInput(0,reader1->GetOutput());
  intersection->SetInput(1,reader2->GetOutput());
  intersection->Update();

  float dice = 2.0 * intersection->GetOutput()->GetTotalNumberOfEdges() / 
    ( graph1->GetTotalNumberOfEdges() + graph2->GetTotalNumberOfEdges() );
  std::cout << dice << std::endl;
  
  if ( (argc > 3) && (dice > 0) )
    {
    GraphWriterType::Pointer writer = GraphWriterType::New();
    writer->SetFileName( argv[3] );
    writer->SetInput( intersection->GetOutput() );
    writer->SetColumnHeaders( reader1->GetColumnHeaders() ); 
    
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
