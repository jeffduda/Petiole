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
#ifndef __itkGraphNodeAssortativityCalculator_hxx
#define __itkGraphNodeAssortativityCalculator_hxx

#include "itkGraphNodeAssortativityCalculator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputImage, typename TValueType >
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::GraphNodeAssortativityCalculator()
{
  this->m_Type = UNDIRECTED;
}

/**
 * Compute Min and Max of m_Image
 */
template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::Compute(void)
{

  typename DegreeCalculatorType::ValueVectorPointer degree1;
  typename DegreeCalculatorType::ValueVectorPointer degree2;

  DegreeCalculatorPointer degrees = DegreeCalculatorType::New();
  degrees->SetGraph( this->m_Graph );


  switch ( this->m_Type )
    {
    case UNDIRECTED:
      degrees->ComputeUndirectedDegree();
      degree1 = degrees->GetDegreeCentrality();
      degree2 = degree1;
      break;
    case ININ:
      degrees->ComputeInDegree();
      degree1 = degrees->GetDegreeCentrality();
      degree2 = degree1;
      break;
    case OUTOUT:
      degrees->ComputeOutDegree();
      degree1 = degrees->GetDegreeCentrality();
      degree2 = degree1;
      break;
    case INOUT:
      degrees->ComputeInDegree();
      degree1 = degrees->GetDegreeCentrality();
      degrees->ComputeOutDegree();
      degree2 = degrees->GetDegreeCentrality();
      break;
    case OUTIN:
      degrees->ComputeOutDegree();
      degree1 = degrees->GetDegreeCentrality();
      degrees->ComputeInDegree();
      degree2 = degrees->GetDegreeCentrality();
      break;
    }

  typename ValueVectorType::Pointer values1 = ValueVectorType::New();
  typename ValueVectorType::Pointer values2 = ValueVectorType::New();  

  unsigned long nEdges = this->m_Graph->GetTotalNumberOfEdges();
  for (unsigned int i=0; i<nEdges; i++)
    {
    values1->InsertElement( i, degree1->GetElement(this->m_Graph->GetEdge( i ).SourceIdentifier) );
    values2->InsertElement( i, degree2->GetElement(this->m_Graph->GetEdge( i ).TargetIdentifier) );
    }

  ValueType product = 0.0;
  ValueType sum = 0.0;
  ValueType squaresum = 0.0;

  for (unsigned int i=0; i<nEdges; i++)
    {
    ValueType val1 = values1->GetElement(i);
    ValueType val2 = values2->GetElement(i);
    product += val1*val2;
    sum += (val1 + val2)/2.0;
    squaresum += (val1*val1 + val2*val2)/2.0;
    }

  ValueType term = (sum/nEdges)*(sum/nEdges);
  this->m_Assortativity = ( product/nEdges - term ) / ( squaresum/nEdges - term );
}

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::ComputeUndirectedAssortativity(void)
{
  this->m_Type = UNDIRECTED;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::ComputeDirectedInInAssortativity(void)
{
  this->m_Type = ININ;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::ComputeDirectedOutOutAssortativity(void)
{
  this->m_Type = OUTOUT;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::ComputeDirectedInOutAssortativity(void)
{
  this->m_Type = INOUT;
  this->Compute();
}

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::ComputeDirectedOutInAssortativity(void)
{
  this->m_Type = OUTIN;
  this->Compute();
}

/*
template< class TInputImage >
void
GraphNodeAssortativityCalculator< TInputImage >
::SetRegion(const RegionType & region)
{
  m_Region = region;
  m_RegionSetByUser = true;
}
*/

template< class TInputImage, typename TValueType >
void
GraphNodeAssortativityCalculator< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Assortativity: "
     << this->m_Assortativity
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
