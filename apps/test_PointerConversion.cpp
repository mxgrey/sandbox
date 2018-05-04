

void foo(void* pointer)
{
}

//void bar(void** pointer)
//{
//}


int main()
{
  int value = 10;
  int* pointerToValue = &value;
  int ** pointerToPointer = &pointerToValue;

  foo(pointerToPointer);
//  bar(pointerToPointer);
}
