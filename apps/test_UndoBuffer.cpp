
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
