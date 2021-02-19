#pragma once

template<std::size_t ...>
struct seq 
{ 

};

template<std::size_t N, std::size_t ... S>
struct gens : gens<N-1, N-1, S ...> 
{ 

};

template<std::size_t ... S>
struct gens<0, S ...> 
{
  typedef seq<S ...> type;
};

struct run
{
  template<class ... Args> 
  run(Args && ...)
  {  

  }
};
