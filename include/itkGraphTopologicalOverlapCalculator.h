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
#ifndef __itkGraphTopologicalOverlapCalculator_h
#define __itkGraphTopologicalOverlapCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"
#include "itkGraphNodeDegreeCentralityCalculator.h"
#include "itkVectorContainer.h"

namespace itk
{
/** \class GraphTopologicalOverlapCalculator
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
 * \wikiexample{ImageProcessing/GraphNodeStrengthCalculator,Find the minimum and maximum value (and the position of the value) in an image}
 * \wikiexample{Developer/OilPaintingImageFilter,Multi-threaded oil painting image filter}
 * \endwiki
 */
template< class TInputGraph, typename TValueType=float >
class ITK_EXPORT GraphTopologicalOverlapCalculator:public Object
{
public:
  /** Standard class typedefs. */
  typedef GraphTopologicalOverlapCalculator Self;
  typedef Object                         Superclass;
  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;
  typedef TValueType                     ValueType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GraphTopologicalOverlapCalculator, Object);

  /** Type definition for the input image. */
  typedef TInputGraph GraphType;

  /** Pointer type for the image. */
  typedef typename TInputGraph::Pointer GraphPointer;

  /** Const Pointer type for the image. */
  typedef typename TInputGraph::ConstPointer GraphConstPointer;

  typedef GraphNodeDegreeCentralityCalculator<GraphType, ValueType> DegreeCalculatorType;
  typedef typename DegreeCalculatorType::Pointer                    DegreeCalculatorPointer;

  typedef VariableSizeMatrix<ValueType>  MatrixType;

  /** Type definition for the input image region type. */
  typedef typename TInputGraph::RegionType RegionType;
  
  typedef typename RegionType::Pointer  RegionPointerType;

  /** Set the input image. */
  itkSetObjectMacro(Graph, GraphType);

  /** Compute the centrality of the nodes in the graph. */
  void Compute(void);

  /** Return the centrality for node n */
  ValueType GetTopologicalOverlap( unsigned int n, unsigned int m )
  { return this->m_Overlap(n,m); }
  
  MatrixType GetTopologicalOverlap( void ) 
  { return this->m_Overlap; }

  /** Set the region over which the values will be computed */
  //void SetRegion(const RegionType & region);

  itkSetMacro( Degree, unsigned int );
  itkGetMacro( Degree, unsigned int );

protected:
  GraphTopologicalOverlapCalculator();
  virtual ~GraphTopologicalOverlapCalculator() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  GraphTopologicalOverlapCalculator(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  GraphPointer m_Graph;

  MatrixType m_Overlap;

  unsigned int m_Degree;

  RegionPointerType GetNeighbors( unsigned long nodeId, unsigned long m );

  //RegionType m_Region;
  //bool       m_RegionSetByUser;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGraphTopologicalOverlapCalculator.hxx"
#endif

#endif /* __itkGraphTopologicalOverlapCalculator_h */
