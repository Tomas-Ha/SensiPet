#pragma once

#include "../src/blep2p/blep2p.cpp"



/** Schedule processing of events from the BLE middleware in the event queue. */
extern void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);

extern BLE &bleInstance;
/* look for other device and then settle on a role */
extern BluetoothP2P bleP2p;

