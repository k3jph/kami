/*-
 * TODO FILEHEADER
 */

#include "boltzmann3d.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <stdlib.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <iostream>
#include <kami/agent.hpp>
#include <kami/config.hpp>
#include <kami/grid2d.hpp>
#include <kami/kami.hpp>
#include <kami/multigrid2d.hpp>
#include <map>

using namespace kami;
using namespace std;

kami::MultiGrid3D *MoneyAgent::world = nullptr;
BoltzmannWealthModel *MoneyAgent::model = nullptr;
shared_ptr<spdlog::logger> console;

template <>
struct fmt::formatter<AgentID> : fmt::formatter<string> {
    auto format(AgentID agentID, format_context &ctx) {
        return format_to(ctx.out(), "{}", agentID.toString());
    }
};

template <>
struct fmt::formatter<GridCoord3D> : fmt::formatter<string> {
    auto format(GridCoord3D coord, format_context &ctx) {
        return format_to(ctx.out(), "{}", coord.toString());
    }
};

MoneyAgent::MoneyAgent() {
    stepCounter = 0;
    agentWealth = 1;
}

void MoneyAgent::step() {
    stepCounter++;

    moveAgent();
    if (agentWealth > 0)
        giveMoney();
}

void MoneyAgent::setWorld(kami::MultiGrid3D *w) {
    world = w;
}

void MoneyAgent::setModel(class BoltzmannWealthModel *m) {
    model = m;
}

void MoneyAgent::moveAgent() {
    auto agentID = this->getAgentID();
    auto moveList = world->getNeighborhood(agentID, kami::GridNeighborhoodType::Moore, false);
    auto newLocation = moveList[static_cast<unsigned int>(rand()) % moveList.size()];

    console->trace("Moving Agent {} to location {}", agentID, newLocation);
    world->moveAgent(agentID, newLocation);
}

int MoneyAgent::getWealth() {
    return agentWealth;
}

void MoneyAgent::setWealth(int newWealth) {
    agentWealth = newWealth;
}

void MoneyAgent::giveMoney() {
    kami::AgentID agentID = getAgentID();
    kami::GridCoord3D location = world->getLocationByAgent(agentID);
    std::vector<kami::AgentID> *cellMates = world->getCellContents(location);

    if (cellMates->size() > 1) {
        kami::AgentID otherAgentID = cellMates->at(static_cast<unsigned int>(rand()) % cellMates->size());
        auto otherAgent = model->getAgentByID(otherAgentID);

        console->trace("Agent {} giving unit of wealth to agent {}", agentID, otherAgentID);
        otherAgent->agentWealth += 1;
        agentWealth -= 1;
    }
}

void MoneyAgent::prinfo(void) const {
    kami::AgentID agentID = getAgentID();
    kami::GridCoord3D location = world->getLocationByAgent(agentID);

    console->trace("Agent state for agent {}, step {}, wealth {}, location {}", agentID, stepCounter, agentWealth, location);
}

BoltzmannWealthModel::BoltzmannWealthModel(unsigned int numberAgents, unsigned int lengthX, unsigned int lengthY, unsigned int lengthZ) {
    world = new kami::MultiGrid3D(lengthX, lengthY, lengthZ, true, true, true);
    sched = new kami::RandomScheduler(this);

    stepCount = 0;
    MoneyAgent::setWorld(world);
    MoneyAgent::setModel(this);

    for (unsigned int i = 0; i < numberAgents; i++) {
        MoneyAgent *newAgent = new MoneyAgent();

        agentList.insert(std::pair<kami::AgentID, MoneyAgent *>(newAgent->getAgentID(), newAgent));
        sched->addAgent(newAgent->getAgentID());
        world->addAgent(newAgent->getAgentID(),
                        kami::GridCoord3D(rand() % static_cast<int>(lengthX),
                                          rand() % static_cast<int>(lengthY),
                                          rand() % static_cast<int>(lengthZ)));
    }
}

BoltzmannWealthModel::~BoltzmannWealthModel() {
    for (auto agentPair = agentList.begin(); agentPair != agentList.end(); agentPair++) {
        delete agentPair->second;
    }

    delete sched;
    delete world;
}

void BoltzmannWealthModel::step() {
    stepCount++;
    sched->step();
}

void BoltzmannWealthModel::run(unsigned int n) {
    for (unsigned int i = 0; i < n; i++)
        step();
}

void BoltzmannWealthModel::prinfo(void) const {
    console->trace("Model state step {}", stepCount);
    for (auto agent = agentList.begin(); agent != agentList.end(); ++agent) {
        agent->second->prinfo();
    }
}

MoneyAgent *BoltzmannWealthModel::getAgentByID(kami::AgentID agentID) const {
    MoneyAgent *agentPair = agentList.at(agentID);

    return agentPair;
}

int main(int argc, char **argv) {
    string ident = "boltzmann3d";
    CLI::App app{ident};
    string logLevelOption = "info";
    unsigned int xSize = 8, ySize = 8, zSize = 8,
                 agentCount = xSize * ySize * zSize, maxSteps = 100;

    app.add_option("-c", agentCount, "Set the number of agents")->check(CLI::PositiveNumber);
    app.add_option("-l", logLevelOption, "Set the logging level")->check(CLI::IsMember(SPDLOG_LEVEL_NAMES));
    app.add_option("-n", maxSteps, "Set the number of steps to run the model")->check(CLI::PositiveNumber);
    app.add_option("-x", xSize, "Set the number of colums")->check(CLI::PositiveNumber);
    app.add_option("-y", ySize, "Set the number of rows")->check(CLI::PositiveNumber);
    app.add_option("-z", ySize, "Set the depth of the grid")->check(CLI::PositiveNumber);
    CLI11_PARSE(app, argc, argv);

    console = spdlog::stdout_color_st(ident);
    console->set_level(spdlog::level::from_str(logLevelOption));
    console->info("Compiled with Kami/{}.{}.{}, log level {}", KAMI_VERSION_MAJOR, KAMI_VERSION_MINOR, KAMI_VERSION_PATCH, logLevelOption);
    console->info("Starting Boltzmann Wealth Model with {} agents on a {}x{}x{}-unit grid for {} steps", agentCount, xSize, ySize, zSize, maxSteps);

    BoltzmannWealthModel model(agentCount, xSize, ySize, zSize);

    spdlog::stopwatch sw;
    for (int i = 0; i < maxSteps; i++) {
        console->trace("Initiating model step {}", i);
        model.step();
    }
    console->info("Boltzman Wealth Model simulation complete, requiring {} ", sw);

    model.prinfo();
}
