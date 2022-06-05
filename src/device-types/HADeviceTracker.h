#ifndef AHA_HADEVICETRACKER_H
#define AHA_HADEVICETRACKER_H

#include "BaseDeviceType.h"

#ifndef EX_ARDUINOHA_DEVICE_TRACKER

class HADeviceTracker : public BaseDeviceType
{
public:
    enum SourceType {
        SourceTypeUnknown = 0,
        SourceTypeGPS,
        SourceTypeRouter,
        SourceTypeBluetooth,
        SourceTypeBluetoothLE
    };

    enum TrackerState {
        StateUnknown = 0,
        StateHome,
        StateNotHome,
        StateNotAvailable
    };

    HADeviceTracker(
        const char* uniqueId,
        const TrackerState initialState = StateUnknown
    );

    /**
     * Sets icon of the tracker.
     * Any icon from MaterialDesignIcons.com. Prefix name with mdi:, ie mdi:home.
     *
     * @param class Icon name
     */
    inline void setIcon(const char* icon)
        { _icon = icon; }

    /**
     * Sets source type of the tracker.
     * 
     * @param Source type (see SourceType enum)
     */
    inline void setSourceType(const SourceType type)
        { _sourceType = type; }

    /**
     * Changes state of the tracker and publishes MQTT message.
     * Please note that if a new value is the same as previous one,
     * the MQTT message won't be published.
     *
     * @param state New state of the tracker.
     * @param force Forces to update state without comparing it to previous known state.
     * @returns Returns true if MQTT message has been published successfully.
     */
    bool setState(const TrackerState state, const bool force = false);

    /**
     * Returns last known state of the tracker.
     * If setState method wasn't called the initial value will be returned.
     */
    inline TrackerState getState() const
        { return _currentState; }

protected:
    virtual void buildSerializer() override;
    virtual void onMqttConnected() override;

private:
    bool publishState(TrackerState state);

    const char* _icon;
    SourceType _sourceType;
    TrackerState _currentState;
};

#endif
#endif