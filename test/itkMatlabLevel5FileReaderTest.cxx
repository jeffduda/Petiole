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

int itkMatlabLevel5FileReaderTest( int argc, char * argv [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::MatlabLevel5FileReader<float>       MatlabReaderType;

  if ( argc < 2 )
    {
    std::cout << "No input provided" << std::endl;
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
    

  std::cout << reader->GetFileName() << " has " << reader->GetNumberOfVariables() << " varibles" << std::endl;
  for ( unsigned int i=0; i<reader->GetNumberOfVariables(); i++)
    {
    std::cout << reader->GetVariableName(i) << " has dimensions: ";
    for ( unsigned int j=0; j<reader->GetVariableDimensions( i )->Size(); j++)
      {
      std::cout << reader->GetVariableDimensions( i )->GetElement(j);
      if ( j < (reader->GetVariableDimensions( i )->Size()-1) )
        {
        std::cout << "x";
        }
      }
    std::cout << " ( " << reader->GetVariableData(i)->Size() << " elements )" << std::endl;
    }

  unsigned int idx = 0;
  for ( unsigned int i=0; i<reader->GetVariableDimensions( 1 )->GetElement( 0 ); i++ )
    {
    for ( unsigned int j=0; j<reader->GetVariableDimensions( 1 )->GetElement( 1 ); j++)
      {
      std::cout << reader->GetVariableData(1)->GetElement( idx ) << " ";
      ++idx;
      }
    std::cout << std::endl;
    }
    

  return EXIT_SUCCESS;
}
