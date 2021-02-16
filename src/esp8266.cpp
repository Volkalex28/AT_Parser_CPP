
#include "esp8266.hpp"

esp8266::esp8266(void (*write)(const char * buf, size_t size)) 
  : Parser("AT", write),
    AT(this, "", "\n\r%s\n\r"),
    WIFI(this, "+CWLAP", ":(%i,\"%s\",%i,\"%s\""),
    Echo(this, "E", "%i")
{

}