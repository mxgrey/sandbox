
//#warning This is a warning\
// all about how\
// my life got flip-turned upside down

#define GZ_STRINGIFY(x) #x

#if defined __GNUC__

#define GZ_COMPILE_WARNING_HELPER(x) \
  GZ_STRINGIFY(GCC warning x)

#define GZ_COMPILE_WARNING(x) \
  _Pragma(GZ_COMPILE_WARNING_HELPER(#x))

#elif defined _MSC_VER

#define GZ_COMPILE_WARNING(x) \
  __pragma(message(x))

#endif

#define SOMEMACRO 1 GZ_COMPILE_WARNING(SOMEMACRO is deprecated)

int main()
{
  int i = SOMEMACRO;
}
