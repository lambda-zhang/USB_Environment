package controllers

import (
	"strconv"
	"time"

	m "../models"

	"github.com/gin-gonic/gin"
)

func Pong(c *gin.Context) {
	c.JSON(200, gin.H{"status": 200, "ping": "pong"})
}

type env struct {
	Index          []int     `json:"index"`
	Ts             []int     `json:"ts"`
	Temperature    []float32 `json:"temperature"`
	Pressure       []float32 `json:"pressure"`
	Humidity       []float32 `json:"humidity"`
	MaxTemperature float32   `json:"maxt"`
	MaxPressure    float32   `json:"maxp"`
	MaxHumidity    float32   `json:"maxh"`
	MinTemperature float32   `json:"mint"`
	MinPressure    float32   `json:"minp"`
	MinHumidity    float32   `json:"minh"`
}

func max(a float32, b float32) (c float32) {
	if a > b {
		return a
	}
	return b
}
func min(a float32, b float32) (c float32) {
	if a < b {
		return a
	}
	return b
}

func GetEnviroment2(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")
	environment := m.Environment{}
	ret_e, err1 := environment.Search(time.Now(), type_str)
	if err1 != nil {
		c.JSON(200, gin.H{"status": 200, "data": ""})
		return
	}

	e := env{}
	length := len(*ret_e)
	e.Index = make([]int, length)
	e.Ts = make([]int, length)
	e.Temperature = make([]float32, length)
	e.Pressure = make([]float32, length)
	e.Humidity = make([]float32, length)
	for i := 0; i < length; i++ {
		e.Index[i] = i
		e.Ts[i] = 0
	}

	for i := 0; i < len(*ret_e); i++ {
		index, _ := strconv.ParseInt((*ret_e)[i].Ctx, 10, 64)
		val_t, _ := strconv.ParseInt((*ret_e)[i].T, 10, 64)
		val_h, _ := strconv.ParseInt((*ret_e)[i].H, 10, 64)
		val_p, _ := strconv.ParseInt((*ret_e)[i].P, 10, 64)
		e.Ts[i] = int(index)
		e.Temperature[i] = float32(val_t) / 10
		e.Humidity[i] = float32(val_h) / 10
		e.Pressure[i] = float32(val_p) / 100

		if i == 0 {
			e.MaxTemperature = e.Temperature[0]
			e.MaxHumidity = e.Humidity[0]
			e.MaxPressure = e.Pressure[0]
			e.MinTemperature = e.Temperature[0]
			e.MinHumidity = e.Humidity[0]
			e.MinPressure = e.Pressure[0]
		}

		e.MaxTemperature = max(e.MaxTemperature, e.Temperature[i])
		e.MaxHumidity = max(e.MaxHumidity, e.Humidity[i])
		e.MaxPressure = max(e.MaxPressure, e.Pressure[i])
		e.MinTemperature = min(e.MinTemperature, e.Temperature[i])
		e.MinHumidity = min(e.MinHumidity, e.Humidity[i])
		e.MinPressure = min(e.MinPressure, e.Pressure[i])
	}
	c.JSON(200, gin.H{"status": 200, "data": e})
}
