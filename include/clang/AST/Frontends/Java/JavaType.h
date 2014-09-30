//===--- JavaType.h - Java Language Family Type Representation ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This file defines the JavaType interface and subclasses.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_JAVATYPE_H
#define LLVM_CLANG_AST_JAVATYPE_H

#include "clang/AST/Type.h"

namespace clang {

class JavaQualifiers : public Qualifiers {
  unsigned JavaMask;
public:
  enum JavaTQ {
    Public = (1 << 0),
    Private = (1 << 1),
    Protected = (1 << 2),
    Static = (1 << 3),
    Final = (1 << 4),
    Native = (1 << 5),
    Synchronized = (1 << 6),
    Abstract = (1 << 7),
    Threadsafe = (1 << 8),
    Transient = (1 << 9),
  };

  JavaQualifiers() : Qualifiers(), JavaMask(0) { }

  bool isPublic() const { return JavaMask & Public; }
  void addPublic() { JavaMask |= Public; }

  bool isPrivate() const { return JavaMask & Private; }
  void addPrivate() { JavaMask |= Private; }

  bool isProtected() const { return JavaMask & Protected; }
  void addProtected() { JavaMask |= Protected; }

  bool isStatic() const { return JavaMask & Static; }
  void addStatic() { JavaMask |= Static; }

  bool isFinal() const { return JavaMask & Final; }
  void addFinal() { JavaMask |= Final; }

  bool isNative() const { return JavaMask & Native; }
  void addNative() { JavaMask |= Native; }

  bool isSynchronized() const { return JavaMask & Synchronized; }
  void addSynchronized() { JavaMask |= Synchronized; }

  bool isAbstract() const { return JavaMask & Abstract; }
  void addAbstract() { JavaMask |= Abstract; }

  bool isThreadsafe() const { return JavaMask & Threadsafe; }
  void addThreadsafe() { JavaMask |= Threadsafe; }

  bool isTransient() const { return JavaMask & Transient; }
  void addTransient() { JavaMask |= Transient; }

};

}

#endif
