#include <iostream>
#include <string>

#define CPPUTEST \
  friend class TEST_ParserAT_testingCorrectInitializationParser_Test; \
  friend class TEST_ParserAT_testingPrivatePrintFunctionParser_Test; \
  friend class TEST_ParserAT_testingPrivateConvertFunctionParser_Test; \
  friend class TEST_ParserAT_testingPrivateFindCommands_Test

#include "../../src/Parser.hpp"
#include "../../cpputest/include/CppUTest/TestHarness.h"

struct testParser : public Parser<4>
{
  std::string str;

  AT_Command<> AT;
  AT_Command<int, string_t, int, string_t> CWLAP;
  AT_Command<int> Echo;
  AT_Command<> Event;

  int compare(const char * str)
  {
    const int result = this->str.compare(str);
    this->str.clear();
    return result;
  }

  testParser() 
    : Parser("AT", [&](const char * str) { this->str += str; }),
      str(""),
      AT(this, "", ""),
      CWLAP(this, "+CWLAP", "#:(%,\"%\",%,\"%\""),
      Echo(this, "E", "AT#%"),
      Event(this, "", "WIFI_CONNECTED")
  { 

  }
};

using string_t = testParser::string_t;

TEST_GROUP(ParserAT)
{
  struct testParser test;
};

TEST(ParserAT, testingCorrectInitializationParser)
{
  CHECK_EQUAL(4, test.arrAT.size());

  CHECK_EQUAL(&test.AT, test.arrAT.at(0));
  CHECK_EQUAL(&test.CWLAP, test.arrAT.at(1));
  CHECK_EQUAL(&test.Echo, test.arrAT.at(2));
  CHECK_EQUAL(&test.Event, test.arrAT.at(3));

  CHECK_EQUAL(0, test.prefix.compare("AT"));
}

TEST(ParserAT, testingPrivateConvertFunctionParser)
{
  CHECK_EQUAL(156, test.convert(string_t("156"), (int *)nullptr));
  CHECK_EQUAL(15, test.convert(string_t("15.6"), (int *)nullptr));

  CHECK_EQUAL(0, test.convert(string_t("Hello"), (string_t *)nullptr).compare(string_t("Hello")));

  CHECK_EQUAL(0, strcmp(test.convert(string_t("15.6"), (const char **)nullptr), "15.6"));
}

TEST(ParserAT, testingPrivatePrintFunctionParser)
{
  test.print(2);
  CHECK_EQUAL(0, test.compare("2"));

  test.print("Hello World");
  CHECK_EQUAL(0, test.compare("Hello World"));

  test.print(string_t("Hello World from string"));
  CHECK_EQUAL(0, test.compare("Hello World from string"));

  test.print("Hello ", "World", string_t(" from multiply"));
  CHECK_EQUAL(0, test.compare("Hello World from multiply"));

  test.print("Hello", string_t(" "), "World", string_t(" from multiply"), " ", 1, 2.9);
  CHECK_EQUAL(0, test.compare("Hello World from multiply 12"));
}

TEST(ParserAT, testingPrivateFindCommands)
{
  using AT_t = testParser::AT_base;

  CHECK_EQUAL((AT_t *)nullptr, test.findAT(""));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT(" "));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT("AT"));

  CHECK_EQUAL(&test.CWLAP, test.findAT("+CWLAP:("));
  CHECK_EQUAL(&test.CWLAP, test.findAT("+CWLAP:(g"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT("+CWLAP:"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT(" +CWLAP:("));

  CHECK_EQUAL(&test.Echo, test.findAT("ATE"));
  CHECK_EQUAL(&test.Echo, test.findAT("ATE85"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT(" ATE"));
  
  CHECK_EQUAL(&test.Event, test.findAT("WIFI_CONNECTED"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT("WIFI_CONNECTED "));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT(" WIFI_CONNECTED"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT("WIFI"));
  CHECK_EQUAL((AT_t *)nullptr, test.findAT("WIFI_CONNECTED\r\n"));
}

TEST(ParserAT, testingPrintATCommands)
{
  test.AT();
  CHECK_EQUAL(0, test.compare("AT\n\r"));

  test.Echo(1);
  CHECK_EQUAL(0, test.compare("ATE1\n\r"));

  test.CWLAP("=","\"TestNameNetwork\"",",","\"TestPasswordNetwork\"");
  CHECK_EQUAL(0, test.compare("AT+CWLAP=\"TestNameNetwork\",\"TestPasswordNetwork\"\n\r"));
}

TEST(ParserAT, testingResponseATCommands)
{
  test.AT >> [](void)
  {
    CHECK(false);
  };
  test.CWLAP >> [](int i1, string_t str1, int i2, string_t str2)
  {
    CHECK_EQUAL(2, i1);
    CHECK_EQUAL(0, str1.compare("Home"));
    CHECK_EQUAL(-70, i2);
    CHECK_EQUAL(0, strcmp("18:31:bf:67:bd:cc", str2.c_str()));
  };
  test.Echo >> [](int i)
  {
    CHECK_EQUAL(2, i);
  };

  test.Parse("+CWLAP:(2,\"Home\",-70,\"18:31:bf:67:bd:cc\",12,23,0,4,4,7,1)");
  test.Parse("ATE2");
  test.Parse("");
}
