/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkMatlabLevel5FileReader_hxx
#define __itkMatlabLevel5FileReader_hxx

#include "itkMatlabLevel5FileReader.h"
#include "itk_zlib.h"
#include <iostream>
#include <fstream>

namespace itk
{
/**
 * Constructor
 */
template< typename TValueType >
MatlabLevel5FileReader< TValueType >
::MatlabLevel5FileReader()
{
  this->m_NumberOfVariables = 0;
  this->m_VariableData = ValueVectorHolderType::New();
  this->m_VariableDimensions = DimensionVectorHolderType::New();
  this->m_Names = StringVectorType::New();
}

/**
 * get data value from array
 */
template< typename TValueType >
typename MatlabLevel5FileReader<TValueType >::ValueType
MatlabLevel5FileReader<TValueType >
::GetValue(char * data, MatlabDataType data_type, MatlabDataType offset)
{
  ValueType value = 0;
  switch ( data_type ) 
    {
    case miINT8:
      value = static_cast<ValueType>( *reinterpret_cast<char *>( data + offset*sizeof(char)) );
      break;
    case miUINT8:
      value = static_cast<ValueType>( *reinterpret_cast<unsigned char *>( data + offset*sizeof(unsigned char)) );
      break;
    case miINT16:
      value = static_cast<ValueType>( *reinterpret_cast<int16_t *>( data + offset*sizeof(int16_t)) );
      break;
    case miUINT16:
      value = static_cast<ValueType>( *reinterpret_cast<uint16_t *>( data + offset*sizeof(uint16_t) ) );
      break;
    case miINT32:
      value = static_cast<ValueType>( *reinterpret_cast<int32_t *>( data + offset*sizeof(int32_t)) );
      break;
    case miUINT32:
      value = static_cast<ValueType>( *reinterpret_cast<uint32_t *>( data + offset*sizeof(uint32_t) ) );
      break;
    case miINT64:
      value = static_cast<ValueType>( *reinterpret_cast<int64_t *>( data + offset*sizeof(int64_t)) );
      break;
    case miUINT64:
      value = static_cast<ValueType>( *reinterpret_cast<uint64_t *>( data + offset*sizeof(uint64_t) ) );
      break;
    case miSINGLE:
      value = static_cast<ValueType>( *reinterpret_cast<float *>( data + offset*sizeof(float)) );
      break;
    case miDOUBLE:
      value = static_cast<ValueType>( *reinterpret_cast<double *>( data + offset*sizeof(double) ) );
      break;
    case miUTF8:
      value = static_cast<ValueType>( *reinterpret_cast<uint8_t *>( data + offset*sizeof(uint8_t)) );
      break;
    case miUTF16:
      value = static_cast<ValueType>( *reinterpret_cast<uint16_t *>( data + offset*sizeof(uint16_t)) );
      break;
    case miUTF32:
      value = static_cast<ValueType>( *reinterpret_cast<uint32_t *>( data + offset*sizeof(uint32_t)) );
      break;
    default:
      itkExceptionMacro("Unknown data type: " << data_type);
    }
  
  return value;
  
}

/**
 * uncompress data
 */
template< typename TValueType >
int
MatlabLevel5FileReader<TValueType >
::InflateData(char * data, MatlabDataType size, char * vdata, MatlabDataType * vSize)
{
  z_stream strm;
  strm.total_in  = strm.avail_in  = size;
  strm.total_out = strm.avail_out = size*100;
  strm.next_in   = (Bytef *) data;
  strm.next_out  = (Bytef *) vdata;
  
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
      return err;
      }
    }
  else 
    {
    inflateEnd(&strm);
    return err;
    }
  inflateEnd(&strm);

  *vSize = ret;
  return err;
}

/**
 * Read matlab level-5 file array data
 */
template< typename TValueType >
void
MatlabLevel5FileReader<TValueType >
::ParseArray(char * data, MatlabDataType itkNotUsed(size) )
{

  // Array Flags
  MatlabDataType data_type = *( reinterpret_cast<MatlabDataType *>( data ));
  MatlabDataType arrayFlagsSize = *( reinterpret_cast<MatlabDataType *>( data + sizeof(data_type) ) );
  
  /*  NOTE: class seems to be ignored. Even when it says "miDOUBLE_CLASS" the
   *  data may still be saved as unsigned char
   */
  //unsigned int arrayClass =  (unsigned int) data[sizeof(data_type) + sizeof(arrayFlagsSize)];
  //unsigned int arrayFlags =  (unsigned int) data[sizeof(data_type) + sizeof(arrayFlagsSize) + 1];    
  //std::cout << "class = " << arrayClass << std::endl;

  MatlabDataType dimensionOffset = sizeof(data_type) + sizeof(arrayFlagsSize) + arrayFlagsSize; 
  data_type = *( reinterpret_cast<MatlabDataType *>( data + dimensionOffset ) );
  MatlabDataType dimensionDataSize = *( reinterpret_cast<MatlabDataType *>( data + dimensionOffset + sizeof(data_type)  ) );
  MatlabDataType nDimensions = dimensionDataSize;

  if ( data_type > 2 ) 
    {
    nDimensions /= 2;
    }
  if ( data_type > 4 ) 
    {
    nDimensions /= 2;
    }
  if ( data_type > 6 )
    {
    nDimensions /= 2;
    }
  if ( data_type > 11 )
    {
    nDimensions /= 2;
    }

  // Get dimenions information
  DimensionVectorPointer dimensions = DimensionVectorType::New();
  char * dimensionDataOffset = data + dimensionOffset + sizeof(data_type) + sizeof(dimensionDataSize);
  for ( unsigned int i=0; i<nDimensions; i++)
    {
    ValueType d = this->GetValue( dimensionDataOffset, data_type, i );
    dimensions->InsertElement( i, static_cast<unsigned long>(d) );
    }
  this->m_VariableDimensions->InsertElement( this->GetNumberOfVariables(), dimensions );
  
  // Get variable name
  MatlabDataType nameOffset = dimensionOffset + sizeof(data_type) + sizeof(dimensionDataSize) + dimensionDataSize; 
  data_type = *( reinterpret_cast<MatlabDataType *>( data + nameOffset ) );
  MatlabDataType nameDataSize = *( reinterpret_cast<MatlabDataType *>( data + nameOffset + sizeof(data_type)  ) );
  char * name = new char [nameDataSize];
  
  for ( unsigned int i=0; i<nameDataSize; i++)
    {
    name[i] =  data[ nameOffset + sizeof(data_type) + sizeof(nameDataSize) + i];
    }

  std::string vName( name );
  this->m_Names->InsertElement( this->GetNumberOfVariables(), vName );
  
  MatlabDataType realOffset = nameOffset + sizeof(data_type) + sizeof(nameDataSize) + nameDataSize + ( 8 - (nameDataSize % 8) );
  data_type = *( reinterpret_cast<MatlabDataType *>( data + realOffset ) );
  MatlabDataType realDataSize = *( reinterpret_cast<MatlabDataType *>( data + realOffset + sizeof(data_type)  ) );
  
  char * dataStart = data + realOffset + sizeof(data_type) + sizeof(realDataSize );
  ValueVectorPointer values = ValueVectorType::New();
  
  for ( MatlabDataType i=0; i<realDataSize; i++)
    {
    ValueType value = this->GetValue( dataStart, data_type, i );
    values->InsertElement( i, value );
    }

  this->m_VariableData->InsertElement( this->GetNumberOfVariables(), values );

}

/**
 * Read matlab level-5 file
 */
template< typename TValueType >
void
MatlabLevel5FileReader<TValueType >
::ParseDataElement(char * data, MatlabDataType element_data_type, MatlabDataType size)
{

  char * vData = data;
  MatlabDataType vSize = size;
  bool compressed = false;

  if ( element_data_type == miCOMPRESSED )
    {
    vSize = size*100;
    vData = new char [ vSize ];   
    int err = this->InflateData( data, size, vData, &vSize );
    if ( err != 1 )
      {
      itkExceptionMacro("Error occured while decompressing data " << "\"");
      }
    compressed = true;
    }

  // Now work on vdata
  MatlabDataType data_type = ( reinterpret_cast<MatlabDataType *>( vData ) )[0];
  MatlabDataType num_bytes = ( reinterpret_cast<MatlabDataType *>( vData ) )[1];
  
  if ( data_type == miMATRIX )
    {
    this->ParseArray( vData+sizeof(data_type)+sizeof(num_bytes), num_bytes );
    }
  else
    {
    itkExceptionMacro("Non-array data not currently supported " << "\"");
    }

  if ( compressed )
    {
    delete [] vData;
    }
 
}

/**
 * Read matlab level-5 file
 */
template< typename TValueType >
void
MatlabLevel5FileReader<TValueType >
::ReadDataElement(void)
{
  MatlabDataType data_type, num_bytes;
  this->m_MatFile.read(reinterpret_cast<char*>(&data_type), sizeof(MatlabDataType));
  this->m_MatFile.read(reinterpret_cast<char*>(&num_bytes), sizeof(MatlabDataType));
  char * data = new char [ num_bytes ];
  this->m_MatFile.read( data, num_bytes );
  this->ParseDataElement( data, data_type, num_bytes );

  delete [] data;
}



/**
 * Read matlab level-5 file
 */
template< typename TValueType >
void
MatlabLevel5FileReader<TValueType >
::Update(void)
{
  this->m_NumberOfVariables = 0;
  this->m_VariableData->Initialize();
  this->m_VariableDimensions->Initialize();
  this->m_Names->Initialize();

  this->m_MatFile.open(this->GetFileName(),
                       std::ios::in | std::ios::binary);

  if ( this->m_MatFile.fail() )
    {
    this->m_MatFile.close();
    itkExceptionMacro("The file could not be opened for read access "
                      << std::endl << "Filename: \"" << this->GetFileName() << "\"");
    }
  
  bool swap = false;

  struct mat5hdr 
  {
    char description[116];
    char offset1[4];
    char offset2[4];
    char version[2];
    char endian[2];
  } header;

  this->m_MatFile.read( reinterpret_cast<char*>( &header ), sizeof( header ) );
  if ( std::string( header.endian ) == "IM" ) 
    {
    swap = true;
    }

  while ( !this->m_MatFile.eof() )
    {
    this->ReadDataElement();
    ++this->m_NumberOfVariables;
    }
  
  this->m_MatFile.close();

}

template< typename TValueType >
void
MatlabLevel5FileReader< TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{  Superclass::PrintSelf(os, indent);

  os << indent << "Number of Variables: "
     << m_NumberOfVariables
     << std::endl;
}
} // end namespace itk

#endif
