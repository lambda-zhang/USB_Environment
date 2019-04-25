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

type Temperature struct {
	Type  string    `json:"type"`
	Index []int     `json:"index"`
	Val   []float32 `json:"val"`
}

func GetTemperature(c *gin.Context) {
	type_str := c.DefaultQuery("type", "day")

	temp := Temperature{}
	t := m.Temperature{}
	var r *[]m.Result
	var err error

	r, err = t.Search(time.Now(), type_str)
	if err == nil && len(*r) > 0 {
		temp.Index = make([]int, 0)
		temp.Val = make([]float32, 0)

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
