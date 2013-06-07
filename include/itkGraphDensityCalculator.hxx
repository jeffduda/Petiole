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
#ifndef __itkGraphDensityCalculator_hxx
#define __itkGraphDensityCalculator_hxx

#include "itkGraphDensityCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphDensityCalculator< TInputImage, TValueType >
::GraphDensityCalculator()
{
  this->m_Density = 0;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphDensityCalculator< TInputImage, TValueType >
::Compute(void)
{
  if ( this->m_Graph->IsDirected() )
    {
    this-ComputeDirectedDensity();
    }
  else
    {
    this->ComputeUndirectedDensity();
    }
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphDensityCalculator< TInputImage, TValueType >
::ComputeUndirectedDensity(void)
{
  ValueType nNodes = this->m_Graph->GetTotalNumberOfNodes();
  this->m_Density = this->m_Graph->GetTotalNumberOfEdges();
  this->m_Density /= ( (nNodes*nNodes-nNodes)/2.0 );
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphDensityCalculator< TInputImage, TValueType >
::ComputeDirectedDensity(void)
{
  ValueType nNodes = this->m_Graph->GetTotalNumberOfNodes();
  this->m_Density = this->m_Graph->GetTotalNumberOfEdges();
  this->m_Density /= (nNodes*nNodes-nNodes);
}

/*
template< class TInputImage >
void
GraphDensityCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphDensityCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Density: "
     << m_Density
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
