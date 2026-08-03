/*
 **********************************************************************************************************************
 *
 * draupnir-lib
 * Copyright (C) 2026 Ivan Odinets <i_odinets@protonmail.com>
 *
 * This file is part of draupnir-lib
 *
 * draupnir-lib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * draupnir-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with draupnir-lib; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef CONCURRENTTESTHELPERS_H
#define CONCURRENTTESTHELPERS_H

#include <QtConcurrent>

/*! @class ConcurrentTestHelpers draupnir-test/helpers/ConcurrentTestHelpers.h
 *  @ingroup TestHelpers
 *  @brief Provides helper functions for concurrent and multithreaded tests.
 *
 *  @details Contains utilities for repeatedly invoking test callables from multiple worker threads, primarily for stress-testing
 *           thread-safety and synchronization behavior. */

class ConcurrentTestHelpers
{
public:
    ConcurrentTestHelpers() = delete;

    /*! @brief Executes a callable concurrently from multiple worker threads.
     *  @param threadCount Number of worker threads to start.
     *  @param callsPerThread Number of callable invocations performed by each thread.
     *  @param callable Callable object invoked by every worker thread.
     *  @details The function blocks until all worker threads have finished. */
    template<std::invocable F>
    static void performSpamCalls(const unsigned int threadCount, const unsigned int callCount, const F& callable) {
        QList<QFuture<void>> futureList;
        futureList.reserve(threadCount);

        // Start threads
        for (unsigned int i = 0; i < threadCount; i++) {
            auto future = QtConcurrent::run([&callable,callCount](){
                for (int j = 0; j < callCount; j++)
                    std::invoke(callable);
            });
            futureList.append(std::move(future));
        }

        // Wait for threads to be finished
        for (auto& future : futureList)
            future.waitForFinished();
    }
};

#endif // CONCURRENTTESTHELPERS_H
