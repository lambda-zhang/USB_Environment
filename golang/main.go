package main

import (
	"fmt"
	"log"
	"net"
	"net/http"
	"os"

	c "./collector"
	s "./cron"
	m "./models"
	r "./router"
)

var period_sec int = 5
var usbenv c.USB_env
var ledon uint16 = 0

func callback() {
	err := usbenv.USB_init()
	if err == nil {
		ret, err2 := usbenv.Get_Environment()
		if err2 == nil {
			log.Printf("Temperature=%d Pressure=%d Humidity=%d\n", ret.Temperature, ret.Pressure, ret.Humidity)
			t := m.Temperature{Val: ret.Temperature, DevId: 0}
			h := m.Humidity{Val: ret.Humidity, DevId: 0}
			p := m.Pressure{Val: ret.Pressure, DevId: 0}
			t.Save()
			h.Save()
			p.Save()

			if ledon != 0 {
				ledon = 0
			} else {
				ledon = 1
			}
			usbenv.Set_LED(ledon)
		}
	}
	usbenv.USB_close()
}

func atexit() {
	log.Println(">>> exit")
}

func printlistenaddr(port string) {
	addrs, err := net.InterfaceAddrs()
	if err != nil {
		panic(err)
	}
	for _, address := range addrs {
		if ipnet, ok := address.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
			if ipnet.IP.To4() != nil {
				log.Printf("http://%s%s", ipnet.IP.String(), port)
			}
		}
	}
}

func main() {
	_setinterval := s.New(period_sec, callback)
	_setinterval.Start()

	router := r.Router()

	port := os.Getenv("PORT")
	if port == "" {
		port = "9000"
	}
	endPoint := fmt.Sprintf(":%s", port)

	defer atexit()

	printlistenaddr(endPoint)
	if err := http.ListenAndServe(endPoint, router); err != nil {
		log.Fatal(err)
	}
}
