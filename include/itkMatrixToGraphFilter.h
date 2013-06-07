/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatrixToGraphFilter.h,v $
  Language:  C++
  Date:      $Date: 2008/11/11 11:51:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrixToGraphFilter_h
#define __itkMatrixToGraphFilter_h

#include "itkGraphSource.h"
#include "itkObjectFactory.h"
#include "itkDefaultImageToGraphFunctor.h"
#include "itkVtkPolyDataFileReader.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkVariableSizeMatrix.h"

namespace itk
{

/** \class MatrixToGraphFilter
 * \brief
 *
 * MatrixToGraphFilter is the base class for all process objects that output
 * Graph data and require image data as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup ImageFilters
 */
template < class TOutputGraph >
class ITK_EXPORT MatrixToGraphFilter : public GraphSource<TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef MatrixToGraphFilter               Self;
  typedef GraphSource<TOutputGraph>     Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( MatrixToGraphFilter, GraphSource );

  /** Create a valid output. */
  //DataObject::Pointer  MakeOutput( unsigned int idx );
  using Superclass::MakeOutput;
  virtual ProcessObject::DataObjectPointer MakeOutput(ProcessObject::DataObjectPointerArraySizeType idx);

  /** Some Graph related typedefs. */
  typedef TOutputGraph                              GraphType;
  typedef typename GraphType::GraphTraitsType       GraphTraitsType;
  typedef typename GraphType::Pointer               GraphPointer;
  typedef typename GraphType::NodeType              NodeType;
  typedef typename GraphType::NodePointerType       NodePointerType;
  typedef typename GraphType::NodeIdentifierType    NodeIdentifierType;
  typedef typename GraphTraitsType::NodeWeightType  NodeWeightType;
  typedef typename GraphType::EdgeType              EdgeType;
  typedef typename GraphType::EdgePointerType       EdgePointerType;
  typedef typename GraphType::EdgeIdentifierType    EdgeIdentifierType;
  typedef typename GraphTraitsType::EdgeWeightType  EdgeWeightType;

  typedef VariableSizeMatrix<EdgeWeightType>   MatrixType;

  itkSetMacro( IsDirected, bool );
  itkGetMacro( IsDirected, bool );

  itkSetMacro( Matrix, MatrixType );
  itkGetMacro( Matrix, MatrixType );

  /** Get the output Graph of this process object.  */
  GraphType * GetOutput( void );

  /** Prepare the output */
  void GenerateOutputInformation( void );

protected:
  MatrixToGraphFilter();
  ~MatrixToGraphFilter();
  void PrintSelf( std::ostream& os, Indent indent ) const;

  void GenerateData();

  std::string m_FileName;

  bool m_IsDirected;

private:
  MatrixToGraphFilter( const MatrixToGraphFilter& ); //purposely not implemented
  void operator=( const MatrixToGraphFilter& ); //purposely not
                                            //implemented
  MatrixType m_Matrix;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMatrixToGraphFilter.hxx"
#endif

#endif
