#include "Pushover.h"
#include <ArduinoJson.h>

// Store Expiry Date in Memory
RTC_DATA_ATTR unsigned long EXPIRE_TIME = 60;

// Class Constructors
Pushover::Pushover() {}

Pushover::Pushover(const char *API_KEY, const char *API_USER, const char *API_ENDPOINT)
{
    _API_KEY = API_KEY;
    _API_USER = API_USER;
    _API_ENDPOINT = API_ENDPOINT;
}

// SSL CERT - PUSHOVER
const char *PUSHOVER_ROOT_CA =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEszCCA5ugAwIBAgIQCyWUIs7ZgSoVoE6ZUooO+jANBgkqhkiG9w0BAQsFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
    "MjAeFw0xNzExMDIxMjI0MzNaFw0yNzExMDIxMjI0MzNaMGAxCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xHzAdBgNVBAMTFlJhcGlkU1NMIFRMUyBSU0EgQ0EgRzEwggEiMA0GCSqGSIb3\n"
    "DQEBAQUAA4IBDwAwggEKAoIBAQC/uVklRBI1FuJdUEkFCuDL/I3aJQiaZ6aibRHj\n"
    "ap/ap9zy1aYNrphe7YcaNwMoPsZvXDR+hNJOo9gbgOYVTPq8gXc84I75YKOHiVA4\n"
    "NrJJQZ6p2sJQyqx60HkEIjzIN+1LQLfXTlpuznToOa1hyTD0yyitFyOYwURM+/CI\n"
    "8FNFMpBhw22hpeAQkOOLmsqT5QZJYeik7qlvn8gfD+XdDnk3kkuuu0eG+vuyrSGr\n"
    "5uX5LRhFWlv1zFQDch/EKmd163m6z/ycx/qLa9zyvILc7cQpb+k7TLra9WE17YPS\n"
    "n9ANjG+ECo9PDW3N9lwhKQCNvw1gGoguyCQu7HE7BnW8eSSFAgMBAAGjggFmMIIB\n"
    "YjAdBgNVHQ4EFgQUDNtsgkkPSmcKuBTuesRIUojrVjgwHwYDVR0jBBgwFoAUTiJU\n"
    "IBiV5uNu5g/6+rkS7QYXjzkwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQGCCsG\n"
    "AQUFBwMBBggrBgEFBQcDAjASBgNVHRMBAf8ECDAGAQH/AgEAMDQGCCsGAQUFBwEB\n"
    "BCgwJjAkBggrBgEFBQcwAYYYaHR0cDovL29jc3AuZGlnaWNlcnQuY29tMEIGA1Ud\n"
    "HwQ7MDkwN6A1oDOGMWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydEds\n"
    "b2JhbFJvb3RHMi5jcmwwYwYDVR0gBFwwWjA3BglghkgBhv1sAQEwKjAoBggrBgEF\n"
    "BQcCARYcaHR0cHM6Ly93d3cuZGlnaWNlcnQuY29tL0NQUzALBglghkgBhv1sAQIw\n"
    "CAYGZ4EMAQIBMAgGBmeBDAECAjANBgkqhkiG9w0BAQsFAAOCAQEAGUSlOb4K3Wtm\n"
    "SlbmE50UYBHXM0SKXPqHMzk6XQUpCheF/4qU8aOhajsyRQFDV1ih/uPIg7YHRtFi\n"
    "CTq4G+zb43X1T77nJgSOI9pq/TqCwtukZ7u9VLL3JAq3Wdy2moKLvvC8tVmRzkAe\n"
    "0xQCkRKIjbBG80MSyDX/R4uYgj6ZiNT/Zg6GI6RofgqgpDdssLc0XIRQEotxIZcK\n"
    "zP3pGJ9FCbMHmMLLyuBd+uCWvVcF2ogYAawufChS/PT61D9rqzPRS5I2uqa3tmIT\n"
    "44JhJgWhBnFMb7AGQkvNq9KNS9dd3GWc17H/dXa1enoxzWjE0hBdFjxPhUb0W3wi\n"
    "8o34/m8Fxw==\n"
    "-----END CERTIFICATE-----\n";

// Return true if Post is Allowed
bool Pushover::postAllowed()
{
    unsigned long currentTime = millis();

    bool allow = false;

    if ((currentTime - EXPIRE_TIME) / 1000 >= _TIMEOUT)
    {
        EXPIRE_TIME = currentTime;
        allow = true;
    }

    return allow;
}

String Pushover::getJsonNotification(PushoverMessage msg)
{
    JsonDocument notif;
    notif["token"] = _API_KEY;      // required
    notif["user"] = _API_USER;      // required
    notif["message"] = msg.message; // required
    notif["title"] = msg.title ? msg.title : "";
    notif["url"] = msg.url ? msg.url : "";
    notif["url_title"] = msg.url_title ? msg.url_title : "";
    notif["html"] = msg.html ? msg.html : "";
    notif["priority"] = msg.priority ? msg.priority : "";
    notif["sound"] = msg.sound ? msg.sound : "";
    notif["timestamp"] = msg.timestamp ? msg.timestamp : "";

    String SerializedNotification;
    serializeJson(notif, SerializedNotification);

    return SerializedNotification;
}

int Pushover::send(PushoverMessage newMessage, bool limit)
{
    int responseCode = 0;

    // Timout Check
    if (limit && !postAllowed())
    {
        Serial.println("Timeout Active");
        return responseCode;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        // Initialize Secure WiFi Client
        WiFiClientSecure client;
        client.setCACert(PUSHOVER_ROOT_CA);

        // Initialize HTTP Client with secure Wi-Fi client
        HTTPClient https;
        if (!https.begin(client, _API_ENDPOINT))
        {
            // Error handling: if the connection fails
            Serial.println("Unable to connect to the endpoint.");
            return -1; // You can return an error code
        }

        https.addHeader("Content-Type", "application/json");

        // Get the serialized notification JSON
        String SerializedNotification = getJsonNotification(newMessage);

        // Send the HTTP POST request
        responseCode = https.POST(SerializedNotification);
        if (responseCode <= 0)
        {
            // Error handling: if the POST request fails
            Serial.printf("HTTP POST failed, response code: %d\n", responseCode);
        }

        https.end(); // End the HTTPS session
    }
    else
    {
        Serial.println("WiFi Not Connected");
    }

    return responseCode;
}

// Setters
Pushover &Pushover::setKey(const char *API_KEY)
{
    this->_API_KEY = API_KEY;
    return *this;
};

Pushover &Pushover::setUser(const char *API_USER)
{
    this->_API_USER = API_USER;
    return *this;
};

Pushover &Pushover::setEndpoint(const char *API_ENDPOINT)
{
    this->_API_ENDPOINT = API_ENDPOINT;
    return *this;
};

Pushover &Pushover::setTimeout(unsigned int TIMEOUT)
{
    this->_TIMEOUT = TIMEOUT;
    return *this;
}