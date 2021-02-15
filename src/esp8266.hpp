#pragma once

#include "Parser.hpp"

class esp8266 : public Parser
{
public:
  AT_Command<std::string> AT;
  AT_Command<int, std::string, int, std::string, int, int, int, int, int,
    int, int> WIFI;
  AT_Command<int> Echo;

  esp8266(void (*write)(const char * buf, size_t size));
};

