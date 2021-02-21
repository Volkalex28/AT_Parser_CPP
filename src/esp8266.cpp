
#include "esp8266.hpp"

esp8266::esp8266(printf_t write) 
  : Parser("AT", write),
    AT(this, "", ""),
    CWLAP(this, "+CWLAP", "#:(%,\"%\",%,\"%\""),
    Echo(this, "E", "AT#%"),
    Test(this, "", "WIFI_CONNECTED")
{
  using namespace std;

  this->CWLAP >> [&](int c, string_t s1, int d, string_t s2)
  {
    cout << "Name: " << s1 << "; MAC: " << s2 << " " << c << " " << d << endl;
  };
  this->AT >> []()
  {
    cout << "AT" << endl;
  };
  this->Echo >> [](int val)
  {
    cout << val << endl;
  };
  this->Test >> []()
  {
    cout << "))))" << endl;
  };
  
}