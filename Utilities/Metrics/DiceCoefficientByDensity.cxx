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
#include "itkDensityThresholdGraphFilter.h"


int main( int argc, char * argv [] )
{

  typedef itk::ImageGraphTraits<int,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                    GraphType;
  typedef itk::GraphFileReader<GraphType>                GraphReaderType;
  typedef itk::GraphFileWriter<GraphType>                GraphWriterType;

  typedef itk::GraphIntersectionGraphFilter<GraphType,GraphType> FilterType;
  typedef itk::DensityThresholdGraphFilter<GraphType,GraphType>  ThresholdType;

  if ( argc < 6 )
    {
    std::cout << "usage: " << argv[0] << "input1.csv input2.csv density_start density_end density_increment" << std::endl;
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
  
  float d1 = atof( argv[3] );
  float d2 = atof( argv[4] );
  float di = atof( argv[5] );

  reader1->GetOutput()->IsUndirectedOn();
  reader2->GetOutput()->IsUndirectedOn();
  
  if ( d2 < d1 ) 
    {
    std::cout << "density_end must be larger than density_start" << std::endl;
    return EXIT_FAILURE;
    }

  for ( float d=d1; d<=d2; d+=di)
    {

    ThresholdType::Pointer thresh1 = ThresholdType::New();
    thresh1->SetInput( reader1->GetOutput() );
    thresh1->SetDensity( d );

    ThresholdType::Pointer thresh2 = ThresholdType::New();
    thresh2->SetInput( reader2->GetOutput() );
    thresh2->SetDensity( d );    

    FilterType::Pointer intersection = FilterType::New();
    intersection->SetInput(0,thresh1->GetOutput());
    intersection->SetInput(1,thresh2->GetOutput());
    intersection->Update();
    
    float dice = 2.0 * intersection->GetOutput()->GetTotalNumberOfEdges() / 
      ( thresh1->GetOutput()->GetTotalNumberOfEdges() + thresh2->GetOutput()->GetTotalNumberOfEdges() );
    std::cout << dice;
    if ( !( (d+di > d2) ) )
      {
      std::cout << ",";
      }
    }
  std::cout << std::endl;  
  return EXIT_SUCCESS;
}
