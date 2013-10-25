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
#ifndef __itkDensityThresholdGraphFilter_hxx
#define __itkDensityThresholdGraphFilter_hxx

#include "itkDensityThresholdGraphFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkArray.h"

namespace itk
{
/**
 * Constructor
 */
template< class TInputGraph, typename TOutputGraph >
DensityThresholdGraphFilter< TInputGraph, TOutputGraph >
::DensityThresholdGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 1 );

  OutputGraphPointer output
    = dynamic_cast<OutputGraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_Density = 1.0;
}


template <class TInputGraph, class TOutputGraph>
void
DensityThresholdGraphFilter<TInputGraph,TOutputGraph>
::GenerateData( void )
{
  OutputGraphPointer output = this->GetOutput();  
  unsigned long nNodes = this->GetInput()->GetTotalNumberOfNodes();
  unsigned long nEdges = this->GetInput()->GetTotalNumberOfEdges();

  float indensity = (float)nEdges /( nNodes * (nNodes-1.0) );
  if ( ! this->GetInput()->IsDirected() )
    {
    indensity *= 2;
    }

  if ( indensity < m_Density ) 
    {
    itkExceptionMacro( "itk::DensityThresholdGraphFilter: Input density is less than threshold level, no output written" );
    }
  

  unsigned int nOutEdges = m_Density * nNodes * (nNodes-1) / 2.0;
  std::vector<std::pair<float,unsigned long> > vp;
  vp.reserve( nEdges );
  for (unsigned long i = 0; i < nEdges; i++) 
    {
    float w = this->GetInput()->GetEdgeWeight(i);
    vp.push_back(std::make_pair(w, i));
    }

  std::sort(vp.begin(), vp.end());

  for ( unsigned int i=0; i<nNodes; i++)
    {
    output->CreateNewNode();
    }

  for (unsigned long i = 0; i < nEdges; i++) 
    {
    std::cout << vp[i].second << " - " << vp[i].first << std::endl;
    }

  for (unsigned long i = 0; i != vp.size(); i++) 
    {
    if ( i > ( vp.size() - 1 - nOutEdges) ) 
      {   
      typename OutputGraphType::EdgePointerType edge = output->CreateNewEdge();

      edge->SourceIdentifier = this->GetInput()->GetEdge(vp[i].second).SourceIdentifier;
      edge->TargetIdentifier = this->GetInput()->GetEdge(vp[i].second).TargetIdentifier;

      if ( m_Binarize ) 
        {
        edge->Weight = 1;
        }
      else
        {
        edge->Weight = this->GetInput()->GetEdgeWeight(vp[i].second);
        }
      }
    }
  output->SetIncomingAndOutgoingEdges();
  output->SetIsDirected( this->GetInput()->IsDirected() );
}

template <class TInputGraph, class TOutputGraph>
void
DensityThresholdGraphFilter<TInputGraph,TOutputGraph>
::GenerateOutputInformation( void )
{
}

template< class TInputImage, typename TValueType >
void
DensityThresholdGraphFilter< TInputImage, TValueType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "DensityThresholdGraphFilter: "
     << std::endl;
  //os << indent << "Region set by User: " << m_RegionSetByUser << std::endl;
}
} // end namespace itk

#endif
