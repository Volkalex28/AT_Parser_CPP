#include <iostream>
#include "esp8266.hpp"

using namespace std;

void Print(const char * buf, size_t size)
{
  cout << buf;
}

int main(void)
{
  esp8266 esp(Print);

  //esp.WIFI("=\"", "5G_Obluchatel", "\",\"", string("Kikimora11"), "\"");
  esp.Parse("+CWLAP:(3,\"arin\",-70,\"18:31:bf:67:bd:cc\",12,23,0,4,4,7,1)\r\n");
  return 0;
}