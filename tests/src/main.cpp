#include "../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../cpputest/include/CppUTest/MemoryLeakDetectorNewMacros.h"

IMPORT_TEST_GROUP(Function);
IMPORT_TEST_GROUP(ParserAT);

int main(int argc, char** argv)
{
  return RUN_ALL_TESTS(argc, argv);
}
