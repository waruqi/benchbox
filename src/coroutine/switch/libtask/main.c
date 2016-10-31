/*!The Benchmark Testing Box 
 * 
 * Benchbox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * Benchbox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with Benchbox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2016-2020, ruki All rights reserved.
 *
 * @author      ruki
 * @file        main.c
 *
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "tbox/tbox.h"
#include "task.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the task stack size
#define STACK       (32768)

// the switch count
#define COUNT       (10000000)

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the start time
static tb_hong_t g_startime = 0;

// the coroutine count
static tb_size_t g_cocount = 0;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementaiton
 */
static tb_void_t switchtask(tb_pointer_t priv)
{
    // loop
    tb_size_t count = (tb_size_t)priv;
    while (count--) taskyield();

    // computing time
    tb_hong_t duration = tb_mclock() - g_startime;

    // trace
    tb_trace_i("switch[%lu]: libtask: %d switches in %lld ms, %lld switches per second", g_cocount, COUNT, duration, (((tb_hong_t)1000 * COUNT) / duration));

    // exit all tasks
    taskexitall(0);

    // exit tbox
    tb_exit();
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_void_t taskmain(tb_int_t argc, tb_char_t** argv)
{
    // init tbox
    if (!tb_init(tb_null, tb_null)) return ;

    // get coroutine count
    g_cocount = argv[1]? tb_atoi(argv[1]) : 2;
    tb_assert_and_check_return(g_cocount > 1);

    // create task
    tb_size_t i = 0;
    for (i = 0; i < g_cocount; i++)
        taskcreate(switchtask, (tb_pointer_t)(COUNT / g_cocount), STACK);

    // init start time
    g_startime = tb_mclock();
}
