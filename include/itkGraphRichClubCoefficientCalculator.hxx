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
#ifndef __itkGraphRichClubCoefficientCalculator_hxx
#define __itkGraphRichClubCoefficientCalculator_hxx

#include "itkGraphRichClubCoefficientCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphRichClubCoefficientCalculator< TInputImage, TValueType >
::GraphRichClubCoefficientCalculator()
{
  this->m_Level = 0;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphRichClubCoefficientCalculator< TInputImage, TValueType >
::Compute(void)
{
  DegreeCalculatorPointer degrees = DegreeCalculatorType::New();
  degrees->SetGraph( this->m_Graph );
  degrees->ComputeUndirectedDegree();
  
  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();
  unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();
  typename GraphType::RegionType::Pointer subgraph = GraphType::RegionType::New();
  unsigned long idx = 0;
  for ( unsigned int i=0; i<nNodes; i++)
    {
    if ( degrees->GetDegreeCentrality()->GetElement(i) > this->m_Level )
      {
      subgraph->InsertElement( idx, i );
      ++idx;
      } 
    }

  if ( subgraph->Size() < 2 )
    {
    this->m_RichClubCoefficient = 0;
    }
  else
    {
    std::vector<float> weights;
    typename GraphType::RegionType::Pointer subedges = GraphType::RegionType::New();
    idx = 0;
    ValueType weightsum = 0.0;
    for ( unsigned int i=0; i<nEdges; i++)
      {
      weights.push_back( this->m_Graph->GetEdge(i).Weight );
      if ( ( degrees->GetDegreeCentrality()->GetElement( this->m_Graph->GetEdge( i ).SourceIdentifier ) > this->m_Level ) &&
           ( degrees->GetDegreeCentrality()->GetElement( this->m_Graph->GetEdge( i ).TargetIdentifier ) > this->m_Level ) )
        {
        subedges->InsertElement(idx,i);
        weightsum += this->m_Graph->GetEdge( i ).Weight;
        ++idx;
        }
      }
    
    ValueType rcNodes = subgraph->Size();
    ValueType rcEdges = subedges->Size();
    
    if ( this->m_Graph->IsUndirected() )
      {
      rcEdges *= 2.0;
      }

    if ( this->m_IsWeighted )
      {
      std::sort( weights.begin(), weights.end() );
      ValueType normweighted = 0.0;
      for (unsigned int i=(nEdges-subedges->Size()); i<nEdges; i++)
        {
        normweighted += weights[i];
        }

      if ( this->m_Graph->IsUndirected() )
        {
        weightsum *= 2.0;
        }
      
      this->m_RichClubCoefficient = weightsum / normweighted;
      }
    else
      {
      this->m_RichClubCoefficient = rcEdges / ( rcNodes * (rcNodes-1) ); 
      }
    }
}

template< class TInputImage, typename TValueType >
void
GraphRichClubCoefficientCalculator< TInputImage, TValueType >
::ComputeWeighted(void)
{
  this->m_IsWeighted = true;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphRichClubCoefficientCalculator< TInputImage, TValueType >
::ComputeUnweighted(void)
{
  this->m_IsWeighted = false;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphRichClubCoefficientCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphRichClubCoefficientCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "RichClubCoefficient: "
     << this->m_RichClubCoefficient
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
