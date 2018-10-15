// lazyGenerator.cpp

#include "fut.h"
#include "sleep_awaiter.h"

#include <boost/asio.hpp>

#include <iostream>

fut<int> do_slow_operation(boost::asio::io_context& io, int x)
{
   /*
    * __context = new coro_context();
    *
    * __return = __context->promise().get_return_object();
    *
    * co_await __context->promise().initial_suspend();
    */

   std::cout << "--corofun begin" << std::endl;

   sleep_awaiter w{io, x, 5000 + x};

   std::cout << "--corofun before co_await on sleep_awaiter" << std::endl;

   /*
    * __awaiter = __context->promise().await_transform(w);
    *  -OR-
    * __awaiter = operator co_await(w);
    *  -OR-
    * __awaiter = w;
    *
    *
    * if( !__awaiter.await_ready() ) {
    *    __awaiter.await_suspend( std::experimental::coroutine_handle<prom>::from_promise( __context->promise() ) );
    * }
    *
    * <suspend>
    *
    * int z = __awaiter.await_resume();
    *  -OR-
    *         __awaiter.await_resume();
    */

   int z = co_await w;

   std::cout << "--corofun before co_return" << std::endl;

   /*
    * __context->promise().return_void();
    *  -OR-
    * __context->promise().return_value("abc");
    *
    * goto function_exit;
    */

   co_return z;

   /*
    * function_exit:
    *   co_await __context->promise().final_suspend();
    *   delete __context;
    */
}


int main()
{
   boost::asio::io_context io;

   std::cout << "-----------------\n";

   {
      auto&& f1 = do_slow_operation(io, 2);
      auto&& f2 = do_slow_operation(io, 5);

      io.run();

      f1.get();
      f2.get();
   }

   std::cout << "-----------------\n";
}
