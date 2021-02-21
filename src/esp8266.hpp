#pragma once

#include "Parser.hpp"

class esp8266 : public Parser<4>
{
public:
  AT_Command<> AT;
  AT_Command<int, esp8266::string_t, int, esp8266::string_t> CWLAP;
  AT_Command<int> Echo;
  AT_Command<> Test;

  esp8266(printf_t write);
};

