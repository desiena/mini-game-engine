#pragma once

#include <cstdint>
#include <array>
#include <variant>
#include <unordered_map>

namespace eventSystem
{
	constexpr size_t val_const{ 0xcbf29ce484222325 };
	constexpr size_t prime_const{ 0x100000001b3 };

	// From https://simoncoenen.com/blog/programming/StaticReflection
	constexpr uint32_t Hash(const char* const str, const size_t value) noexcept
	{
		return (str[0] == '\0') ? value : Hash(&str[1], (value ^ size_t(str[0])) * prime_const);
	}

	constexpr uint32_t getEventType(std::string eventTypeName) {
		uint32_t hashedType = Hash(eventTypeName.c_str(), eventTypeName.length());
		return hashedType;
	};

	struct EventArg
	{
		// Key is a hashed string id.
		uint32_t key;
		std::variant<bool, uint32_t, void*, float> value;
	};

	struct Event
	{
		// Type is a hashed string id.
		uint32_t type;
		// args is a fixed size array to keep open the possibility of an event pool allocator.
		std::array<EventArg, 5> args;

		EventArg getArg(std::string argName);
	};

	class Listener
	{
	public:
		virtual void handleEvent(Event) = 0;
	};

	struct Subscription
	{
		Listener* subscriber;
	};

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

}
