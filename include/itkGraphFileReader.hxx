/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphFileReader.hxx,v $
  Language:  C++
  Date:      $Date: 2008/11/11 11:51:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGraphFileReader_hxx
#define __itkGraphFileReader_hxx

#include "vnl/vnl_matlab_read.h"
#include "itkGraphFileReader.h"
#include "itkByteSwapper.h"
#include "itk_zlib.h"

namespace itk
{

/**
 *
 */
template <class TGraph, class TImage>
GraphFileReader<TGraph, TImage>
::GraphFileReader()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 0 );

  GraphPointer output
    = dynamic_cast<GraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_IsDirected = false;
}

/**
 *
 */
template <class TGraph, class TImage>
GraphFileReader<TGraph,TImage>
::~GraphFileReader()
{
}


/**
 *   Make Ouput
 */
template <class TGraph, class TImage>
DataObject::Pointer
GraphFileReader<TGraph,TImage>
::MakeOutput( ProcessObject::DataObjectPointerArraySizeType )
{
  GraphPointer  outputGraph = GraphType::New();
  return dynamic_cast< DataObject *>( outputGraph.GetPointer() );
}

template <class TGraph, class TImage>
typename GraphFileReader<TGraph, TImage>::GraphType *
GraphFileReader<TGraph, TImage>
::GetOutput( void )
{
  return dynamic_cast<GraphType*>
    ( this->ProcessObject::GetOutput( 0 ) );
}


/**
 *
 */
template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::GenerateData()
{

  if( this->m_FileName == "" )
    {
    itkExceptionMacro( "No FileName" );
    return;
    }

  //
  // Read output file
  //
  std::ifstream inputFile( m_FileName.c_str() );

  if( !inputFile.is_open() )
    {
    itkExceptionMacro( "Unable to open file\n"
        "inputFilename= " << m_FileName );
    return;
    }
  else
    {
    inputFile.close();
    }

  /**
   * Get filename extension
   */
  std::string::size_type pos = this->m_FileName.rfind( "." );
  std::string extension( this->m_FileName, pos+1, this->m_FileName.length()-1 );

  // Fixme - add .mat and images ( csv too ? ) - generic matrix reader ?
  if( extension == "vtk" )
    {
    this->ReadVtkPolyData();
    }
  if ( extension == "csv" )
    {
    this->ReadCSVData();
    }
  else if ( extension == "mat" )
    {
    std::cout << "Attempt to read matlab file" << std::endl;
    this->ReadMatlabMatrixData( );
    }
  else
    {
    itkExceptionMacro( "Unknown extension: " << extension );
    }
}

template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::ReadVtkPolyData()
{

  GraphPointer output = this->GetOutput();


  typename VtkReaderType::Pointer reader = VtkReaderType::New();
  reader->SetFileName( this->m_FileName );
  reader->Update();

  for (unsigned int i=0; i<reader->GetOutput()->GetNumberOfPoints(); i++)
    {
    NodePointerType node = output->CreateNewNode();
    for (unsigned int j=0; j<ImageType::ImageDimension; j++)
      {
      node->ImageIndex[j] = reader->GetOutput()->GetPoint( i )[j];
      }

    //if (nodeWeights != NULL)
    //  {
    //  node->Weight = nodeWeights->GetComponent(i,0);
    //  }

    }

  typename VtkReaderType::LineSetType::Pointer lines = reader->GetLines();
  for (unsigned int j=0; j<lines->Size(); j++)
    {
    typename VtkReaderType::LineType line = lines->GetElement(j);
    if (line.Size() == 2)
      {
      EdgePointerType edge = output->CreateNewEdge();
      edge->SourceIdentifier = line[0];
      edge->TargetIdentifier = line[1];
      }

    //if (edgeWeights != NULL)
    //  {
    //  edge->Weight = edgeWeights->GetComponent(j,0);
    //  }

    }

}

template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::ReadCSVData()
{

  GraphPointer output = this->GetOutput();

  typename CSVReaderType::Pointer reader = CSVReaderType::New();
  reader->SetFileName( this->m_FileName );
  reader->HasRowHeadersOff();
  reader->HasColumnHeadersOn();

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    itkExceptionMacro( "Failed to read input as csv: " << this->m_FileName );
    }

  typename CSVReaderType::Array2DDataObjectType * array =
    reader->GetModifiableArray2DDataObject();

  this->m_ColumnHeaders = array->GetColumnHeaders();

  unsigned int nNodes = array->GetMatrix().cols();


  if ( nNodes != array->GetMatrix().rows() )
    {
    std::cout << "Rows = " << array->GetMatrix().rows() << " Cols = " << array->GetMatrix().cols() << std::endl;
    itkExceptionMacro( "Failed to read csv file as square matrix: " << this->m_FileName );
    }

  for (unsigned int i=0; i<nNodes; i++)
    {
    NodePointerType node = output->CreateNewNode();

    for (unsigned int j=0; j<ImageType::ImageDimension; j++)
      {
      node->ImageIndex[j] = i;
      }
    }

  for ( unsigned int i=0; i<nNodes; i++)
    {
    unsigned int startj = 0;
    if ( ! this->m_IsDirected )
      {
      startj = i+1;
      }

    for ( unsigned int j=startj; j < nNodes; j++)
      {
      float value = array->GetMatrix()(j,i);

      if ( value != 0.0 )
        {
        EdgePointerType edge = output->CreateNewEdge();
        edge->SourceIdentifier = i;
        edge->TargetIdentifier = j;
        edge->Weight = value;
        }
      }
    }

}


template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::ReadImageAsMatrixData()
{

  GraphPointer output = this->GetOutput();

  typename ImageMatrixReaderType::Pointer reader = ImageMatrixReaderType::New();
  reader->SetFileName( this->m_FileName );

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    itkExceptionMacro( "Failed to read input as image: " << this->m_FileName );
    }

  unsigned int nNodes = reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];

  if ( nNodes != reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1] )
    {
    itkExceptionMacro( "Failed to read image as square matrix: " << this->m_FileName );
    }

  for (unsigned int i=0; i<nNodes; i++)
    {
    NodePointerType node = output->CreateNewNode();

    for (unsigned int j=0; j<ImageType::ImageDimension; j++)
      {
      node->ImageIndex[j] = i;
      }
    }

  for ( unsigned int i=0; i<nNodes; i++)
    {
    unsigned int startj = 0;
    if ( ! this->m_IsDirected )
      {
      startj = i+1;
      }

    for ( unsigned int j=startj; j < nNodes; j++)
      {
      typename ImageMatrixType::IndexType idx;
      idx[0] = j;
      idx[1] = i;
      float value =  reader->GetOutput()->GetPixel( idx );
      if ( value != 0.0 )
        {
        EdgePointerType edge = output->CreateNewEdge();
        edge->SourceIdentifier = i;
        edge->TargetIdentifier = j;
        edge->Weight = value;
        }
      }
    }

}

template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::ReadMatlabMatrixData(void)
{

  GraphPointer output = this->GetOutput();
  std::ifstream matfile(this->GetFileName(),
                        std::ios::in | std::ios::binary);

  if ( matfile.fail() )
    {
    matfile.close();
    itkExceptionMacro("The file could not be opened for read access "
                      << std::endl << "Filename: \"" << this->GetFileName() << "\"");
    }

  char * description = new char [116];
  char * offset1 = new char [4];
  char * offset2 = new char [4];
  char * version = new char [2];
  char * endian = new char [2];

  bool swap = false;

  matfile.read( description, 116 );
  matfile.read( offset1, 4 );
  matfile.read( offset2, 4 );
  matfile.read( version, 2 );
  matfile.read( endian, 2 );

  if ( std::string( endian ) == "IM" )
    {
    swap = true;
    }

  delete [] description;
  delete [] offset1;
  delete [] offset2;
  delete [] version;
  delete [] endian;


  while ( !matfile.eof() )
    {
    uint32_t data_type, num_bytes;
    matfile.read(reinterpret_cast<char*>(&data_type), sizeof(uint32_t));
    matfile.read(reinterpret_cast<char*>(&num_bytes), sizeof(uint32_t));

    //uint16_t * smallElementTest = reinterpret_cast<uint16_t *>( &data_type );
    //std::cout << "is small element = " << *smallElementTest << std::endl;
    std::cout << std::endl << data_type << "," << num_bytes << std::endl;

    char * data = new char [ num_bytes ];
    matfile.read( data, num_bytes );

    char * dest = new char [ 10000 ];

    z_stream strm;
    strm.total_in  = strm.avail_in  = num_bytes;
    strm.total_out = strm.avail_out = 10000;
    strm.next_in   = (Bytef *) data;
    strm.next_out  = (Bytef *) dest;

    strm.zalloc = Z_NULL;
    strm.zfree  = Z_NULL;
    strm.opaque = Z_NULL;

    int err = -1;
    int ret = -1;

    err = inflateInit2(&strm, (15 + 32)); //15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
    if (err == Z_OK)
      {
      err = inflate(&strm, Z_FINISH);
      if (err == Z_STREAM_END)
        {
        ret = strm.total_out;
        }
      else
        {
        inflateEnd(&strm);
        //return err;
        }
      }
    else
      {
      inflateEnd(&strm);
      //return err;
      }

    inflateEnd(&strm);

    data_type = ( reinterpret_cast<uint32_t *>( dest ) )[0];
    num_bytes = ( reinterpret_cast<uint32_t *>( dest ) )[1];
    std::cout << "uncompressed data type = " << data_type << "," << num_bytes << std::endl;

    // Array Flags
    uint32_t arrayOffset = sizeof(data_type) + sizeof(num_bytes);
    data_type = *( reinterpret_cast<uint32_t *>( dest + arrayOffset ) );
    uint32_t arrayFlagsSize = *( reinterpret_cast<uint32_t *>( dest + arrayOffset + sizeof(data_type) ) );
    std::cout << "Array flags type,size = " << data_type << "," << arrayFlagsSize << std::endl;
    unsigned int arrayClass =  (unsigned int) dest[arrayOffset + sizeof(data_type) + sizeof(arrayFlagsSize)];
    unsigned int arrayFlags =  (unsigned int) dest[arrayOffset + sizeof(data_type) + sizeof(arrayFlagsSize) + 1];
    std::cout << "  class = " << arrayClass << std::endl;
    std::cout << "  flags = " << arrayFlags << std::endl;

    uint32_t dimensionOffset = arrayOffset + sizeof(data_type) + sizeof(arrayFlagsSize) + arrayFlagsSize;
    data_type = *( reinterpret_cast<uint32_t *>( dest + dimensionOffset ) );
    uint32_t dimensionDataSize = *( reinterpret_cast<uint32_t *>( dest + dimensionOffset + sizeof(data_type)  ) );
    std::cout << "Dimension type,size = " << data_type << "," << dimensionDataSize << std::endl;
    uint32_t nDimensions = dimensionDataSize;

    if ( data_type > 4 )
      {
      nDimensions /= 2;
      }
    if ( data_type < 6 )
      {
      nDimensions /= 2;
      }
    else if ( data_type < 11 )
      {
      nDimensions /= 2;
      }
    std::cout << "  Number of dimensions = " << nDimensions << std::endl;
    for ( unsigned int i=0; i<nDimensions; i++)
      {
      std::cout << "  " << reinterpret_cast<uint32_t *>( dest + dimensionOffset + sizeof(data_type) + sizeof(dimensionDataSize))[i];
      }
    std::cout << std::endl;

    uint32_t nameOffset = dimensionOffset + sizeof(data_type) + sizeof(dimensionDataSize) + dimensionDataSize;
    data_type = *( reinterpret_cast<uint32_t *>( dest + nameOffset ) );
    uint32_t nameDataSize = *( reinterpret_cast<uint32_t *>( dest + nameOffset + sizeof(data_type)  ) );
    std::cout << "Name type,size = " << data_type << "," << nameDataSize << std::endl;
    char * name = new char [nameDataSize];

    for ( unsigned int i=0; i<nameDataSize; i++)
      {
      name[i] =  dest[ nameOffset + sizeof(data_type) + sizeof(nameDataSize) + i];
      }
    std::cout << "  Name = " << name << std::endl;

    uint32_t realOffset = nameOffset + sizeof(data_type) + sizeof(nameDataSize) + nameDataSize + ( 8 - (nameDataSize % 8) );  ;
    data_type = *( reinterpret_cast<uint32_t *>( dest + realOffset ) );
    uint32_t realDataSize = *( reinterpret_cast<uint32_t *>( dest + realOffset + sizeof(data_type)  ) );
    std::cout << "Real_data type,size = " << data_type << "," << realDataSize << std::endl;

    delete [] name;
    delete [] dest;
    delete [] data;

    }

  matfile.close();


  std::cout << "End of ReadMatlabMatrixData()" << std::endl;
}

/**
 *
 */
template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template <class TGraph, class TImage>
void
GraphFileReader<TGraph,TImage>
::GenerateOutputInformation()
{
}


} // end namespace itk

#endif
