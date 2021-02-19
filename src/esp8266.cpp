
#include "esp8266.hpp"

esp8266::esp8266(printf_t && write) 
  : Parser("AT", write),
    AT(this, "\n\r", "\r%\n"),
    CWLAP(this, "+CWLAP", ":(%,\"%\",%,\"%\""),
    Echo(this, "E", "%")
{
  
}