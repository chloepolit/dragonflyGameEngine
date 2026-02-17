#include "EventStep.h"

namespace df {

EventStep::EventStep() : m_step_count(0) {
    setType(STEP_EVENT);
}

EventStep::EventStep(int init_step_count) : m_step_count(init_step_count) {
    setType(STEP_EVENT);
}

void EventStep::setStepCount(int new_step_count) {
    m_step_count = new_step_count;
}

int EventStep::getStepCount() const {
    return m_step_count;
}

} // end namespace df
