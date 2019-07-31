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

type retVal struct {
	Type  string    `json:"type"`
	Index []int     `json:"index"`
	Val   []float32 `json:"val"`
}

func GetTemperature(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")

	temp := retVal{}
	t := m.Temperature{}
	var r *[]m.Result
	var err error

	r, err = t.Search(time.Now(), type_str)
	if err == nil && len(*r) > 0 {
		temp.Index = make([]int, 0)
		temp.Val = make([]float32, 0)
		temp.Type = type_str

		for i := len(*r) - 1; i >= 0; i-- {
			hour, _ := strconv.ParseInt((*r)[i].Ctx, 10, 64)
			val, _ := strconv.ParseInt((*r)[i].Avg, 10, 64)
			temp.Index = append(temp.Index, int(hour))
			temp.Val = append(temp.Val, float32(val)/10)
		}
		c.JSON(200, gin.H{"status": 200, "data": temp})
	} else {
		c.JSON(200, gin.H{"status": 200, "data": ""})
	}
}

func GetHumidity(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")

	temp := retVal{}
	t := m.Humidity{}
	var r *[]m.Result
	var err error

	r, err = t.Search(time.Now(), type_str)
	if err == nil && len(*r) > 0 {
		temp.Index = make([]int, 0)
		temp.Val = make([]float32, 0)
		temp.Type = type_str

		for i := len(*r) - 1; i >= 0; i-- {
			hour, _ := strconv.ParseInt((*r)[i].Ctx, 10, 64)
			val, _ := strconv.ParseInt((*r)[i].Avg, 10, 64)
			temp.Index = append(temp.Index, int(hour))
			temp.Val = append(temp.Val, float32(val)/10)
		}
		c.JSON(200, gin.H{"status": 200, "data": temp})
	} else {
		c.JSON(200, gin.H{"status": 200, "data": ""})
	}
}

func GetPressure(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")

	temp := retVal{}
	t := m.Pressure{}
	var r *[]m.Result
	var err error

	r, err = t.Search(time.Now(), type_str)
	if err == nil && len(*r) > 0 {
		temp.Index = make([]int, 0)
		temp.Val = make([]float32, 0)
		temp.Type = type_str

		for i := len(*r) - 1; i >= 0; i-- {
			hour, _ := strconv.ParseInt((*r)[i].Ctx, 10, 64)
			val, _ := strconv.ParseInt((*r)[i].Avg, 10, 64)
			temp.Index = append(temp.Index, int(hour))
			temp.Val = append(temp.Val, float32(val)/10)
		}
		c.JSON(200, gin.H{"status": 200, "data": temp})
	} else {
		c.JSON(200, gin.H{"status": 200, "data": ""})
	}
}

type env struct {
	Index       []int     `json:"index"`
	Ts          []int     `json:"ts"`
	Temperature []float32 `json:"temperature"`
	Pressure    []float32 `json:"pressure"`
	Humidity    []float32 `json:"humidity"`
}

func GetEnviroment(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")
	e := env{}
	if type_str == "hour" {
		e.Index = make([]int, 60)
		e.Temperature = make([]float32, 60)
		e.Pressure = make([]float32, 60)
		e.Humidity = make([]float32, 60)
		for i := 0; i < 60; i++ {
			e.Index[i] = i
		}
	} else if type_str == "month" {
		e.Index = make([]int, 32)
		e.Temperature = make([]float32, 32)
		e.Pressure = make([]float32, 32)
		e.Humidity = make([]float32, 32)
		for i := 0; i < 32; i++ {
			e.Index[i] = i
		}
	} else if type_str == "year" {
		e.Index = make([]int, 13)
		e.Temperature = make([]float32, 13)
		e.Pressure = make([]float32, 13)
		e.Humidity = make([]float32, 13)
		for i := 0; i < 13; i++ {
			e.Index[i] = i
		}
	} else {
		//day
		e.Index = make([]int, 24)
		e.Temperature = make([]float32, 24)
		e.Pressure = make([]float32, 24)
		e.Humidity = make([]float32, 24)
		for i := 0; i < 24; i++ {
			e.Index[i] = i
		}
	}

	t := m.Temperature{}
	h := m.Humidity{}
	p := m.Pressure{}
	r_t, err1 := t.Search(time.Now(), type_str)
	r_h, err2 := h.Search(time.Now(), type_str)
	r_p, err3 := p.Search(time.Now(), type_str)

	if err1 != nil || err2 != nil || err3 != nil {
		c.JSON(200, gin.H{"status": 200, "data": ""})
		return
	}

	for i := len(*r_t) - 1; i >= 0; i-- {
		index, _ := strconv.ParseInt((*r_t)[i].Ctx, 10, 64)
		val, _ := strconv.ParseInt((*r_t)[i].Avg, 10, 64)
		e.Temperature[index] = float32(val) / 10
	}
	for i := len(*r_h) - 1; i >= 0; i-- {
		index, _ := strconv.ParseInt((*r_h)[i].Ctx, 10, 64)
		val, _ := strconv.ParseInt((*r_h)[i].Avg, 10, 64)
		e.Humidity[index] = float32(val) / 10
	}
	for i := len(*r_p) - 1; i >= 0; i-- {
		index, _ := strconv.ParseInt((*r_p)[i].Ctx, 10, 64)
		val, _ := strconv.ParseInt((*r_p)[i].Avg, 10, 64)
		e.Pressure[index] = float32(val) / 100
	}
	c.JSON(200, gin.H{"status": 200, "data": e})
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
		e.Temperature[i] = float32(val_t) / 100
		e.Humidity[i] = float32(val_h) / 100
		e.Pressure[i] = float32(val_p) / 100
	}
	c.JSON(200, gin.H{"status": 200, "data": e})
}
