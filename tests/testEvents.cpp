#include "pch.h"
#include "../mini_engine/EventManager.h"

using namespace eventSystem;


class TestListener : public Listener
{
public:
	std::vector<Event> eventTypes;

	// Inherited via Listener
	virtual void handleEvent(Event e) override
	{
		eventTypes.push_back(e);
	}
};


TEST(EventSystem, CallsListeners) {
	EventManager em;
	TestListener tl1;
	TestListener tl2;
	em.subscribe(&tl1, "testEvent");
	em.subscribe(&tl2, "testEvent");
	em.publish({ getEventType("testEvent") });
	EXPECT_EQ(tl1.eventTypes.size(), 1);
	EXPECT_EQ(tl2.eventTypes.size(), 1);
}

TEST(EventSystem, DoesntCallOthers) {
	EventManager em;
	TestListener tl1;
	TestListener tl2;
	em.subscribe(&tl1, "testEvent");
	em.subscribe(&tl2, "notGonnaHappen");
	em.publish({ getEventType("testEvent") });
	EXPECT_EQ(tl1.eventTypes.size(), 1);
	EXPECT_EQ(tl2.eventTypes.size(), 0);
}