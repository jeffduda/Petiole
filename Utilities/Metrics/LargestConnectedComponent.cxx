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
#include "itkLargestConnectedComponentGraphFilter.h"


int main( int argc, char * argv [] )
{
  typedef itk::ImageGraphTraits<float,3>                           ImageGraphTraits;
  typedef itk::DijkstrasGraphTraits<float,3>                       GraphTraitsType;
  typedef itk::Graph<ImageGraphTraits>                             GraphType;
  typedef itk::GraphFileReader<GraphType>                          GraphReaderType;
  typedef itk::Graph<GraphTraitsType>                              SearchGraphType;
  typedef itk::GraphFileWriter<GraphType>                          GraphWriterType;

  typedef itk::LargestConnectedComponentGraphFilter<GraphType,GraphType> FilterType;

  if ( argc < 3 )
    {
    std::cout << "usage: " << argv[0] << "input.csv output.csv" << std::endl;
    return EXIT_FAILURE;
    }

  GraphReaderType::Pointer reader = GraphReaderType::New();
  reader->SetFileName( argv[1] );
  //std::cout << "Reading file: " << argv[1] << std::endl;


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

  FilterType::Pointer connected = FilterType::New();
  connected->SetInput( graph );
  connected->Update();
  
/*
  GraphReaderType::NamesType outNames;

  FilterType::MapType map = connected->GetMap();
  for ( unsigned long i=0; i<map.Size(); i++)
    {
    if ( map[i] > 0 ) 
      {

      }
    }
*/

  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( connected->GetOutput() );

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
