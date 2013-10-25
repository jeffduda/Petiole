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
#ifndef __itkGraphNodeDegreeCentralityCalculator_hxx
#define __itkGraphNodeDegreeCentralityCalculator_hxx

#include "itkGraphNodeDegreeCentralityCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::GraphNodeDegreeCentralityCalculator()
{
  this->m_DegreeCentrality = ValueVectorType::New();
  this->m_DegreeCentrality->Initialize();
  this->m_DegreeType = DEGREE_SYMMETRIC;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::Compute(void)
{
  this->m_DegreeCentrality = ValueVectorType::New();
  this->m_DegreeCentrality->Initialize();

  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();
  //unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();
  
  for ( unsigned int i=0; i<nNodes; i++)
    {
    if ( this->m_DegreeType == DEGREE_SYMMETRIC )
      {
      this->m_DegreeCentrality->InsertElement(i, this->m_Graph->GetAllNeighbors( i ).size() );
      }
    else if ( this->m_DegreeType == DEGREE_OUT )
      {
      this->m_DegreeCentrality->InsertElement(i, this->m_Graph->GetOutgoingNeighbors( i ).size() );
      }
    else if ( this->m_DegreeType == DEGREE_IN )
      {
      this->m_DegreeCentrality->InsertElement(i, this->m_Graph->GetIncomingNeighbors( i ).size() );
      }
    }
  /*
  for ( unsigned int i=0; i<nNodes; i++)
    {
    this->m_DegreeCentrality->InsertElement(i,0);
    }

  for ( unsigned int i=0; i<nEdges; i++)
    {
    unsigned int source = this->m_Graph->GetEdge( i ).SourceIdentifier;
    unsigned int target = this->m_Graph->GetEdge( i ).TargetIdentifier;
    //std::cout << source << "," << target << std::endl;
    
    if ( this->m_DegreeType == DEGREE_SYMMETRIC )
      {
      this->m_DegreeCentrality->SetElement( source, this->m_DegreeCentrality->GetElement(source)+1 );
      this->m_DegreeCentrality->SetElement( target, this->m_DegreeCentrality->GetElement(target)+1 );
      }
    if ( this->m_DegreeType == DEGREE_OUT )
      {
      this->m_DegreeCentrality->SetElement( source, this->m_DegreeCentrality->GetElement(source)+1 );
      }
    if ( this->m_DegreeType == DEGREE_IN )
      {
      this->m_DegreeCentrality->SetElement( target, this->m_DegreeCentrality->GetElement(target)+1 );
      }
    }
  */
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::ComputeInDegree(void)
{
  this->m_DegreeType = DEGREE_IN;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::ComputeOutDegree(void)
{
  this->m_DegreeType = DEGREE_OUT;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::ComputeUndirectedDegree(void)
{
  this->m_DegreeType = DEGREE_SYMMETRIC;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphNodeDegreeCentralityCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeDegreeCentralityCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Centrality: "
     << m_DegreeCentrality
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
