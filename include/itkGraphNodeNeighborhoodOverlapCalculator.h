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
#ifndef __itkGraphNodeNeighborhoodOverlapCalculator_h
#define __itkGraphNodeNeighborhoodOverlapCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"

namespace itk
{
/** \class GraphNodeNeighborhoodOverlapCalculator
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
class ITK_EXPORT GraphNodeNeighborhoodOverlapCalculator:public Object
{
public:
  /** Standard class typedefs. */
  typedef GraphNodeNeighborhoodOverlapCalculator Self;
  typedef Object                         Superclass;
  typedef SmartPointer< Self >           Pointer;
  typedef SmartPointer< const Self >     ConstPointer;
  typedef TValueType                     ValueType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GraphNodeNeighborhoodOverlapCalculator, Object);

  typedef enum { OVERLAP_SYMMETRIC, OVERLAP_IN, OVERLAP_OUT } OverlapType;

  /** Type definition for the input image. */
  typedef TInputGraph GraphType;

  /** Pointer type for the image. */
  typedef typename TInputGraph::Pointer GraphPointer;

  /** Const Pointer type for the image. */
  typedef typename TInputGraph::ConstPointer GraphConstPointer;

  typedef typename GraphType::GraphTraitsType GraphTraitsType;

  typedef typename GraphType::NodeIdentifierType NodeIdentifierType;

  typedef typename GraphTraitsType::EdgeIdentifierContainerType EdgeIdentifierContainerType;

  typedef VariableSizeMatrix<ValueType>  MatrixType;

  /** Type definition for the input image region type. */
  // typedef typename TInputGraph::RegionType RegionType;

  /** Set the input image. */
  itkSetConstObjectMacro(Graph, GraphType);

  /** Compute the centrality of the nodes in the graph. */
  void Compute(void);

  /** Return the centrality for node n */
  ValueType GetNeighborhoodOverlap( unsigned int n, unsigned int m )
  { return this->m_NeighborhoodOverlap(n,m); }
  
  MatrixType GetNeighborhoodOverlap( void ) 
  { return this->m_NeighborhoodOverlap; }

  /** Set the region over which the values will be computed */
  //void SetRegion(const RegionType & region);

  void ComputeNeighborhoodOverlap( void );

  void ComputeIncomingNeighborhoodOverlap( void );

  void ComputeOutgoingNeighborhoodOverlap( void );

  void ComputeUndirectedNeighborhoodOverlap( void );

protected:
  GraphNodeNeighborhoodOverlapCalculator();
  virtual ~GraphNodeNeighborhoodOverlapCalculator() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  GraphNodeNeighborhoodOverlapCalculator(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  GraphConstPointer m_Graph;

  MatrixType m_NeighborhoodOverlap;

  OverlapType m_Overlap;

  

  //RegionType m_Region;
  //bool       m_RegionSetByUser;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGraphNodeNeighborhoodOverlapCalculator.hxx"
#endif

#endif /* __itkGraphNodeNeighborhoodOverlapCalculator_h */
