#pragma once

#include <cstdint>
#include <array>
#include <variant>
#include <unordered_map>

namespace eventSystem
{

	// ToDo: Stricter event type management.
	class EventManager
	{
	public:
		void publish(Event event);
		void subscribe(Listener* subscriber, std::string eventType);
	private:
		std::unordered_map<uint32_t, std::vector<Subscription>> subscriptions;
		// ToDo: add a map that allows for the original string to be retrieved using a hash.
	};

	struct Event
	{
		// Type is a hashed string id.
		uint32_t type;
		// args is a fixed size array to keep open the possibility of an event pool allocator.
		std::array<EventArg, 5> args;
	};

	struct EventArg
	{
		// Name is a hashed string id.
		uint32_t key;
		std::variant<bool, uint64_t> value;
	};

	struct Subscription
	{
		Listener* subscriber;
	};

	class Listener
	{
	public:
		virtual void handleEvent(Event) = 0;
	};

}
