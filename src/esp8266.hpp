#pragma once

#include "Parser.hpp"

class esp8266 : public Parser<3>
{
public:
  AT_Command<esp8266::string_t> AT;
  AT_Command<int, esp8266::string_t, int, esp8266::string_t> CWLAP;
  AT_Command<int> Echo;

  esp8266(printf_t && write);
};

