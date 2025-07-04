enum class E_GattsProfil_t {
    GATTS_CB,          // Callback function for GATT server events
    GATTS_IF,          // GATT interface handle, assigned by the ESP-IDF stack
    APP_ID,            // Application ID for the GATT profile
    CONN_ID,           // Connection ID for the BLE connection
    SERVICE_HANDLE,    // Handle for the GATT service
    SERVICE_ID,        // Service ID, including UUID and instance information
    CHAR_HANDLE,       // Handle for the characteristic
    CHAR_UUID,         // UUID for the characteristic
    PERM,              // Permissions for the characteristic (e.g., READ, WRITE)
    PROPERTY,          // Properties for the characteristic (e.g., READ, WRITE, NOTIFY)
    DESCR_HANDLE,      // Handle for the descriptor
    DESCR_UUID         // UUID for the descriptor
};