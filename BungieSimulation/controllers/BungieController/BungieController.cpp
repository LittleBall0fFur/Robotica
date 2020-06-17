// File:          bungie_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <RobotController/RobotController.hpp>
#include <webots/Keyboard.hpp>
#include "MachineIntelligence/MeasureWeightController.h"
#include "MachineIntelligence/Strategies/TransportRockStrategy.h"
#include "MachineIntelligence/NSA.h"
#include "TCPServer/TCPServer.hpp" 
#include "TCPServer/IObserver.hpp" 
#include "TCPServer/ISubject.hpp" 
#include <thread>
#include <future>
#include <chrono>
// All the webots classes are defined in the "webots" namespace
using namespace webots;
using namespace bungie;
using namespace std::chrono_literals;
// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node




int main(int argc, char **argv) {
  // get the time step of the current world.
  int timeStep = (int)RobotController::getInstance().getRobot().getBasicTimeStep();
  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);
  Keyboard keyboard = Keyboard();
  keyboard.enable(32);
  std::thread serverThread;
  std::cout << "Hello, world!" << std::endl;
  bool merged = false;
   //std::promise<bool> p;
  //auto future = p.get_future();
  //std::thread first (TCPServer::getInstance().launch);
  TCPServerObserver observerA = TCPServerObserver();
  //TCPServer::getInstance().registObserver(observerA);
  TCPServer * server = &TCPServer::getInstance();
 // TCPServer::getInstance().launch();
 //serverThread = std::async([&p] p.set_value(true); // Is done atomically.
 //   );
 auto future = std::async(std::launch::async, &TCPServer::launch, server);


  ;
  

  TCPServer::getInstance().registObserver(observerA);
  //std::thread second (TCPServer::getInstance().registObserver(observerA));
    NSA nsa = NSA();
    MeasureWeightController weightController = MeasureWeightController();
    TransportRockStrategy strategy = TransportRockStrategy(weightController);
    //MoonSurvivalStrategy strategy = MoonSurvivalStrategy();
    nsa.ExecuteAssignment(strategy);

  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  while (RobotController::getInstance().getRobot().step(timeStep) != -1) {
    if (merged == false){
     auto status = future.wait_for(0ms);
     if (status == std::future_status::ready) {
        future.get();
        merged = true;
      }
    }
    else
    TCPServer::getInstance().run();
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();
    int pressed_key = keyboard.getKey();
    switch(pressed_key){
    case Keyboard::UP:
      RobotController::getInstance().Drive('f', 1.5);
      break;
    case Keyboard::DOWN:
      RobotController::getInstance().Drive('b', 1.5);
      break;
    case Keyboard::LEFT:
      RobotController::getInstance().Drive('l', 1.5);
      break;
    case Keyboard::RIGHT:
      RobotController::getInstance().Drive('r', 1.5);
      break;
    default:
      RobotController::getInstance().Drive('f', 0.0);
    }
  
    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
  };

  // Enter here exit cleanup code.
  
  return 0;
}
