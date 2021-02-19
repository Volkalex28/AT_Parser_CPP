#include <iostream>
#include "esp8266.hpp"

using namespace std;

template<typename ... Types>
struct test
{
  
};

int main(void)
{

  esp8266 esp([](const char * buf){ cout << buf; });

  esp.CWLAP >> [&](int c, esp8266::string_t s1, int d, esp8266::string_t s2)
  {
    cout << "Name: " << s1 << "; MAC: " << s2 << " " << c << " " << d << endl;
  };
  esp.AT >> [&](esp8266::string_t str)
  {
    cout << "AT" << endl;
  };
  esp.Echo >> [&](int val)
  {
    cout << val << endl;
  };

  esp.CWLAP("=\"","5G_Obluchatel", "\",\"", "Kikimora11", "\"");
  esp.Parse("+CWLAP:(3,\"arin\",-70,\"18:31:bf:67:bd:cc\",12,23,0,4,4,7,1)");
  esp.Parse("E20515");

  return 0;

}
