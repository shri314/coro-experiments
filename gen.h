#pragma once

#include <experimental/coroutine>

#include "prom.h"

#include <iostream>

template <typename T>
struct gen
{
   gen() = delete;

   gen(const gen&) = delete;

   gen(gen&& g)
      : m_coro(std::move(g.m_coro))
   {
      std::cout << "gen<T>::gen(gen&&)" << std::endl;

      g.m_coro = nullptr;
   }

   ~gen()
   {
      std::cout << "gen<T>::~gen() { " << (not m_coro ? "m_coro is nullptr" : "m_coro is not nullptr... destroying") << " }" << std::endl;
      if (m_coro)
         m_coro.destroy();
   }

   using promise_type = prom< gen<T>, T, void >;

   struct iterator;

   auto begin()
   {
      return iterator{*this, false};
   }

   auto end()
   {
      return iterator{*this, true};
   }

private:

   friend promise_type;

   gen(std::experimental::coroutine_handle<promise_type> coro)
      : m_coro(coro)
   {
      std::cout << "gen<T>::gen(coro)" << std::endl;
   }

private:
   std::experimental::coroutine_handle<promise_type> m_coro;
};

///////////////////////////////////////////////////////////////////

template <typename T>
struct gen<T>::iterator
{
public:
   friend gen<T>;

   friend bool operator!=(const iterator& left, const iterator& right)
   {
      return left.finished != right.finished;
   }

   iterator& operator++()
   {
      std::cout << "gen<T>::iterator::operator++() { resuming..." << std::endl;

      owner.m_coro.resume();

      auto running = not owner.m_coro.done();

      std::cout << "gen<T>::iterator::operator++() } -> " << (running ? "suspended" : "done") << std::endl;

      finished = not running;

      return *this;
   }

   T operator*() const
   {
      return owner.m_coro.promise().get_yield_value();
   }

private:
   iterator(gen& owner, bool finished)
      : owner(owner)
      , finished(finished)
   {
   }

private:
   gen& owner;
   bool finished;
};
