#include "ble/BLE.h"
#include "mbed-trace/mbed_trace.h"
#include "states/globals.h"
#include "action.h"
#include <events/mbed_events.h>
#include <pretty_printer.h>
#include "screen/globals.h"

using namespace std::literals::chrono_literals;

/** Demonstrate advertising and scanning for Bluetooth 4.1 devices
 */
class BluetoothP2P : private mbed::NonCopyable<BluetoothP2P>, public ble::Gap::EventHandler {
  public:
    BluetoothP2P(BLE &ble, events::EventQueue *event_queue)
        : _ble(ble), _event_queue(event_queue) {
        ble::own_address_type_t addr_type;
        ble::address_t address;
        _ble.gap().getAddress(addr_type, address);
        constructDeviceName(address);
    }

    ~BluetoothP2P() {
        if (_ble.hasInitialized()) {
            _ble.shutdown();
        }
    }
    bool is_ble_running = false;

    /** Start BLE interface initialisation */
    void run() {
        /* handle gap events */
        is_ble_running = true;
        _role_established = false;
        _num_search_cycles = 0;
        if (!_init_complete) 
        {
            _ble.gap().setEventHandler(this);

            ble_error_t error = _ble.init(this, &BluetoothP2P::on_init_complete);
            if (error) {
                print_error(error, "Error returned by BLE::init\r\n");
                return;
            }
        }
        else 
        {
            print_mac_address();
            start_role();
        }
    }

  private:
    static const char DEVICE_NAME_PREFIX[];
    char _device_name[20];
    // Maximum RSSI value for a device to be considered within range
    static const int MAX_RSSI = -60;

    /** This is called when BLE interface is initialised and starts the first mode */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *event) {
        if (event->error) {
            print_error(event->error, "Error during the initialisation\r\n");
            return;
        }

        print_mac_address();

        /* all calls are serialised on the user thread through the event queue */
        start_role();
        _init_complete = true;
    }

    void start_role() {
        /* This example is designed to be run on two boards at the same time,
         * depending on our role we will either be the advertiser or scanner,
         * until the roles are established we will cycle the roles until we find each
         * other */
        if (!_role_established) {
            _is_scanner = !_is_scanner;
            _num_search_cycles++;

            if (_is_scanner) {
                _event_queue->call(this, &BluetoothP2P::scan);
            } else {
                _event_queue->call(this, &BluetoothP2P::advertise);
            }
        }
    }

    /** Set up and start advertising */
    void advertise() {
        ble_error_t error;

        /* Set advertising parameters */
        static const ble::AdvertisingParameters advertising_params(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(25)),
            ble::adv_interval_t(ble::millisecond_t(50)));

        error =
            _ble.gap().setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, advertising_params);

        if (error) {
            print_error(error, "Gap::setAdvertisingParameters() failed\r\n");
            return;
        }

        ble::AdvertisingDataSimpleBuilder<ble::LEGACY_ADVERTISING_MAX_SIZE> data_builder;
        data_builder.setFlags();
        data_builder.setName(_device_name);

        /* Set payload for advertising */
        error = _ble.gap().setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE,
                                                 data_builder.getAdvertisingData());

        if (error) {
            print_error(error, "Gap::setAdvertisingPayload() failed\r\n");
            return;
        }

        /* since we have two boards which might start running this example at the same
         * time we randomise the interval of advertising to have them meet when one is
         * advertising and the other one is scanning
         */
        ble::millisecond_t random_duration_ms((2 + rand() % 5) * 1000);
        ble::adv_duration_t random_duration(random_duration_ms);

        /* Start advertising */
        error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE, random_duration);

        if (error) {
            print_error(error, "Gap::startAdvertising() failed\r\n");
            return;
        }

        printf("Advertising started for %dms\r\n", random_duration_ms);
    }

    /** Set up and start scanning */
    void scan() {
        _is_connecting = false;

        ble::ScanParameters scan_params;
        scan_params.setOwnAddressType(ble::own_address_type_t::RANDOM);

        ble_error_t error = _ble.gap().setScanParameters(scan_params);

        if (error) {
            print_error(error, "Error caused by Gap::setScanParameters\r\n");
            return;
        }

        error = _ble.gap().startScan(ble::scan_duration_t(500));

        if (error) {
            print_error(error, "Error caused by Gap::startScan\r\n");
            return;
        }

        printf("Scanning started\r\n");
    }

    void constructDeviceName(const ble::address_t &address) {
        snprintf(_device_name, sizeof(_device_name), "%s%02X%02X%02X%02X", DEVICE_NAME_PREFIX,
                 address[0], address[1], address[2], address[3]);
    }

    /* Gap::EventHandler */

    void onAdvertisingStart(const ble::AdvertisingStartEvent &event) override {
        /* No periodic advertising in Bluetooth 4.1 */
        printf("onAdvertising start. \r\n");
    }

    /** Look at scan payload to find a peer device and connect to it */
    void onAdvertisingReport(const ble::AdvertisingReportEvent &event) override {
        /* don't bother with analysing scan result if we're already connecting */
        if (_is_connecting) {
            return;
        }

        ble::AdvertisingDataParser adv_parser(event.getPayload());

        /* parse the advertising payload, looking for a discoverable device */
        while (adv_parser.hasNext()) {
            ble::AdvertisingDataParser::element_t field = adv_parser.next();

            /* identify peer by name prefix and RSSI value */
            if (field.type == ble::adv_data_type_t::COMPLETE_LOCAL_NAME &&
                field.value.size() >= strlen(DEVICE_NAME_PREFIX) &&
                memcmp(field.value.data(), DEVICE_NAME_PREFIX, strlen(DEVICE_NAME_PREFIX)) == 0 &&
                event.getRssi() >= MAX_RSSI) {

                ble_error_t error = _ble.gap().connect(
                    event.getPeerAddressType(), event.getPeerAddress(),
                    ble::ConnectionParameters() // use the default connection parameters
                );

                if (error) {
                    print_error(error, "Error caused by Gap::connect\r\n");
                    return;
                }

                /* we may have already scan events waiting to be processed
                 * so we need to remember that we are already connecting
                 * and ignore them */
                _is_connecting = true;

                return;
            }
        }
    }

    void onAdvertisingEnd(const ble::AdvertisingEndEvent &event) override {
        printf("Advertising ended.\r\n");
        if (!event.isConnected()) {
            printf("No device connected to us, (%d) ", _num_search_cycles);
            if (_num_search_cycles >= 10) {
                printf("ending BLE\r\n");
                is_ble_running = false;
                return;
            }
            printf("switch modes.\r\n");
            start_role();
        }
        
    }

    void onScanTimeout(const ble::ScanTimeoutEvent &event) override {
        printf("Scanning ended\r\n");
        if (!_is_connecting) {
            printf("Failed to find peer\r\n");
            start_role();
        }
    }

    /** This is called by Gap to notify the application we connected */
    void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override {
        if (event.getStatus() == BLE_ERROR_NONE) {
            printf("Connected to: ");
            print_address(event.getPeerAddress().data());
            printf("Roles established\r\n");
            _role_established = true;
            is_ble_running = false;
            _event_queue->call([&](){gSensiPet.update_state(Action::FRIEND);});
            // Disconnect after a short delay
            _event_queue->call_in(1000ms, [this, handle = event.getConnectionHandle()] {
                _ble.gap().disconnect(handle,
                                      ble::local_disconnection_reason_t(
                                          ble::local_disconnection_reason_t::USER_TERMINATION));
            });
        } else {
            printf("Failed to connect\r\n");
            start_role();
        }
    }

    /** This is called by Gap to notify the application we disconnected */
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override {
        
        printf("Disconnected\r\n");
    }

    BLE &_ble;
    events::EventQueue *_event_queue;

    bool _is_scanner = false;
    bool _is_connecting = false;
    bool _role_established = false;
    bool _init_complete = false;
    int _num_search_cycles = 0;
};
