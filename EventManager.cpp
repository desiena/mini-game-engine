#include "EventManager.h"

using namespace eventSystem;

uint32_t eventSystem::getEventType(std::string typeName)
{
	return std::hash<std::string>{}(typeName);
}

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
