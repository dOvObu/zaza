#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED
#include <map>
#include <vector>
#include <functional>


/*
  class mouse {
    zevent(click);
  };
  
  class hero {
    cevent(walk, (float x, float y), (x, y));
  };
  
  class penguin {
  public:
    feed() {} // om-nom-nom
      
    ievent(come, penguin);
  }

// ...

  mouse m;
  m.subscribe_on_click([](){
     // tra-ta-ta
  });
  m.click(); // tra-ta-ta

// ...  

  penguin p;
  p.subscribe_on_come([](penguin& pn)) {
    pn.feed();
  });
  p.come(); // om-nom-nom
  
// ...  
  
  hero h;
  h.subscribe_on_walk([](float x, float y){
     // walking-walking-walking
  });
  p.walk(x, y); // walking-walking-walking
*/




#define zevent(NAME)\
public: \
   void NAME(){for(auto& it : _pool_of_ ## NAME ## _cbs)it(); for(auto& it : __pool_of_ ## NAME ## _cbs)it.second();} \
   void subscribe_on_ ## NAME(std::function<void(void)> const& foo) { _pool_of_ ## NAME ## _cbs.push_back(foo); } \
   void subscribe_on_ ## NAME(void* key, std::function<void(void)> const& foo) { __pool_of_ ## NAME ## _cbs.insert({key, foo}); } \
   void unsubscribe_from_ ## NAME(void* key, std::function<void(void)> const& foo) { __pool_of_ ## NAME ## _cbs.erase(key); } \
private: \
   std::vector<std::function<void(void)>> _pool_of_ ## NAME ## _cbs; \
   std::map<void*,std::function<void(void)>> __pool_of_ ## NAME ## _cbs


#define ievent(NAME,CB_ARGS, CALL_ARGS)\
public: \
   void subscribe_on_ ## NAME(std::function<void##CB_ARGS> const& foo) { _pool_of_ ## NAME ## _cbs.push_back(foo); } \
   void subscribe_on_ ## NAME(void* key, std::function<void##CB_ARGS> const& foo) { __pool_of_ ## NAME ## _cbs.insert({key, foo}); } \
   void unsubscribe_from_ ## NAME(void* key, std::function<void##CB_ARGS> const& foo) { __pool_of_ ## NAME ## _cbs.erase(key); } \
   void NAME ## CB_ARGS {for(auto& it : _pool_of_ ## NAME ## _cbs)it ## CALL_ARGS; for(auto& it : __pool_of_ ## NAME ## _cbs)it.second ## CALL_ARGS;} \
private: \
   std::vector<std::function<void##CB_ARGS>> _pool_of_ ## NAME ## _cbs; \
   std::map<void*,std::function<void##CB_ARGS>> __pool_of_ ## NAME ## _cbs


#endif // !EVENTS_HPP_INCLUDED
