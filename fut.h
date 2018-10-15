#pragma once

#include <experimental/coroutine>

#include "prom.h"

#include <iostream>

template <class T>
struct fut
{
   fut() = delete;

   fut(const fut&) = delete;

   fut(fut&& g)
      : m_coro(std::move(g.m_coro))
   {
      std::cout << "fut<T>::fut(fut&&)" << std::endl;

      g.m_coro = nullptr;
   }

   ~fut()
   {
      std::cout << "fut<T>::~fut() { " << (not m_coro ? "m_coro is nullptr" : "m_coro is not nullptr... destroying") << " }" << std::endl;
      if (m_coro)
         m_coro.destroy();
   }

   using promise_type = prom< fut<T>, void, T >;

   auto get()
   {
      if
         constexpr(std::is_same_v<T, void>)
         {
            std::cout << "fut<T>::get() -> void" << std::endl;
            return;
         }
      else
      {
         auto ret = m_coro.promise().get_value();
         std::cout << "fut<T>::get() -> T:'" << ret << "'" << std::endl;
         return ret;
      }
   }

private:
   friend promise_type;

   fut(std::experimental::coroutine_handle<promise_type> coro)
      : m_coro(coro)
   {
      std::cout << "fut<T>::fut(coro)" << std::endl;
   }

private:
   std::experimental::coroutine_handle<promise_type> m_coro;
};
