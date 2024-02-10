#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

namespace android {

class ILedService : public IInterface
{
public:
   enum {
        CONNECT = IBinder::FIRST_CALL_TRANSACTION,
        LED_ON,
        LED_OFF,
        LED_SET_NAME,
        CONNECT
   };

public:
    /*
      #define DECLARE_META_INTERFACE(INTERFACE)                           \
      static const String16 descriptor;                                   \
      static sp<I##INTERFACE> asInterface(const sp<IBinder>& obj);        \
      virtual String16 getInterfaceDescriptor() const;                    \
     */
    DECLARE_META_INTERFACE(LedService);

    virutal int setOn(int led) = 0;
    // Step 1: extend API
    virutal int setOff(int led, int delay) = 0;
    virtual int setName(const char *name) = 0;
};


class BnLedService: public BnInterface<ILedService>
{
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

class LedService : public BnLedService
{
public:
    static void instantiate();
    virtual int setOn(int led);
    virtual int setOff(int led, int delay);
    virtual int setName(const char *name);

private:
    LedService();
    virtual ~LedService();

    class Client
    {
        public:
        private:
        Client(const sp<ILedClient>& ledClient,
               int clientPid);
        ~Client();

        sp<ILedClient>  mLedClient;
        pid_t           mClientPid;
    };    
};   


// -------------------------------------------------------------------  


/*
 * Application callbacks
 */
class ILedClient: public IInterface
{
public:
    enum {
        NOTIFY_CALLBACK = IBinder::FIRST_CALL_TRANSACTION,
    };  
    DECLARE_META_INTERFACE(LedClient);

    virtual void    notifyCallback(int32_t msgType) = 0;
};
        
class BnLedClient: public BnInterface<ILedClient>
{       
public:
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

}   
