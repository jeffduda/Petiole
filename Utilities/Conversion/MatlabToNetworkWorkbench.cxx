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
#include "itkMatlabLevel5FileReader.h"
#include "itkMatrixToGraphFilter.h"
#include "itkDefaultGraphTraits.h"
#include "itkGraph.h"
#include "itkNWBDataFileWriter.h"

int main( int argc, char * argv [] )
{
  /*
  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::MatlabLevel5FileReader<float>    MatlabReaderType;
  typedef itk::DefaultGraphTraits<float>        GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>           GraphType;
  typedef itk::MatrixToGraphFilter<GraphType>   GraphSourceType;
  typedef GraphSourceType::MatrixType           MatrixType;
  typedef itk::NWBDataFileWriter<GraphType>     WriterType;

  if ( argc < 2 )
    {
    std::cout << "No input provided" << std::endl;
    return EXIT_FAILURE;
    }

  if ( argc < 3 )
    {
    std::cout << "No variable offset provided" << std::endl;
    return EXIT_FAILURE;
    }

  if ( argc < 4 )
    {
    std::cout << "No output provided" << std::endl;
    return EXIT_FAILURE;
    }

  MatlabReaderType::Pointer reader = MatlabReaderType::New();
  reader->SetFileName( argv[1] );
  std::cout << "Reading file: " << argv[1] << std::endl;

  try 
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  
  unsigned int nVar = atoi( argv[2] );
  std::cout << reader->GetVariableName(nVar) << " has dimensions: ";
  for ( unsigned int j=0; j<reader->GetVariableDimensions( nVar )->Size(); j++)
    {
    std::cout << reader->GetVariableDimensions( nVar )->GetElement(j);
    if ( j < (reader->GetVariableDimensions( nVar )->Size()-1) )
      {
      std::cout << "x";
      }
    }
  std::cout << " ( " << reader->GetVariableData(nVar)->Size() << " elements )" << std::endl;

  MatrixType matrix( reader->GetVariableDimensions( nVar )->GetElement(0), reader->GetVariableDimensions( nVar )->GetElement(1) );
  for ( unsigned int i=0; i<reader->GetVariableData(nVar)->Size(); i++ )
    {
    int row = i / reader->GetVariableDimensions( nVar )->GetElement(1);
    int col = i % reader->GetVariableDimensions( nVar )->GetElement(1);
    matrix(row,col) = reader->GetVariableData(nVar)->GetElement(i);
    }
  
  GraphSourceType::Pointer filter = GraphSourceType::New();
  filter->SetMatrix( matrix );
  filter->Update();

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( filter->GetOutput() );
  writer->Update();
  */

  return EXIT_SUCCESS;
}
