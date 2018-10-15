// lazyGenerator.cpp

#include "gen.h"

#include <iostream>

gen<int> make_gen(int begin, int end, int inc = 1)
{
   /*
    * __context = new coro_context();
    *
    * __return = __context->promise().get_return_object();
    *
    * co_await __context->promise().initial_suspend();
    */

   std::cout << "--corofun begin" << std::endl;

   for (int i = begin; i < end; i += inc)
   {
      std::cout << "--corofun before co_yield - " << i << std::endl;

      /*
       * co_await __context->promise().yield_value(i);
       */

      co_yield i;

      std::cout << "--corofun after co_yield - " << i << std::endl;
   }

   std::cout << "--corofun before co_return" << std::endl;

   /*
    * __context->promise().return_void();
    *  -OR-
    * __context->promise().return_value("abc");
    *
    * goto function_exit;
    */

   co_return;

   /*
    * function_exit:
    *   co_await __context->promise().final_suspend();
    *   delete __context;
    */
}


int main()
{
   std::cout << "-----------------\n";

   for (auto n : make_gen(0, 5))
   {
      std::cout << "[" << n << "]" << std::endl;
   }

   std::cout << "-----------------\n";
}
