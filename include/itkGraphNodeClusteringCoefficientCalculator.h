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
#ifndef __itkGraphNodeClusteringCoefficientCalculator_h
#define __itkGraphNodeClusteringCoefficientCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVectorContainer.h"

namespace itk
{
/** \class GraphNodeClusteringCoefficientCalculator
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
 * \wikiexample{ImageProcessing/GraphNodeClusteringCoefficientCalculator,Find the minimum and maximum value (and the position of the value) in an image}
 * \wikiexample{Developer/OilPaintingImageFilter,Multi-threaded oil painting image filter}
 * \endwiki
 */
template< class TInputGraph, typename TValueType=float >
class ITK_EXPORT GraphNodeClusteringCoefficientCalculator:public Object
{
public:
  /** Standard class typedefs. */
  typedef GraphNodeClusteringCoefficientCalculator Self;
  typedef Object                        Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef TValueType                                 ValueType;
  typedef VectorContainer<unsigned long, ValueType>  ValueVectorType;
  typedef typename ValueVectorType::Pointer          ValueVectorPointer;

  typedef enum { DEGREE_SYMMETRIC, DEGREE_IN, DEGREE_OUT } DegreeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GraphNodeClusteringCoefficientCalculator, Object);

  /** Type definition for the input image. */
  typedef TInputGraph GraphType;

  /** Pointer type for the image. */
  typedef typename TInputGraph::Pointer GraphPointer;

  /** Const Pointer type for the image. */
  typedef typename TInputGraph::ConstPointer GraphConstPointer;

  typedef typename GraphType::GraphTraitsType::EdgeIdentifierContainerType EdgeIdentifierContainerType;

  typedef typename GraphType::GraphTraitsType::NodeIdentifierContainerType NodeIdentifierContainerType;

  typedef typename GraphType::GraphTraitsType::EdgePointerType EdgePointerType;

  /** Type definition for the input image region type. */
  // typedef typename TInputGraph::RegionType RegionType;

  /** Set the input image. */
  itkSetConstObjectMacro(Graph, GraphType);

  /** Compute the centrality of the nodes in the graph. */
  void Compute(void);

  void ComputeWeightedClusteringCoefficient(void);

  void ComputeClusteringCoefficient(void);

  /** Return the centrality for node n */
  ValueType GetClusteringCoefficient( unsigned int n )
  { return this->m_ClusteringCoefficient->GetElement( n ); }

  ValueVectorPointer GetClusteringCoefficient( void ) 
  { return this->m_ClusteringCoefficient; }

  /** Set the region over which the values will be computed */
  //void SetRegion(const RegionType & region);

protected:
  GraphNodeClusteringCoefficientCalculator();
  virtual ~GraphNodeClusteringCoefficientCalculator() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  GraphNodeClusteringCoefficientCalculator(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  GraphConstPointer m_Graph;

  ValueVectorPointer m_ClusteringCoefficient;

  bool m_IsWeighted;

  //RegionType m_Region;
  //bool       m_RegionSetByUser;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGraphNodeClusteringCoefficientCalculator.hxx"
#endif

#endif /* __itkGraphNodeClusteringCoefficientCalculator_h */
