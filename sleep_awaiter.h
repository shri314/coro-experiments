#pragma once

#include <experimental/coroutine>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

#include <iostream>

struct sleep_awaiter
{
   sleep_awaiter(boost::asio::io_context& io, int x, int calc)
      : m_timer(io, boost::posix_time::seconds(x))
      , m_calc(calc)
   {
      std::cout << "sleep_awaiter::sleep_awaiter(" << x << ")" << std::endl;
   }

   ~sleep_awaiter()
   {
      std::cout << "sleep_awaiter::~sleep_awaiter()" << std::endl;
   }

   sleep_awaiter(const sleep_awaiter&) = delete;
   sleep_awaiter(sleep_awaiter&&)      = delete;

   bool await_ready() const
   {
      bool x = false;

      std::cout << "sleep_awaiter::await_ready() -> " << x << std::endl;

      return x;
   }

   void await_suspend(std::experimental::coroutine_handle<> coro)
   {
      m_timer.async_wait([coro, this](auto ec) mutable {

         std::cout << "sleep_awaiter::await_suspend::timer_callback - resuming..." << std::endl;

         coro.resume();

         bool d = coro.done();

         m_calc = 50;

         std::cout << "sleep_awaiter::await_suspend::timer_callback } finished:" << d << std::endl;
      });

      std::cout << "sleep_awaiter::await_suspend(coro) -> { scheduled m_timer callback and suspending..." << std::endl;
   }

   int await_resume()
   {
      int x = m_calc;

      std::cout << "sleep_awaiter::await_resume() -> " << x << std::endl;

      return x;
   }

private:
   boost::asio::deadline_timer m_timer;
   int m_calc;
};
