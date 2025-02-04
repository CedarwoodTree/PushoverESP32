#ifndef PUSHOVER_H
#define PUSHOVER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Default time in seconds before notification can be sent again
// #define DEFAULT_DELAY 60

struct PushoverMessage
{
    // Typecast beforehand
    const char *message = "";
    const char *title = "";
    const char *url = "";
    const char *url_title = "";
    const char *sound = "";
    const char *html = "";
    const char *priority = "";
    const char *timestamp = "";
};

class Pushover
{
private:
    // Class Variables
    const char *_API_KEY;
    const char *_API_USER;
    const char *_API_ENDPOINT;
    unsigned int _TIMEOUT = 60; // In Seconds
    // Private Methods
    bool postAllowed();

public:
    Pushover();                                                                    // Default constructor
    Pushover(const char *API_KEY, const char *API_USER, const char *API_ENDPOINT); // Constructor with parameters

    // Public Methods
    String getJsonNotification(PushoverMessage msg);
    int send(PushoverMessage newMessage, bool limit = true);

    // Setters
    Pushover &setKey(const char *API_KEY);
    Pushover &setUser(const char *API_USER);
    Pushover &setEndpoint(const char *API_ENDPOINT);
    Pushover &setTimeout(unsigned int TIMEOUT);
};

#endif