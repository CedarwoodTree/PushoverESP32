#ifndef PUSHOVER_H
#define PUSHOVER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


struct PushoverMessage
{
    // Typecast beforehand :)
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
    const char *_API_KEY;
    const char *_API_USER;
    const char *_API_ENDPOINT;

public:
    Pushover();                                                                    // Default constructor
    Pushover(const char *API_KEY, const char *API_USER, const char *API_ENDPOINT); // Constructor with parameters

    String getJsonNotification(PushoverMessage msg);
    int send(PushoverMessage newMessage);

    // Setters
    void setKey(const char *API_KEY);
    void setUser(const char *API_USER);
    void setEndpoint(const char *API_ENDPOINT);
};

#endif