# Howto

* Copy the ```led/``` folder to AOSP ```hardware/interfaces``` source tree.
* Run `update-makefiles.sh` to generate makefiles: ```.//hardware/interfaces/update-makefiles.sh```
  
## Add HIDL HALs to Android 10+

For development workaround, add `android.hardware.led@1.0` to the HIDL map list:

```
system/tools/hidl/build/hidl_interface.go
```

NOTICE: ```As of Android 10, HIDL is deprecated and Android is migrating to use AIDL everywhere,``` see: [HIDL](https://source.android.com/devices/architecture/hidl)
