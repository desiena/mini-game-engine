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

void EventManager::subscribe(Listener* listener, std::string eventType)
{
	uint32_t typeHash = std::hash<std::string>{}(eventType);
	std::vector<Subscription> eventSubscriptions = subscriptions[typeHash];
	eventSubscriptions.push_back({listener});
}
