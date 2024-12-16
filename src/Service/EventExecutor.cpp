
#include <iostream>
#include "Service/EventExecutor.hpp"
#include "WorkerModule/Events.hpp"

void service::EventExecutor::operator()(const std::shared_ptr<worker_utility::NetworkEvent> event) const 
{
    std::cout<< "Executing source_id: " << event->source_id << ", message: " << event->message<<"...\n";
}
