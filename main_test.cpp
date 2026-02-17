// =============================================================================
// Dragonfly Game Engine - Test Suite
// Tests all graded components: Manager, GameManager, LogManager, WorldManager,
// Object, ObjectList, Clock, Vector, Event, EventStep
// =============================================================================

#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
#include <thread>
#include <chrono>

#include "LogManager.h"
#include "GameManager.h"
#include "WorldManager.h"
#include "DisplayManager.h"
#include "Clock.h"
#include "Vector.h"
#include "Object.h"
#include "ObjectList.h"
#include "Event.h"
#include "EventStep.h"
#include "EventOut.h"
#include "EventCollision.h"

// -----------------------------------------------------------------------
// Test helpers
// -----------------------------------------------------------------------
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cout << "  FAIL: " << msg << "\n"; \
            tests_failed++; \
        } else { \
            std::cout << "  PASS: " << msg << "\n"; \
            tests_passed++; \
        } \
    } while (0)

#define ASSERT_EQ(a, b, msg) ASSERT_TRUE((a) == (b), msg)
#define ASSERT_NEAR(a, b, eps, msg) ASSERT_TRUE(std::fabs((a)-(b)) < (eps), msg)

// -----------------------------------------------------------------------
// Test Object subclass that counts events received
// -----------------------------------------------------------------------
class TestObject : public df::Object {
public:
    int step_count       = 0;
    int collision_count  = 0;
    int out_count        = 0;
    std::string last_event;

    TestObject() { setType("TestObject"); }

    int eventHandler(const df::Event *p_e) override {
        last_event = p_e->getType();
        if (p_e->getType() == STEP_EVENT)     { step_count++;      return 1; }
        if (p_e->getType() == COLLISION_EVENT){ collision_count++; return 1; }
        if (p_e->getType() == OUT_EVENT)      { out_count++;       return 1; }
        return 0;
    }
};

// -----------------------------------------------------------------------
// VECTOR TESTS
// -----------------------------------------------------------------------
void testVector() {
    std::cout << "\n--- Vector Tests ---\n";

    df::Vector v1;
    ASSERT_EQ(v1.getX(), 0.0f, "Default constructor x=0");
    ASSERT_EQ(v1.getY(), 0.0f, "Default constructor y=0");

    df::Vector v2(3.0f, 4.0f);
    ASSERT_EQ(v2.getX(), 3.0f, "Constructor sets x");
    ASSERT_EQ(v2.getY(), 4.0f, "Constructor sets y");

    ASSERT_NEAR(v2.getMagnitude(), 5.0f, 0.001f, "Magnitude of (3,4) = 5");

    df::Vector v3 = v2;
    v3.normalize();
    ASSERT_NEAR(v3.getMagnitude(), 1.0f, 0.001f, "Normalize produces unit vector");

    df::Vector v4(1.0f, 0.0f);
    v4.scale(7.0f);
    ASSERT_EQ(v4.getX(), 7.0f, "Scale x component");
    ASSERT_EQ(v4.getY(), 0.0f, "Scale y component");

    df::Vector va(1.0f, 2.0f);
    df::Vector vb(3.0f, 4.0f);
    df::Vector sum = va + vb;
    ASSERT_EQ(sum.getX(), 4.0f, "Addition x");
    ASSERT_EQ(sum.getY(), 6.0f, "Addition y");

    df::Vector diff = vb - va;
    ASSERT_EQ(diff.getX(), 2.0f, "Subtraction x");
    ASSERT_EQ(diff.getY(), 2.0f, "Subtraction y");

    df::Vector v5(1.0f, 2.0f);
    df::Vector v6(1.0f, 2.0f);
    ASSERT_TRUE(v5 == v6, "Equality operator");
    v6.setX(9.0f);
    ASSERT_TRUE(v5 != v6, "Inequality operator");

    // Zero-magnitude normalize (should not crash)
    df::Vector zero;
    zero.normalize();
    ASSERT_EQ(zero.getMagnitude(), 0.0f, "Normalize zero vector - no crash");

    LM.writeLog("Vector tests complete.");
}

// -----------------------------------------------------------------------
// CLOCK TESTS
// -----------------------------------------------------------------------
void testClock() {
    std::cout << "\n--- Clock Tests ---\n";

    df::Clock c;

    // delta() resets timer
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    long int d = c.delta();
    ASSERT_TRUE(d >= 40000, "delta() returns ~50ms (>=40ms)");
    ASSERT_TRUE(d < 200000, "delta() not excessively long (<200ms)");

    // split() does NOT reset timer
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    long int s1 = c.split();
    ASSERT_TRUE(s1 >= 25000, "split() returns ~30ms (>=25ms)");

    // Second split should be close to first (timer not reset)
    long int s2 = c.split();
    ASSERT_TRUE(s2 >= s1, "Second split() >= first (timer not reset)");
    ASSERT_TRUE(s2 - s1 < 5000, "split() does not reset timer (within 5ms)");

    // After delta, split resets
    c.delta();
    long int s3 = c.split();
    ASSERT_TRUE(s3 < 10000, "split() after delta() is near zero (<10ms)");

    LM.writeLog("Clock tests complete.");
}

// -----------------------------------------------------------------------
// OBJECTLIST TESTS
// -----------------------------------------------------------------------
void testObjectList() {
    std::cout << "\n--- ObjectList Tests ---\n";

    df::ObjectList list;
    ASSERT_TRUE(list.isEmpty(), "New list is empty");
    ASSERT_TRUE(!list.isFull(), "New list is not full");
    ASSERT_EQ(list.getCount(), 0, "New list count = 0");

    // We create objects that auto-add to WorldManager, so test with raw ptrs
    // by temporarily inserting a dummy address (abuse for unit test purposes)
    df::Object *dummy1 = reinterpret_cast<df::Object *>(0x1001);
    df::Object *dummy2 = reinterpret_cast<df::Object *>(0x1002);
    df::Object *dummy3 = reinterpret_cast<df::Object *>(0x1003);

    df::ObjectList raw;
    raw.insert(dummy1);
    ASSERT_EQ(raw.getCount(), 1, "Count after 1 insert");
    raw.insert(dummy2);
    raw.insert(dummy3);
    ASSERT_EQ(raw.getCount(), 3, "Count after 3 inserts");

    ASSERT_EQ(raw[0], dummy1, "Index [0] correct");
    ASSERT_EQ(raw[1], dummy2, "Index [1] correct");

    raw.remove(dummy2);
    ASSERT_EQ(raw.getCount(), 2, "Count after remove");
    ASSERT_EQ(raw[0], dummy1, "After remove: [0] still dummy1");
    ASSERT_EQ(raw[1], dummy3, "After remove: [1] is dummy3 (shifted)");

    raw.clear();
    ASSERT_EQ(raw.getCount(), 0, "Count after clear");
    ASSERT_TRUE(raw.isEmpty(), "isEmpty after clear");

    // Out-of-bounds index returns nullptr
    ASSERT_TRUE(raw[-1] == nullptr, "Negative index returns nullptr");
    ASSERT_TRUE(raw[0] == nullptr, "Out-of-range index on empty list returns nullptr");

    // Remove non-existent returns -1
    ASSERT_EQ(raw.remove(dummy1), -1, "Remove nonexistent returns -1");

    LM.writeLog("ObjectList tests complete.");
}

// -----------------------------------------------------------------------
// EVENT TESTS
// -----------------------------------------------------------------------
void testEvents() {
    std::cout << "\n--- Event Tests ---\n";

    // Base Event
    df::Event e;
    ASSERT_EQ(e.getType(), UNDEFINED_EVENT, "Base Event default type");
    e.setType("my_event");
    ASSERT_EQ(e.getType(), std::string("my_event"), "setType / getType");

    // EventStep
    df::EventStep es;
    ASSERT_EQ(es.getType(), STEP_EVENT, "EventStep type");
    ASSERT_EQ(es.getStepCount(), 0, "EventStep default step count");
    es.setStepCount(42);
    ASSERT_EQ(es.getStepCount(), 42, "EventStep setStepCount");

    df::EventStep es2(99);
    ASSERT_EQ(es2.getStepCount(), 99, "EventStep constructor with count");

    // EventOut
    df::EventOut eo;
    ASSERT_EQ(eo.getType(), OUT_EVENT, "EventOut type");

    // EventCollision
    df::EventCollision ec;
    ASSERT_EQ(ec.getType(), COLLISION_EVENT, "EventCollision default type");
    ASSERT_TRUE(ec.getObject1() == nullptr, "EventCollision default obj1 nullptr");
    ASSERT_TRUE(ec.getObject2() == nullptr, "EventCollision default obj2 nullptr");

    LM.writeLog("Event tests complete.");
}

// -----------------------------------------------------------------------
// OBJECT TESTS
// -----------------------------------------------------------------------
void testObject() {
    std::cout << "\n--- Object Tests ---\n";

    // Create object (auto-adds to WorldManager)
    df::Object *p_o = new df::Object();

    // ID should be positive
    ASSERT_TRUE(p_o->getId() > 0, "Object gets a positive ID");

    // Type
    p_o->setType("Spaceship");
    ASSERT_EQ(p_o->getType(), std::string("Spaceship"), "Object setType/getType");

    // Position
    df::Vector pos(5.0f, 10.0f);
    p_o->setPosition(pos);
    ASSERT_EQ(p_o->getPosition().getX(), 5.0f, "Object position x");
    ASSERT_EQ(p_o->getPosition().getY(), 10.0f, "Object position y");

    // Altitude
    ASSERT_EQ(p_o->setAltitude(2), 0, "setAltitude(2) returns 0");
    ASSERT_EQ(p_o->getAltitude(), 2, "getAltitude() = 2");
    ASSERT_EQ(p_o->setAltitude(-1), -1, "setAltitude(-1) returns -1");
    ASSERT_EQ(p_o->setAltitude(MAX_ALTITUDE + 1), -1, "setAltitude(MAX+1) returns -1");

    // Solidness
    ASSERT_TRUE(p_o->isSolid(), "Default Object is solid (HARD)");
    ASSERT_EQ(p_o->setSolidness(df::SOFT), 0, "setSolidness(SOFT) returns 0");
    ASSERT_TRUE(p_o->isSolid(), "SOFT object is solid");
    ASSERT_EQ(p_o->setSolidness(df::SPECTRAL), 0, "setSolidness(SPECTRAL) returns 0");
    ASSERT_TRUE(!p_o->isSolid(), "SPECTRAL object is NOT solid");
    ASSERT_EQ(p_o->setSolidness(df::HARD), 0, "setSolidness(HARD) returns 0");

    // Speed / Direction / Velocity
    p_o->setSpeed(5.0f);
    ASSERT_EQ(p_o->getSpeed(), 5.0f, "setSpeed/getSpeed");

    df::Vector dir(1.0f, 0.0f);
    p_o->setDirection(dir);
    ASSERT_EQ(p_o->getDirection().getX(), 1.0f, "setDirection/getDirection");

    df::Vector vel = p_o->getVelocity();
    ASSERT_NEAR(vel.getX(), 5.0f, 0.01f, "getVelocity x = speed * direction.x");
    ASSERT_NEAR(vel.getY(), 0.0f, 0.01f, "getVelocity y = speed * direction.y");

    // predictPosition
    df::Vector pred = p_o->predictPosition();
    ASSERT_NEAR(pred.getX(), 10.0f, 0.01f, "predictPosition x = pos.x + vel.x");
    ASSERT_NEAR(pred.getY(), 10.0f, 0.01f, "predictPosition y = pos.y + vel.y");

    // setVelocity
    df::Vector new_vel(3.0f, 4.0f); // magnitude = 5
    p_o->setVelocity(new_vel);
    ASSERT_NEAR(p_o->getSpeed(), 5.0f, 0.01f, "setVelocity sets speed to magnitude");

    // Event handler returns 0 for unhandled events
    df::Event ev;
    ASSERT_EQ(p_o->eventHandler(&ev), 0, "Base eventHandler returns 0");

    delete p_o; // removes from WorldManager
    LM.writeLog("Object tests complete.");
}

// -----------------------------------------------------------------------
// WORLDMANAGER TESTS
// -----------------------------------------------------------------------
void testWorldManager() {
    std::cout << "\n--- WorldManager Tests ---\n";

    ASSERT_TRUE(WM.isStarted(), "WorldManager is started");

    int before = WM.getAllObjects().getCount();

    // Create objects - they auto-insert
    df::Object *p1 = new df::Object();
    df::Object *p2 = new df::Object();
    p1->setType("TypeA");
    p2->setType("TypeB");

    ASSERT_EQ(WM.getAllObjects().getCount(), before + 2, "insertObject: count increases by 2");

    // getAllObjects
    df::ObjectList all = WM.getAllObjects();
    bool found1 = false, found2 = false;
    for (int i = 0; i < all.getCount(); i++) {
        if (all[i] == p1) found1 = true;
        if (all[i] == p2) found2 = true;
    }
    ASSERT_TRUE(found1, "getAllObjects contains p1");
    ASSERT_TRUE(found2, "getAllObjects contains p2");

    // objectsOfType
    df::ObjectList typeA = WM.objectsOfType("TypeA");
    ASSERT_EQ(typeA.getCount(), 1, "objectsOfType TypeA count = 1");
    ASSERT_EQ(typeA[0], p1, "objectsOfType TypeA returns p1");

    // markForDelete (deferred deletion)
    WM.markForDelete(p1);
    // Before update, p1 still there
    ASSERT_EQ(WM.getAllObjects().getCount(), before + 2,
              "markForDelete: object not deleted until update()");
    WM.update();
    // After update, p1 gone (p1 is now deleted, don't use it)
    ASSERT_EQ(WM.getAllObjects().getCount(), before + 1,
              "After update(): marked object deleted");

    // removeObject directly
    delete p2; // destructor calls WM.removeObject
    ASSERT_EQ(WM.getAllObjects().getCount(), before,
              "removeObject via destructor: count back to start");

    LM.writeLog("WorldManager tests complete.");
}

// -----------------------------------------------------------------------
// LOGMANAGER TESTS
// -----------------------------------------------------------------------
void testLogManager() {
    std::cout << "\n--- LogManager Tests ---\n";

    ASSERT_TRUE(LM.isStarted(), "LogManager is started");

    // Write various types
    int r1 = LM.writeLog("LogManager test: plain string");
    ASSERT_TRUE(r1 >= 0, "writeLog plain string returns >= 0");

    int r2 = LM.writeLog("LogManager test: integer %d", 42);
    ASSERT_TRUE(r2 >= 0, "writeLog with integer");

    int r3 = LM.writeLog("LogManager test: float %.2f", 3.14f);
    ASSERT_TRUE(r3 >= 0, "writeLog with float");

    int r4 = LM.writeLog("LogManager test: string %s", "hello");
    ASSERT_TRUE(r4 >= 0, "writeLog with string arg");

    LM.writeLog("LogManager tests complete.");
}

// -----------------------------------------------------------------------
// STEP EVENT via GameManager onEvent TEST
// -----------------------------------------------------------------------
void testStepEvent() {
    std::cout << "\n--- Step Event / Manager::onEvent Tests ---\n";

    TestObject *p_t = new TestObject();
    int before = p_t->step_count;

    df::EventStep es(1);
    GM.onEvent(&es);
    ASSERT_EQ(p_t->step_count, before + 1, "Step event delivered to TestObject");

    GM.onEvent(&es);
    GM.onEvent(&es);
    ASSERT_EQ(p_t->step_count, before + 3, "Multiple step events delivered");

    delete p_t;
    LM.writeLog("Step event tests complete.");
}

// -----------------------------------------------------------------------
// COLLISION / OUT-OF-BOUNDS EVENT TEST (simple dispatch check)
// -----------------------------------------------------------------------
void testEventDispatch() {
    std::cout << "\n--- EventCollision / EventOut Dispatch Tests ---\n";

    TestObject *p_a = new TestObject();
    TestObject *p_b = new TestObject();

    // Manually dispatch collision event
    df::EventCollision ec(p_a, p_b, df::Vector(0, 0));
    p_a->eventHandler(&ec);
    p_b->eventHandler(&ec);
    ASSERT_EQ(p_a->collision_count, 1, "Object A received collision event");
    ASSERT_EQ(p_b->collision_count, 1, "Object B received collision event");

    // getObject1/getObject2 correctness
    ASSERT_EQ(ec.getObject1(), p_a, "EventCollision getObject1 = p_a");
    ASSERT_EQ(ec.getObject2(), p_b, "EventCollision getObject2 = p_b");
    ASSERT_EQ(ec.getPosition().getX(), 0.0f, "EventCollision position x=0");

    // Out event
    df::EventOut eo;
    p_a->eventHandler(&eo);
    ASSERT_EQ(p_a->out_count, 1, "Object received out-of-bounds event");

    delete p_a;
    delete p_b;
    LM.writeLog("Event dispatch tests complete.");
}

// -----------------------------------------------------------------------
// SHORT GAME LOOP TEST (3 steps, then setGameOver)
// -----------------------------------------------------------------------
class QuitAfterN : public df::Object {
public:
    int n;
    int count = 0;
    QuitAfterN(int n_steps) : n(n_steps) { setType("QuitAfterN"); }

    int eventHandler(const df::Event *p_e) override {
        if (p_e->getType() == STEP_EVENT) {
            count++;
            if (count >= n) {
                GM.setGameOver(true);
            }
            return 1;
        }
        return 0;
    }
};

void testGameLoop() {
    std::cout << "\n--- Game Loop Test (3 steps) ---\n";

    QuitAfterN *p_q = new QuitAfterN(3);
    GM.setGameOver(false);
    GM.run(); // should return after 3 steps
    ASSERT_EQ(p_q->count, 3, "Game loop ran exactly 3 steps");

    delete p_q;
    LM.writeLog("Game loop test complete.");
}

// -----------------------------------------------------------------------
// MAIN
// -----------------------------------------------------------------------
int main() {
    std::cout << "======================================\n";
    std::cout << " Dragonfly Engine - Test Suite\n";
    std::cout << "======================================\n";

    // Start engine
    if (GM.startUp() != 0) {
        std::cerr << "ERROR: GameManager failed to start. Is df-font.ttf present?\n";
        return 1;
    }

    std::cout << "All managers started OK.\n";

    // Run tests
    testVector();
    testClock();
    testObjectList();
    testEvents();
    testLogManager();
    testObject();
    testWorldManager();
    testStepEvent();
    testEventDispatch();
    testGameLoop();

    // Summary
    std::cout << "\n======================================\n";
    std::cout << " Results: " << tests_passed << " passed, "
              << tests_failed << " failed\n";
    std::cout << "======================================\n";
    std::cout << "(See dragonfly.log for detailed engine log)\n";

    GM.shutDown();
    return (tests_failed == 0) ? 0 : 1;
}
