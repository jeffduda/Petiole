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
#ifndef __itkMatlabLevel5FileReader_h
#define __itkMatlabLevel5FileReader_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVectorContainer.h"
#include <fstream>

namespace itk
{
/** \class MatlabLevel5FileReader
 *  \brief Computes the minimum and the maximum intensity values of
 *         an image.
 *
 * This calculator computes the minimum and the maximum intensity values of
 * an image.  It is templated over input image type.  If only Maximum or
 * Minimum value is needed, just call ComputeMaximum() (ComputeMinimum())
 * otherwise Compute() will compute both.
 *
 * \ingroup Operators
 * \ingroup ITKCommon
 *
 * \wiki
 * \wikiexample{ImageProcessing/MatlabLevel5FileReader,Find the minimum and maximum value (and the position of the value) in an image}
 * \wikiexample{Developer/OilPaintingImageFilter,Multi-threaded oil painting image filter}
 * \endwiki
 */
template< typename TValueType=float >
class ITK_EXPORT MatlabLevel5FileReader:public Object
{
public:
  /** Standard class typedefs. */
  typedef MatlabLevel5FileReader Self;
  typedef Object                        Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef TValueType                                 ValueType;

  typedef VectorContainer<unsigned long, ValueType>      ValueVectorType;
  typedef typename ValueVectorType::Pointer              ValueVectorPointer;

  typedef VectorContainer<unsigned long, ValueVectorPointer> ValueVectorHolderType;
  typedef typename ValueVectorHolderType::Pointer            ValueVectorHolderPointer;

  typedef VectorContainer<unsigned long, unsigned long>  DimensionVectorType;
  typedef typename DimensionVectorType::Pointer          DimensionVectorPointer;

  typedef VectorContainer<unsigned long, DimensionVectorPointer >   DimensionVectorHolderType;
  typedef typename DimensionVectorHolderType::Pointer               DimensionVectorHolderPointer;

  typedef VectorContainer<unsigned long, std::string> StringVectorType;
  typedef StringVectorType::Pointer                   StringVectorPointer;

  typedef uint32_t  MatlabDataType;

  enum {
    miUNDEFINED,
    miINT8,
    miUINT8,
    miINT16,
    miUINT16,
    miINT32,
    miUINT32,
    miSINGLE,
    miRESERVED1,
    miDOUBLE,
    miRESERVED2,
    miRESERVED3,
    miINT64,
    miUINT64,
    miMATRIX,
    miCOMPRESSED,
    miUTF8,
    miUTF16,
    miUTF32 }
  MatlabDataTypeFlag;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MatlabLevel5FileReader, Object);

  void Update( void );

  itkGetMacro( NumberOfVariables, unsigned int );

  itkGetStringMacro( FileName );
  itkSetStringMacro( FileName );

  ValueVectorPointer GetVariableData( unsigned long i )
  { return this->m_VariableData->GetElement( i ); }

  DimensionVectorPointer GetVariableDimensions( unsigned long i )
  { return this->m_VariableDimensions->GetElement( i ); }

  std::string GetVariableName( unsigned long i )
  { return this->m_Names->GetElement(i); }
    


protected:
  MatlabLevel5FileReader();
  virtual ~MatlabLevel5FileReader() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  MatlabLevel5FileReader(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented
  
  unsigned int m_NumberOfVariables;

  std::string m_FileName;

  ValueVectorHolderPointer m_VariableData;

  DimensionVectorHolderPointer m_VariableDimensions;

  StringVectorPointer m_Names;

  std::ifstream m_MatFile;

  void ReadDataElement( void );

  void ParseDataElement( char * data , MatlabDataType data_type, MatlabDataType size );

  void ParseArray( char * data , MatlabDataType size );

  int InflateData(char * data, MatlabDataType size, char * vdata, MatlabDataType * vSize);

  ValueType GetValue( char * ptr, MatlabDataType data_type, MatlabDataType offset );

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMatlabLevel5FileReader.hxx"
#endif

#endif /* __itkMatlabLevel5FileReader_h */
