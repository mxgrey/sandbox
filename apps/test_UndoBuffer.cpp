/*
 * Copyright (c) 2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author: Michael Grey <mxgrey@gatech.edu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of sandbox nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include "UndoRedo.hpp"

class MyNumber
{
public:

  MyNumber() : number(0) { }

  double number;

  void add(double otherNumber)
  {
    number += otherNumber;
  }

  void subtract(double otherNumber)
  {
    number -= otherNumber;
  }
};

void add(MyNumber* number, double otherNumber)
{
  number->add(otherNumber);
}

void subtract(MyNumber* number, double otherNumber)
{
  number->subtract(otherNumber);
}

int main()
{
  UndoRedoBuffer<-1> undoer;
  MyNumber num;

  std::cout << num.number << std::endl;

  std::cout << "+5" << std::endl;
  num.add(5);
  undoer.addAction(new TemplatedClassAction<MyNumber,double>(
      &num, &MyNumber::add, &MyNumber::subtract, 5));

  std::cout << num.number << std::endl;

  std::cout << "+8" << std::endl;
  num.add(8);
  undoer.addAction(new TemplatedClassAction<MyNumber,double>(
      &num, &MyNumber::add, &MyNumber::subtract, 8));

  std::cout << num.number << std::endl;

  std::cout << "-3" << std::endl;
  num.subtract(3);
  undoer.addAction(new TemplatedClassAction<MyNumber,double>(
      &num, &MyNumber::subtract, &MyNumber::add, 3));

  std::cout << num.number << std::endl;

  std::cout << "+10" << std::endl;
  num.add(10);
  undoer.addAction(new TemplatedClassAction<MyNumber,double>(
      &num, &MyNumber::add, &MyNumber::subtract, 10));

  std::cout << num.number << std::endl;

  std::cout << "undo" << std::endl;
  undoer.undo();

  std::cout << num.number << std::endl;

  std::cout << "undo" << std::endl;
  undoer.undo();

  std::cout << num.number << std::endl;

  std::cout << "undo" << std::endl;
  undoer.undo();

  std::cout << num.number << std::endl;

  std::cout << "redo" << std::endl;
  undoer.redo();

  std::cout << num.number << std::endl;

  std::cout << "+25" << std::endl;
  num.add(25);
  undoer.addAction(new TemplatedClassAction<MyNumber,double>(
      &num, &MyNumber::add, &MyNumber::subtract, 25));

  std::cout << num.number << std::endl;

  std::cout << "undo" << std::endl;
  undoer.undo();

  std::cout << num.number << std::endl;

  std::cout << "redo" << std::endl;
  undoer.redo();

  std::cout << num.number << std::endl;

  std::cout << "-4" << std::endl;
  subtract(&num, 4);
  undoer.addAction(new TemplatedAction<MyNumber*,double>(
      &subtract, &add, &num, 4));

  std::cout << num.number << std::endl;

  std::cout << "undo" << std::endl;
  undoer.undo();

  std::cout << num.number << std::endl;
}
