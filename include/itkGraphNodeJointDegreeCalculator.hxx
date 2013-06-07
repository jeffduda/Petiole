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
#ifndef __itkGraphNodeJointDegreeCalculator_hxx
#define __itkGraphNodeJointDegreeCalculator_hxx

#include "itkGraphNodeJointDegreeCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeJointDegreeCalculator< TInputImage, TValueType >
::GraphNodeJointDegreeCalculator()
{
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeJointDegreeCalculator< TInputImage, TValueType >
::Compute(void)
{
  if ( ! this->m_Graph->IsDirected() )
    {
    itkExceptionMacro( "Joint degree only applies to directed graphs" );
    }
  DegreeCalculatorPointer degrees = DegreeCalculatorType::New();
  degrees->SetGraph( this->m_Graph );

  degrees->ComputeInDegree();
  typename DegreeCalculatorType::ValueVectorPointer inDegree = degrees->GetDegreeCentrality();

  degrees->ComputeOutDegree();
  typename DegreeCalculatorType::ValueVectorPointer outDegree = degrees->GetDegreeCentrality();

  unsigned long nNodes = this->m_Graph->GetTotalNumberOfNodes();

  unsigned long maxIn = 0;
  typename DegreeCalculatorType::ValueVectorType::Iterator inIt = inDegree->Begin();
  while ( inIt != inDegree->End() ) 
    {
    if ( inIt.Value() > maxIn ) 
      {
      maxIn = inIt.Value();
      }
    ++inIt;
    }

  unsigned long maxOut = 0;
  typename DegreeCalculatorType::ValueVectorType::Iterator outIt = outDegree->Begin();
  while ( outIt != outDegree->End() ) 
    {
    if ( outIt.Value() > maxOut ) 
      {
      maxOut = outIt.Value();
      }
    ++outIt;
    }

  this->m_JointDegree.SetSize( maxOut+1, maxIn+1 );
  this->m_JointDegree.Fill( 0.0 );

  for ( unsigned int i=0; i<nNodes; i++ )
    {
    this->m_JointDegree( outDegree->GetElement(i), inDegree->GetElement(i) )++;
    }
}

/*
template< class TInputImage >
void
GraphNodeJointDegreeCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeJointDegreeCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "JointDegree: "
     << this->m_JointDegree
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
