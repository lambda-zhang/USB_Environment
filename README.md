### prepare
```
$ sudo apt install gcc-avr avr-libc avrdude

$ cp udev/90-usb-env.rules  /etc/udev/rules.d/90-usb-env.rules
$ chmod a+x /etc/udev/rules.d/90-usb-env.rules
```

### flash firmware
```
$ cd firmware
$ make clean
$ make
$ make flash
```

### run application
```
$ cd app
$ make clean
$ make
$ make run
```
