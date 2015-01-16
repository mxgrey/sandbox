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

#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <functional>
#include <list>

class Action
{
public:

  virtual ~Action() { }

  virtual void redo() = 0;
  virtual void undo() = 0;
};

template<typename... FunctionParams>
class ReversibleAction : public Action
{
public:
  ReversibleAction( void (*f_redo)(FunctionParams...),
                   void (*f_undo)(FunctionParams...),
                   FunctionParams... args)
  {
    my_f_redo = [=] { (*f_redo)(args...); };
    my_f_undo = [=] { (*f_undo)(args...); };
  }

  std::function<void()> my_f_redo;
  std::function<void()> my_f_undo;

  void redo() { my_f_redo(); }
  void undo() { my_f_undo(); }
};

template<class MyClass, typename... FunctionParams>
class ReversibleClassAction : public Action
{
public:
  ReversibleClassAction( MyClass* instance,
                        void (MyClass::*f_redo)(FunctionParams...),
                        void (MyClass::*f_undo)(FunctionParams...),
                        FunctionParams... args)
  {
    my_f_redo = [=] { (instance->*f_redo)(args...); };
    my_f_undo = [=] { (instance->*f_undo)(args...); };
  }

  std::function<void()> my_f_redo;
  std::function<void()> my_f_undo;

  void redo() { my_f_redo(); }
  void undo() { my_f_undo(); }
};

class SwapAction : public Action
{
public:
  SwapAction(Action* previousAction)
  {
    my_f_redo = [=] () { previousAction->redo(); };
    my_f_undo = [=] () { previousAction->undo(); };
  }

  std::function<void()> my_f_redo;
  std::function<void()> my_f_undo;

  void redo() { my_f_redo(); }
  void undo() { my_f_undo(); }
};

enum {
  UNLIMITED_BUFFER = -1
};

class UndoRedoBuffer
{
public:

  inline UndoRedoBuffer(int buffer_limit = UNLIMITED_BUFFER) :
    _buffer_limit(buffer_limit)
  {
    _current = _action_list.end();
  }

  inline void addAction(Action* newAction)
  {
    if(_buffer_limit == 0)
      return;

    if(_current != _action_list.begin())
    {
      _action_list.erase(_action_list.begin(), _current);
      // TODO: Instead of erasing these actions, we should create a new batch
      // of actions which are the reverse of all these actions. That way,
      // the user can later use undo in order to redo these actions. We can use
      // the SwapAction class to take care of this.
    }

    if( _buffer_limit != UNLIMITED_BUFFER
       && (int)_action_list.size() >= _buffer_limit )
    {
      _remove(--_action_list.end());
    }

    _action_list.push_front(newAction);
    _current = _action_list.begin();

    return;
  }

  inline void undo()
  {
    if(_current != _action_list.end())
    {
      (*_current)->undo();
      ++_current;
    }
  }

  inline void redo()
  {
    if(_current != _action_list.begin())
    {
      --_current;
      (*_current)->redo();
    }
  }

protected:

  inline void _remove(std::list<Action*>::iterator at)
  {
    if(at == _action_list.end())
      return;

    delete (*at);
    _action_list.erase(at);
  }

  inline void _remove(std::list<Action*>::iterator from,
              std::list<Action*>::iterator to)
  {
    if(from == _action_list.end())
      return;

    for(; from != to; ++from)
    {
      delete (*from);
    }

    _action_list.erase(from, to);
  }

  std::list<Action*> _action_list;
  std::list<Action*>::iterator _current;
  int _buffer_limit;

};

#endif // UNDOREDO_HPP
