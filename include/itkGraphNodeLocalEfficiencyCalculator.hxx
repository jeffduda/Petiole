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
#ifndef __itkGraphNodeClusteringCoefficientCalculator_hxx
#define __itkGraphNodeClusteringCoefficientCalculator_hxx

#include "itkGraphNodeClusteringCoefficientCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeClusteringCoefficientCalculator< TInputImage, TValueType >
::GraphNodeClusteringCoefficientCalculator()
{
  this->m_ClusteringCoefficient = ValueVectorType::New();
  this->m_ClusteringCoefficient->Initialize();
  this->m_IsWeighted = false;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeClusteringCoefficientCalculator< TInputImage, TValueType >
::Compute(void)
{
  this->m_ClusteringCoefficient = ValueVectorType::New();
  this->m_ClusteringCoefficient->Initialize();

  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();

  for ( unsigned int i=0; i<nNodes; i++)
    {
    this->m_ClusteringCoefficient->InsertElement(i, 0);
    NodeIdentifierContainerType neighbors = this->m_Graph->GetAllNeighbors( i );
    ValueType triangles = 0;

    if ( neighbors.size() > 1 )
      {
      
      for ( unsigned int n=0; n<neighbors.size(); n++)
        {
        for ( unsigned int m=0; m<neighbors.size(); m++)
          {
          if ( neighbors[m] != neighbors[n] )
            {
            if ( this->m_Graph->NodesAreNeighbors(neighbors[n],neighbors[m] ) ) 
              {
              if ( this->m_IsWeighted )
                {
                triangles += this->m_Graph->GetNodeWeight( i );
                triangles += this->m_Graph->GetNodeWeight( neighbors[n] );
                triangles += this->m_Graph->GetNodeWeight( neighbors[m] );
                }
              else
                {
                ++triangles;
                }
              }
            }
          }
        ValueType denom = neighbors.size() * neighbors.size() - neighbors.size();
        this->m_ClusteringCoefficient->InsertElement( i, triangles / denom );
        }
      }
    else 
      {
      this->m_ClusteringCoefficient->InsertElement( i, 0 );
      }
    
    }
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeClusteringCoefficientCalculator< TInputImage, TValueType >
::ComputeWeightedClusteringCoefficient(void)
{
  this->m_IsWeighted = true;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeClusteringCoefficientCalculator< TInputImage, TValueType >
::ComputeClusteringCoefficient(void)
{
  this->m_IsWeighted = false;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphNodeClusteringCoefficientCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeClusteringCoefficientCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Centrality: "
     << m_ClusteringCoefficient
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
