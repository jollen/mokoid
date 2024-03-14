這是我在[Android HAL & Framework： 軟硬整合實作訓練](https://www.moko365.com/enterprise/af101-android-hal-framework-practical)培訓課程使用的練習範例。

# 入門指南

* [Android 的 HAL 技術, #1: 簡介與發展現況](http://www.jollen.org/blog/2009/10/android-hal-status-report.html)
* [Android 的 HAL 技術, #2: 採用Service架構方式](http://www.jollen.org/blog/2009/10/android-hal-service-introduction.html)
* [Android 的 HAL 技術, #3: 小探Android Service與Native Service](http://www.jollen.org/blog/2009/11/android-hal-android-native-service.html)
* [Android 的 HAL 技術, #4: Android Service 與 HAL Stub](http://www.jollen.org/blog/2009/11/android-hal-android-service-hal.html)
* [Android 的 HAL 技術, #5: 繼承 HAL 的 struct hw_module_t](http://www.jollen.org/blog/2009/12/android-hal-derived-c-language.html)
* [Android 的 HAL 技術, #7: 取得 Proxy Object](http://www.jollen.org/blog/2009/12/android-hal-get-proxy-object.html)
* [Android 的 HAL 技術, #8: 實作 HAL Stub](http://www.jollen.org/blog/2010/01/android-hal-stub-design-implement.html)

# 上機練習

1. [如何從零開始實作一個 Android Native Service](#實作 Android Native Service)

## 如何從零開始實作一個 Android Native Service

### Step 1: Use BnInterface template

* Native service 的 server 使用 BnInterface template

```
class BnLedService: public BnInterface<ILedService>
{
};
```

### Step 2: Declare ILedService and extend framework

** 將 ILedService 擴充 (extend) 至 Android Framework

```
class ILedService: public IInterface
{

};
```

### Step 3: Declare LedService class 

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

### Step 4: Define APIs

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

### Step 5: 實作 LedService 原型

* 實作 instantiate() 與 singleton patter
* constructor 實作
* destructor 實作

```
LedService::LedService()
{
}

LedService::~LedService()
{
}

// Singleton
void LedService::instantiate() {
     defaultServiceManager()->addService(
             String16("led"), new LedService());
}
```

### Step 6: Declare asInterface() 與其它

* 使用 DECLARE_META_INTERFACE 巨集

```
class ILedService: public IInterface
{
public:
    DECLARE_META_INTERFACE(LedService);

    int setOn(int led);
    int setOff(int led);
};
```

### Step 7: Implement asInterface() 與其它

* 使用 IMPLEMENT_META_INTERFACE 巨集

```
IMPLEMENT_META_INTERFACE(LedService, "mokoid.hardware.ILedService");
```

### Step 8: 使用 BpInterface

* 定義 binder proxy
* 實作 virtual function

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
        return 0;
    }

    virtual int setOff(int led)
    {
        return 0;
    }

};
```

### Step 9: 實作 ILedService

* 實作 APIs

```
int LedService::setOn(int led)
{
    return 0;
}

int LedService::setOff(int led)
{
    return 0;
}
```

### Step 10: 實作 Binder Transaction

# Android 11+ Development

## Generating Android.bp

```
androidmk Android.mk > Android.bp
```
