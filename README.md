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

### debug
```
$ sudo modprobe usbmon
$ sudo mount -t debugfs none  /sys/kernel/debug
$ sudo ls /sys/kernel/debug/usb/usbmon

$ sudo cat /sys/kernel/debug/usb/devices
T:  Bus=03 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#= 24 Spd=1.5  MxCh= 0
D:  Ver= 1.10 Cls=ff(vend.) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
P:  Vendor=0403 ProdID=c630 Rev= 1.09
S:  Manufacturer=Till Harbaum
S:  Product=LCD2USB Interface
C:* #Ifs= 1 Cfg#= 1 Atr=a0 MxPwr= 74mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=00(>ifc ) Sub=00 Prot=00 Driver=(none)
E:  Ad=81(I) Atr=03(Int.) MxPS=   8 Ivl=10ms

$ sudo cat /sys/kernel/debug/usb/usbmon/3u
$ sudo cat /sys/kernel/debug/usb/usbmon/3t
```

### golang
```
$ go get -u -v github.com/Unknwon/goconfig
$ go get -u -v github.com/gin-contrib/cors
$ go get -u -v github.com/gin-contrib/gzip
$ go get -u -v github.com/gin-gonic/gin
$ go get -u -v github.com/google/gousb
$ go get -u -v github.com/jinzhu/gorm
$ go get -u -v github.com/jinzhu/gorm/dialects/mysql
$ go get -u -v github.com/robfig/cron
$ go get -u -v golang.org/x/sys/unix

$ sudo apt-get install libusb-1.0-0-dev pkg-config
```
