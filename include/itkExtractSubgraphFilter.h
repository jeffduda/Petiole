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
#ifndef __itkExtractSubgraphFilter_h
#define __itkExtractSubgraphFilter_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"
#include "itkGraphNodeDegreeCentralityCalculator.h"
#include "itkVectorContainer.h"
#include "itkGraphToGraphFilter.h"

namespace itk
{
/** \class ExtractSubgraphFilter
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
template< class TInputGraph, typename TOutputGraph >
class ITK_EXPORT ExtractSubgraphFilter:public GraphToGraphFilter<TInputGraph,TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef ExtractSubgraphFilter                             Self;
  typedef GraphToGraphFilter<TInputGraph,TOutputGraph>    Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractSubgraphFilter, Object);

  typedef float                                     ValueType;
  typedef VectorContainer<unsigned long, ValueType> ValueVectorType;

  typedef TInputGraph InputGraphType;

  /** Pointer type for the image. */
  typedef typename TInputGraph::Pointer InputGraphPointer;

  typedef typename InputGraphType::NodeType InputNodeType;

  typedef typename InputGraphType::RegionType::Pointer RegionPointer;

  /** Const Pointer type for the image. */
  typedef typename TInputGraph::ConstPointer InputGraphConstPointer;

  /** Type definition for the input image. */
  typedef TOutputGraph OutputGraphType;

  /** Pointer type for the image. */
  typedef typename TOutputGraph::Pointer OutputGraphPointer;

  typedef typename OutputGraphType::NodeType NodeType;

  typedef typename OutputGraphType::EdgeType EdgeType;

  typedef typename OutputGraphType::NodePointerType NodePointerType;

  typedef typename OutputGraphType::EdgePointerType EdgePointerType;

  typedef GraphNodeDegreeCentralityCalculator<InputGraphType, ValueType> DegreeCalculatorType;
  typedef typename DegreeCalculatorType::Pointer                         DegreeCalculatorPointer;

  /** Prepare the output */
  void GenerateOutputInformation( void );

  /** Set/Get the output image region.
   *  If any of the ExtractionRegion.Size = 0 for any particular dimension dim,
   *  we have to collapse dimension dim.  This means the output image will have
   *  'c' dimensions less than the input image, where c = number of
   *  ExtractionRegion.Size = 0. */
  void SetExtractionRegion(RegionPointer extractRegion);
  itkGetConstMacro(ExtractionRegion, RegionPointer);

protected:
  ExtractSubgraphFilter();
  virtual ~ExtractSubgraphFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  ExtractSubgraphFilter(const Self &);           //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  void GenerateData();

  RegionPointer    m_ExtractionRegion;
  bool             m_RegionSetByUser;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkExtractSubgraphFilter.hxx"
#endif

#endif /* __itkExtractSubgraphFilter_h */
