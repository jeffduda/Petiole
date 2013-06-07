/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNWBDataFileWriter.h,v $
  Language:  C++
  Date:      $Date: 2009/03/04 23:10:58 $
  Version:   $Revision: 1.17 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNWBDataFileWriter_h
#define __itkNWBDataFileWriter_h

#include "itkGraph.h"
#include "itkArray.h"
#include "itkVectorContainer.h"
#include <fstream>

namespace itk {

/** \class NWBDataFileWriter
 * \brief
 * Writes an itkMesh to a file in various txt file formats.
 *
 */
template <class TInputGraph>
class NWBDataFileWriter : public Object
{
public:
  /** Standard "Self" typedef. */
  typedef NWBDataFileWriter                   Self;
  typedef Object                              Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro( Self );

  /** Write the Input mesh to the Output file.
   * Use either Update() or Write(). */
  void Update( void );
  void Write( void );

  /** Run-time type information (and related methods). */
  itkTypeMacro( NWBDataFileWriter, Object );

  /** Hold on to the type information specified by the template parameters. */
  typedef TInputGraph                            InputGraphType;
  typedef typename TInputGraph::Pointer          InputGraphPointer;
  typedef typename InputGraphType::Superclass    PointSetType;

  typedef Array<unsigned long>                   LineType;
  typedef VectorContainer<long, LineType>        LineSetType;

  /** Set the Input */
  void SetInput( InputGraphType * input );

  InputGraphType * GetInput( ) 
  { return this->m_Input; }

  /** Set/Get the name of the file where data are written. */
  itkSetStringMacro( FileName );
  itkGetStringMacro( FileName );

protected:
  NWBDataFileWriter();
  virtual ~NWBDataFileWriter();

  virtual void GenerateData();

  std::string                         m_FileName;
  InputGraphPointer                   m_Input;

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  NWBDataFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void WriteNWBFile();
  void WriteNodes();
  void WriteEdges();

  std::ofstream m_File;

};

} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNWBDataFileWriter.hxx"
#endif

#endif
