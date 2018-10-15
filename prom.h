#pragma once

#include <experimental/coroutine>

#include <iostream>

template <class SuspType>
static inline const char* str(SuspType)
{
   if
      constexpr(std::is_same_v<SuspType, std::experimental::suspend_never>)
      {
         return "suspend_never";
      }
   else if
      constexpr(std::is_same_v<SuspType, std::experimental::suspend_always>)
      {
         return "suspend_always";
      }
   else
   {
      return "suspend_?";
   }
}

//////////////////////////////////////////////

template<class ReturnType>
struct return_value_helper
{
   ReturnType m_ret_value;

public:
   void return_value(ReturnType value)
   {
      std::cout << "prom::return_value(" << value << ")" << std::endl;

      m_ret_value = value;
   }

   auto get_value() {
      return m_ret_value;
   }
};

template<>
struct return_value_helper<void>
{
public:
   void return_void()
   {
      std::cout << "prom::return_void()" << std::endl;
   }
};

//////////////////////////////////////////////

template<class YieldType>
struct yield_value_helper
{
   YieldType m_current_value;

public:
   auto get_yield_value() {
      return m_current_value;
   }

   auto yield_value(YieldType value)
   {
      auto x = std::experimental::suspend_always{};

      std::cout << "prom::yield_value(" << value << ") -> " << str(x) << std::endl;

      m_current_value = value;

      return x;
   }
};

template<>
struct yield_value_helper<void>
{
};

//////////////////////////////////////////////

template <typename ReturnObject, typename YieldType = void, class ReturnType = void, class ExceptionType = void>
struct prom : yield_value_helper<YieldType>, return_value_helper<ReturnType>
{
   prom()
   {
      std::cout << "prom::prom()" << std::endl;
   }

   prom(const prom&) = delete;

   prom(prom&&)      = default;

   ~prom()
   {
      std::cout << "prom::~prom()" << std::endl;
   }

   auto get_return_object()
   {
      std::cout << "prom::get_return_object() -> ReturnObject{coro}" << std::endl;

      auto coro = std::experimental::coroutine_handle<prom>::from_promise(*this);

      return ReturnObject{coro};
   }

   ////////////////////////

   auto initial_suspend()
   {
      auto x = std::experimental::suspend_never{};

      std::cout << "prom::initial_suspend() -> " << str(x) << std::endl;

      return x;
   }

   auto final_suspend()
   {
      auto x = std::experimental::suspend_never{};

      std::cout << "prom::final_suspend() -> " << str(x) << std::endl;

      return x;
   }

   ////////////////////////

   void unhandled_exception()
   {
      std::cout << "prom::unhandled_exception() -> blast" << std::endl;
      std::terminate();
   }
};
