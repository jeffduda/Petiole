/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphFileWriter.h,v $
  Language:  C++
  Date:      $Date: 2008/11/11 11:51:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGraphFileWriter_h
#define __itkGraphFileWriter_h

#include "itkGraph.h"
#include "itkMesh.h"
#include "itkVtkPolyDataFileWriter.h"


namespace itk {

/** \class GraphFileWriter
 * \brief
 * Writes an itkMesh to a file in various txt file formats.
 *
 */
template < class TInputGraph, class TInputImage=Image<float,TInputGraph::GraphTraitsType::ImageDimension> >
class GraphFileWriter : public Object
{
public:
  /** Standard "Self" typedef. */
  typedef GraphFileWriter             Self;
  typedef Object                      Superclass;
  typedef SmartPointer<Self>          Pointer;
  typedef SmartPointer<const Self>    ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro( Self );

  /** Write the Input mesh to the Output file.
   * Use either Update() or Write(). */
  void Update( void );
  void Write( void );

  /** Extract dimension from the output mesh. */
  //itkStaticConstMacro( Dimension, unsigned int,
  //                     TInputMesh::PointType::Dimension );


  /** Run-time type information (and related methods). */
  itkTypeMacro( GraphFileWriter, Object );

  /** Hold on to the type information specified by the template parameters. */
  typedef TInputGraph                              InputGraphType;
  typedef typename TInputGraph::Pointer            InputGraphPointer;
  typedef typename InputGraphType::GraphTraitsType GraphTraitsType;
  typedef typename GraphTraitsType::IndexType      IndexType;

  typedef Mesh<float, TInputGraph::GraphTraitsType::ImageDimension>   MeshType;
  typedef typename MeshType::PointType                                PointType;
  typedef VtkPolyDataFileWriter<MeshType>                             VtkWriterType;

  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::IndexType       ImageIndexType;

  /** Set the Input */
  void SetInput( InputGraphType * input );

  /** Set/Get the name of the file where data are written. */
  itkSetStringMacro( FileName );
  itkGetStringMacro( FileName );

protected:
  GraphFileWriter();
  virtual ~GraphFileWriter();

  virtual void GenerateData();

  std::string                          m_FileName;
  InputGraphPointer                    m_Input;

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  GraphFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  void WriteVtkPolyData();

  void WriteDot();

};

} // namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkGraphFileWriter.hxx"
#endif

#endif
