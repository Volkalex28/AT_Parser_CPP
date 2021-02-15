
#include "esp8266.hpp"

esp8266::esp8266(void (*write)(const char * buf, size_t size)) 
  : Parser("AT", write),
    AT(this, "", "\n\r%s\n\r"),
    WIFI(this, "+CWLAP", ":(%i,\"%s\",%i,\"%s\",%i,%i,%i,%i,%i,%i,%i)"),
    Echo(this, "E", "%i")
{
  this->AT.foo = [&](std::string p)
  {

  };

  this->WIFI.foo = [&](int, std::string, int, std::string, int, int, int,
  int, int, int, int)
  {

  };

  this->Echo.foo = [&](int)
  {

  };
}