#include "HADeviceTracker.h"
#ifndef EX_ARDUINOHA_DEVICE_TRACKER

#include "../HAMqtt.h"
#include "../utils/HASerializer.h"

HADeviceTracker::HADeviceTracker(
    const char* uniqueId,
    const TrackerState initialState
) :
    BaseDeviceType("device_tracker", uniqueId),
    _icon(nullptr),
    _sourceType(SourceTypeUnknown),
    _currentState(initialState)
{

}

bool HADeviceTracker::setState(const TrackerState state, const bool force)
{
    if (!force && state == _currentState) {
        return true;
    }

    if (publishState(state)) {
        _currentState = state;
        return true;
    }

    return false;
}

void HADeviceTracker::buildSerializer()
{
    if (_serializer || !uniqueId()) {
        return;
    }

    _serializer = new HASerializer(this);
    _serializer->set(HANameProperty, _name);
    _serializer->set(HAUniqueIdProperty, _uniqueId);
    _serializer->set(HAIconProperty, _icon);
    _serializer->set(
        HASourceTypeProperty,
        getSourceTypeProperty(),
        HASerializer::ProgmemPropertyValue
    );
    _serializer->set(HASerializer::WithDevice);
    _serializer->set(HASerializer::WithAvailability);
    _serializer->topic(HAStateTopic);
}

void HADeviceTracker::onMqttConnected()
{
    if (!uniqueId()) {
        return;
    }

    publishConfig();
    publishAvailability();
    publishState(_currentState);
}

bool HADeviceTracker::publishState(const TrackerState state)
{
    const char *stateP = nullptr;
    switch (state) {
    case StateHome:
        stateP = HAHome;
        break;

    case StateNotHome:
        stateP = HANotHome;
        break;

    case StateNotAvailable:
        stateP = HAOffline;
        break;

    default:
        return false;
    }

    return publishOnDataTopic(HAStateTopic, stateP, true, true);
}

const char* HADeviceTracker::getSourceTypeProperty() const
{
    switch (_sourceType) {
    case SourceTypeGPS:
        return HAGPSType;

    case SourceTypeRouter:
        return HARouterType;

    case SourceTypeBluetooth:
        return HABluetoothType;

    case SourceTypeBluetoothLE:
        return HABluetoothLEType;

    default:
        return nullptr;
    }
}

#endif
