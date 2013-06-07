/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphToGraphFilter.hxx,v $
  Language:  C++
  Date:      $Date: 2008/11/11 11:51:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGraphToGraphFilter_hxx
#define __itkGraphToGraphFilter_hxx

#include "itkGraphToGraphFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputGraph, class TOutputGraph>
GraphToGraphFilter<TInputGraph,TOutputGraph>
::GraphToGraphFilter()
{
  // Modify superclass default values, can be overridden by subclasses
  this->SetNumberOfRequiredInputs( 1 );
  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
}

/**
 *
 */
template< class TInputGraph, class TOutputGraph >
void
GraphToGraphFilter< TInputGraph, TOutputGraph >
::SetInput(const InputGraphType *input)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( 0,
                                    const_cast< InputGraphType * >( input ) );
}

/**
 * Connect one of the operands for pixel-wise addition
 */
template< class TInputGraph, class TOutputGraph >
void
GraphToGraphFilter< TInputGraph, TOutputGraph >
::SetInput(unsigned int index, const TInputGraph *image)
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput( index,
                                    const_cast< TInputGraph * >( image ) );
}

/**
 *
 */
template< class TInputGraph, class TOutputGraph >
const typename GraphToGraphFilter< TInputGraph, TOutputGraph >::InputGraphType *
GraphToGraphFilter< TInputGraph, TOutputGraph >
::GetInput(void) const
{
  return itkDynamicCastInDebugMode< const TInputGraph * >( this->GetPrimaryInput() );
}

/**
 *
 */
template< class TInputGraph, class TOutputGraph >
const typename GraphToGraphFilter< TInputGraph, TOutputGraph >::InputGraphType *
GraphToGraphFilter< TInputGraph, TOutputGraph >
::GetInput(unsigned int idx) const
{
  const TInputGraph *in = dynamic_cast< const TInputGraph * >
    ( this->ProcessObject::GetInput(idx) );

  if ( in == NULL && this->ProcessObject::GetInput(idx) != NULL )
    {
    itkWarningMacro (<< "Unable to convert input number " << idx << " to type " <<  typeid( InputGraphType ).name () );
    }
  return in;
}

} // end namespace itk

#endif
