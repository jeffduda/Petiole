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
#ifndef __itkExtractRichClubGraphFilter_h
#define __itkExtractRichClubGraphFilter_h

#include "itkObject.h"
#include "itkObjectFactory.h"
#include "itkVariableSizeMatrix.h"
#include "itkGraphNodeDegreeCentralityCalculator.h"
#include "itkVectorContainer.h"
#include "itkGraphToGraphFilter.h"
#include "itkExtractSubgraphFilter.h"

namespace itk
{
/** \class ExtractRichClubGraphFilter
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
class ITK_EXPORT ExtractRichClubGraphFilter:public GraphToGraphFilter<TInputGraph,TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef ExtractRichClubGraphFilter                             Self;
  typedef GraphToGraphFilter<TInputGraph,TOutputGraph>    Superclass;
  typedef SmartPointer< Self >                            Pointer;
  typedef SmartPointer< const Self >                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractRichClubGraphFilter, Object);

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

  typedef ExtractSubgraphFilter<InputGraphType, OutputGraphType> ExtractFilterType;
  
  typedef typename ExtractFilterType::Pointer                    ExtractFilterPointer;

  /** Prepare the output */
  void GenerateOutputInformation( void );

  itkGetMacro( Level, unsigned long );
  itkSetMacro( Level, unsigned long );

  itkGetMacro( IsWeighted, bool );
  itkSetMacro( IsWeighted, bool );

  itkGetMacro( RichClubCoefficient, ValueType );

protected:
  ExtractRichClubGraphFilter();
  virtual ~ExtractRichClubGraphFilter() {}
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  ExtractRichClubGraphFilter(const Self &);           //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  void GenerateData();

  unsigned long    m_Level;

  bool             m_IsWeighted;

  ValueType        m_RichClubCoefficient;

};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkExtractRichClubGraphFilter.hxx"
#endif

#endif /* __itkExtractRichClubGraphFilter_h */
