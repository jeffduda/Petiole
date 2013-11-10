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
#ifndef __itkGraphNodeLocalEfficiencyCalculator_hxx
#define __itkGraphNodeLocalEfficiencyCalculator_hxx

#include "itkGraphNodeLocalEfficiencyCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeLocalEfficiencyCalculator< TInputImage, TValueType >
::GraphNodeLocalEfficiencyCalculator()
{
  this->m_LocalEfficiency = ValueVectorType::New();
  this->m_LocalEfficiency->Initialize();
  this->m_IsWeighted = false;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeLocalEfficiencyCalculator< TInputImage, TValueType >
::Compute(void)
{
  this->m_LocalEfficiency = ValueVectorType::New();
  this->m_LocalEfficiency->Initialize();

  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();

  for ( unsigned int i=0; i<nNodes; i++)
    {
    this->m_LocalEfficiency->InsertElement(i, 0);
    NodeIdentifierContainerType neighbors = this->m_Graph->GetAllNeighbors( i );
    //ValueType triangles = 0;

    if ( neighbors.size() > 1 )
      {
      
      typename SearchGraphType::Pointer searchGraph = SearchGraphType::New();
      for ( unsigned int j=0; j<nNodes; j++ )
        {
        if ( j != i ) 
          {
          searchGraph->CreateNewNode();
          }
        }
      for ( unsigned int j=0; j<this->m_Graph->GetTotalNumberOfEdges(); j++ )
        {
        unsigned long source = this->m_Graph->GetEdge(j).SourceIdentifier;
        unsigned long target = this->m_Graph->GetEdge(j).TargetIdentifier;
        if ( (source != i) && (target != i) )
          {
          if ( source > i )
            {
            --source;
            }
          if ( target > i ) 
            {
            --target;
            }
          typename SearchGraphType::EdgePointerType edge = searchGraph->CreateNewEdge(source,target);
          edge->Weight = 1.0;
          }
        }

      typename DistanceMatrixFilterType::Pointer filter = DistanceMatrixFilterType::New();
      filter->SetInput( searchGraph );
      filter->Update();

      ValueType localEfficiency = 0.0;

      for (unsigned long x1=0; x1 < neighbors.size(); x1++ )
        {
        for (unsigned long x2=x1+1; x2 < neighbors.size(); x2++ )
          {
          localEfficiency += 1.0 / filter->GetDistanceMatrix()(x1,x2);
          }
        }
      
      localEfficiency = localEfficiency / ( neighbors.size() * (neighbors.size()-1 ) );
      this->m_LocalEfficiency->InsertElement(i, localEfficiency );
    
      }
    }
  
}


/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeLocalEfficiencyCalculator< TInputImage, TValueType >
::ComputeWeightedLocalEfficiency(void)
{
  this->m_IsWeighted = true;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeLocalEfficiencyCalculator< TInputImage, TValueType >
::ComputeLocalEfficiency(void)
{
  this->m_IsWeighted = false;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphNodeLocalEfficiencyCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Local Efficiency: "
     << m_LocalEfficiency
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
