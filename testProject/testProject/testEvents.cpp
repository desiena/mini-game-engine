#include "pch.h"
#include "../../EventManager.h"

using namespace eventSystem;


class TestListener : public Listener
{
public:
	std::vector<Event> handledEvents;

	// Inherited via Listener
	virtual void handleEvent(Event e) override
	{
		handledEvents.push_back(e);
	}
};


TEST(EventSystem, CallsListeners) {
	EventManager em;
	TestListener tl1;
	TestListener tl2;
	em.subscribe(&tl1, "testEvent");
	em.subscribe(&tl2, "testEvent");
	em.publish({ getEventType("testEvent") });
	EXPECT_EQ(tl1.handledEvents.size(), 1);
	EXPECT_EQ(tl2.handledEvents.size(), 1);
}

TEST(EventSystem, DoesntCallOthers) {
	EventManager em;
	TestListener tl1;
	TestListener tl2;
	em.subscribe(&tl1, "testEvent");
	em.subscribe(&tl2, "notGonnaHappen");
	em.publish({ getEventType("testEvent") });
	EXPECT_EQ(tl1.handledEvents.size(), 1);
	EXPECT_EQ(tl2.handledEvents.size(), 0);
}