/*-
 * Copyright (c) 2020 The Johns Hopkins University Applied Physics
 * Laboratory LLC
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#ifndef KAMI_RANDOM_H
#define KAMI_RANDOM_H

#include <algorithm>
#include <kami/KAMI_EXPORT.h>
#include <kami/kami.h>
#include <random>
#include <vector>

namespace kami {

///  \brief     Will execute all agent steps in a random order.
///  \details   A random scheduler will iterate over the agents assigned
///  to the scheduler and call their `step()` function in a random order.
///  That order should be different for each subsequent call to `step()`,
///  but is not gauranteed not to repeat.
///  \pre       First create a Model for the scheduler to live in.
class LIBKAMI_EXPORT RandomScheduler : public Scheduler {
   public:
    ///  \brief Constructor.
    ///  \details   The Model parameter is used by the scheduler to get
    ///  access to an Agent.  The Model is presumed to maintain a master
    ///  list of all Agents in the Model and the Model can be queried for
    ///  a reference to any particular Agent at `step()` time.
    RandomScheduler(Model *model);

    ///  \brief Constructor.
    ///  \details   The Model parameter is used by the scheduler to get
    ///  access to an Agent.  The Model is presumed to maintain a master
    ///  list of all Agents in the Model and the Model can be queried for
    ///  a reference to any particular Agent at `step()` time.
    RandomScheduler(Model *model, uint_fast32_t seed);

    ///  A deconstructor.
    virtual ~RandomScheduler();

    ///  \brief Add an agent to the scheduler.
    ///  \details   The scheduler maintains a list of all AgentIDs currently
    ///  assigned.  This function adds a new Agent to the list.
    void addAgent(AgentID aid);

    ///  \brief Remove an agent from the scheduler.
    ///  \details The scheduler maintains a list of all AgentIDs currently
    ///  assigned.  This function removes an Agent from the list.
    void deleteAgent(AgentID aid);

    ///  \brief Execute a single time step.
    ///  \details   This method will randomize the list of Agents in the
    ///  scheduler's internal queue and then execute the `Agent::step()`
    ///  method for every Agent assigned to this scheduler in the
    ///  randomized order.
    void step();

   protected:
    std::vector<AgentID> agentList;
    std::random_device rd;
    std::mt19937 rng{rd()};
    Model *model;
    int stepCounter;
};

class LIBKAMI_EXPORT TwoActionRandomScheduler : public RandomScheduler {
   public:
    ///  \brief Execute a single time step.
    ///  \details   This method will randomize the list of TwoActionAgents in the
    ///  scheduler's internal queue and then execute the `TwoActionAgent::stepA()`
    ///  method for every TwoActionAgent assigned to this scheduler in the
    ///  randomized order, then execute the `TwoActionAgent::stepB()` method for every
    //   TwoActionAgent assigned to this scheduler in the randomized order,
    void step();
};

};  // namespace kami

#endif  // KAMI_RANDOM_H
