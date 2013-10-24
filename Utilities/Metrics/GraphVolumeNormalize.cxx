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
#include "itkArray.h"
#include "itkImageGraphTraits.h"
#include "itkGraph.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkGraphFileReader.h"
#include "itkGraphFileWriter.h"
#include "../../../../../Modules/IO/NIFTI/include/itkNiftiImageIO.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;

  typedef itk::Array<float>                              ArrayType;
  typedef itk::Image<int, 3>                             ImageType;
  typedef itk::ImageFileReader<ImageType>                ImageReaderType;
  typedef itk::ImageRegionIteratorWithIndex<ImageType>   ImageIteratorType;

  // Must be a better way to do this
  itk::ObjectFactoryBase::RegisterFactory(itk::NiftiImageIOFactory::New());

  if ( argc < 3 )
    {
    std::cout << "usage: " << argv[0] << "input1.csv volumes.img output.csv" << std::endl;
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
  GraphType::Pointer graph = reader->GetOutput();

  ImageReaderType::Pointer imgReader = ImageReaderType::New();
  imgReader->SetFileName( argv[2] );
  try 
    {
    imgReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
  ImageType::Pointer img = imgReader->GetOutput();

  ArrayType volumes(graph->GetTotalNumberOfNodes());
  volumes.Fill(0.0);

  ImageIteratorType it( img, img->GetLargestPossibleRegion() );
  while( !it.IsAtEnd() )
    {
    if ( it.Value() > 0 )
      {
      volumes[it.Value()-1] += 1;
      }
    ++it;
    }

  for ( unsigned long i=0; i<graph->GetTotalNumberOfEdges(); i++ )
    {
    GraphType::EdgePoitnerType edge = graph->GetEdgePointer(i);
    egde->Weight = edge->Weight / (volumes[edge->SourceIdentifier]+volumes[edge->TargetIdentifier]);
    }

  GraphWriterType::Pointer writer = GraphWriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( graph );
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
  
  return EXIT_SUCCESS;
}
