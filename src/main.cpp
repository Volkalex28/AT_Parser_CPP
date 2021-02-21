#include <iostream>
#include "esp8266.hpp"

using namespace std;

esp8266 esp([](const char * buf){ cout << buf; });

int main(void)
{

  esp.CWLAP("=\"","5G_Obluchatel", "\",\"", "Kikimora11", "\"", 5);
  esp.AT();
  esp.Parse("+CWLAP:(3,\"arin\",-70,\"18:31:bf:67:bd:cc\",12,23,0,4,4,7,1)");
  esp.Parse("ATE20515");
  esp.Parse("WIFI_CONNECTED");
  esp.Parse("");

}
