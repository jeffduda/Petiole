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
#ifndef __itkGraphNodeStrengthCalculator_hxx
#define __itkGraphNodeStrengthCalculator_hxx

#include "itkGraphNodeStrengthCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeStrengthCalculator< TInputImage, TValueType >
::GraphNodeStrengthCalculator()
{
  m_Strength = ValueVectorType::New();
  m_Strength->Initialize();
  m_StrengthType = STRENGTH_SYMMETRIC;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeStrengthCalculator< TInputImage, TValueType >
::Compute(void)
{
  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();
  unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();

  for ( unsigned int i=0; i<nNodes; i++)
    {
    this->m_Strength->InsertElement(i, 0);
    }
  for ( unsigned int i=0; i<nEdges; i++)
    {
    unsigned long source = this->m_Graph->GetEdge( i ).SourceIdentifier;
    unsigned long target = this->m_Graph->GetEdge( i ).TargetIdentifier;
    ValueType weight = this->m_Graph->GetEdge( i ).Weight;

    if (( this->m_StrengthType == STRENGTH_SYMMETRIC ) || ( this->m_StrengthType == STRENGTH_OUT ) )
      {
      this->m_Strength->SetElement( source, this->m_Strength->GetElement( source )+weight );
      }
    if (( this->m_StrengthType == STRENGTH_SYMMETRIC ) || ( this->m_StrengthType == STRENGTH_IN ) )
      {
      this->m_Strength->SetElement( target, this->m_Strength->GetElement( target )+weight );
      }                       
    }
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeStrengthCalculator< TInputImage, TValueType >
::ComputeInStrength(void)
{
  this->m_StrengthType = STRENGTH_IN;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeStrengthCalculator< TInputImage, TValueType >
::ComputeOutStrength(void)
{
  this->m_StrengthType = STRENGTH_OUT;
  this->Compute();
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeStrengthCalculator< TInputImage, TValueType >
::ComputeUndirectedStrength(void)
{
  this->m_StrengthType = STRENGTH_SYMMETRIC;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphNodeStrengthCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeStrengthCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Strength: "
     << m_Strength
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
