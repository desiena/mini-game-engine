#include "EventManager.h"

using namespace eventSystem;


void EventManager::publish(Event event)
{
	std::vector<Subscription> interestedSubscriptions = subscriptions[event.type];
	for (Subscription sub : interestedSubscriptions)
	{
		sub.subscriber->handleEvent(event);
	}
}

void EventManager::subscribe(Listener* listener, std::string eventName)
{
	subscriptions[getEventType(eventName)].push_back({ listener });
}

EventArg eventSystem::Event::getArg(std::string argName)
{
	uint32_t argKey = getEventType(argName);
	for (EventArg arg : args)
	{
		if (arg.key == argKey) return arg;
	}
	return { argKey };
}
