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
#ifndef __itkGraphNodeStrengthCalculator_h
#define __itkGraphNodeStrengthCalculator_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVectorContainer.h"

namespace itk
{
/** \class GraphNodeStrengthCalculator
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
class ITK_EXPORT GraphNodeStrengthCalculator:public Object
{
public:
  /** Standard class typedefs. */
  typedef GraphNodeStrengthCalculator Self;
  typedef Object                        Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef TValueType                                 ValueType;
  typedef VectorContainer<unsigned long, ValueType>  ValueVectorType;
  typedef typename ValueVectorType::Pointer          ValueVectorPointer;

  typedef enum { STRENGTH_SYMMETRIC, STRENGTH_IN, STRENGTH_OUT } StrengthType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GraphNodeStrengthCalculator, Object);

  /** Type definition for the input image. */
  typedef TInputGraph GraphType;

  /** Pointer type for the image. */
  typedef typename TInputGraph::Pointer GraphPointer;

  /** Const Pointer type for the image. */
  typedef typename TInputGraph::ConstPointer GraphConstPointer;

  /** Type definition for the input image region type. */
  // typedef typename TInputGraph::RegionType RegionType;

  /** Set the input image. */
  itkSetConstObjectMacro(Graph, GraphType);

  /** Compute the centrality of the nodes in the graph. */
  void Compute(void);

  /** Compute the inward degree of the nodes in the graph. */
  void ComputeInStrength(void);

  /** Compute the outward degree of the nodes in the graph. */
  void ComputeOutStrength(void);

  /** Compute the in + out degree of the nodes in the graph. */
  void ComputeUndirectedStrength(void);

  /** Return the centrality for node n */
  ValueType GetStrength( unsigned int n )
  { return this->m_Strength->GetElement( n ); }

  /** Set the region over which the values will be computed */
  //void SetRegion(const RegionType & region);

protected:
  GraphNodeStrengthCalculator();
  virtual ~GraphNodeStrengthCalculator() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  GraphNodeStrengthCalculator(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  GraphConstPointer m_Graph;

  ValueVectorPointer m_Strength;

  StrengthType m_StrengthType;

  //RegionType m_Region;
  //bool       m_RegionSetByUser;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGraphNodeStrengthCalculator.hxx"
#endif

#endif /* __itkGraphNodeStrengthCalculator_h */
