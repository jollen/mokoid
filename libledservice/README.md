## 實作 Android Native Service

## Step 1: Use BnInterface template

* Native service 的 server 使用 BnInterface template

```
class BnLedService: public BnInterface<ILedService>
{
};
```

## Step 2: Declare ILedService and extend framework

** 將 ILedService 擴充 (extend) 至 Android Framework

```
class ILedService: public IInterface
{

};
```

## Step 3: Declare LedService class 

* 物件的實例化將會使用 singleton pattern
* 使用 virtual function (polymorphism)

```
class LedService: public BnLedService
{
private:
    LedService();
    virtual ~LedService();
};
```

## Step 4: Define APIs

* 定義 API

```
class ILedService: public IInterface
{
    int setOn(int led);
    int setOff(int led);
};
```

* 使用 virtual function
* 透過 *instantiate()* 取得 instance (singleton pattern)

```
class LedService: public BnLedService
{
public:
    static void instantiate();
    virtual int setOn(int led);
    virtual int setOff(int led);

private:
    LedService();
    virtual ~LedService();
};
```

## Step 5: 實作 methods 原型

* 實作 instantiate() 與 constructor

```
LedService::LedService()
{
}

// Singleton
void LedService::instantiate() {
    defaultServiceManager()->addService(
    String16("led"), new LedService());
}
```

## Step 6: Declare asInterface() 與其它

* 使用 DECLARE_META_INTERFACE 巨集
* 提供 Remote binder proxy

```
class ILedService: public IInterface
{
public:
    DECLARE_META_INTERFACE(LedService);

    int setOn(int led);
    int setOff(int led);
};
```

## Step 7: Implement asInterface() 與其它

* 使用 IMPLEMENT_META_INTERFACE 巨集
* .asInterface() 的 code template

```
IMPLEMENT_META_INTERFACE(LedService, "mokoid.hardware.ILedService");
```

## Step 8: 使用 BpInterface

* 生成 Binder proxy object

```
/*
 * Binder proxy object implementation
 */
class BpLedService: public BpInterface<ILedService>
{
public:
    BpLedService(const sp<IBinder>& impl)
        : BpInterface<ILedService>(impl)
    {
    }

    virtual int setOn(int led)
    {
    }

};
```

## Step 9: 實作 Binder proxy

* Binder IPC 實作
* 呼叫 transact()

```
class BpLedService: public BpInterface<ILedService>
{
public:
    BpLedService(const sp<IBinder>& impl)
        : BpInterface<ILedService>(impl)
    {
    }

    virtual int setOn(int led)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ILedService::getInterfaceDescriptor());
        remote()->transact(BnLedService::LED_ON, data, &reply);
        return 0;
    }

};
```

## Final Step: 實作 onTransact()

* Native Service (Server) 的 onTransact() 實作

```
status_t BnLedService::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch(code) {
        case CONNECT: {
            CHECK_INTERFACE(ILedService, data, reply);
            return NO_ERROR;
        } break;
    case LED_ON:
        return NO_ERROR;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
}
```



