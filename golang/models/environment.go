package models

import (
	// "errors"
	"fmt"
	"log"
	"time"

	"github.com/jinzhu/gorm"
)

type Result struct {
	Avg string
	Ctx string
}

func _search(ts time.Time, _type string, dbname string) (*[]Result, error) {
	var ress []Result
	if dbname == "" || _type == "" {
		return &ress, nil
	}

	selector1 := ""
	selector := ""

	if _type == "hour" {
		selector1 = "SELECT ROUND(AVG(val), 0) as avg, DATE_FORMAT(created_at,'%i') as ctx from " +
			dbname + " where DATE_FORMAT(created_at,'%Y-%m-%d %H')=? GROUP BY ctx"
		selector = fmt.Sprintf("%4.4d-%2.2d-%2.2d %2.2d", ts.Year(), ts.Month(), ts.Day(), ts.Hour())
	} else if _type == "day" {
		selector1 = "SELECT ROUND(AVG(val), 0) as avg, DATE_FORMAT(created_at,'%H') as ctx from " +
			dbname + " where DATE_FORMAT(created_at,'%Y-%m-%d')=? GROUP BY ctx"
		selector = fmt.Sprintf("%4.4d-%2.2d-%2.2d", ts.Year(), ts.Month(), ts.Day())
	} else if _type == "month" {
		selector1 = "SELECT ROUND(AVG(val), 0) as avg, DATE_FORMAT(created_at,'%d') as ctx from " +
			dbname + " where DATE_FORMAT(created_at,'%Y-%m')=? GROUP BY ctx"
		selector = fmt.Sprintf("%4.4d-%2.2d", ts.Year(), ts.Month())
	} else if _type == "year" {
		selector1 = "SELECT ROUND(AVG(val), 0) as avg, DATE_FORMAT(created_at,'%m') as ctx from " +
			dbname + " where DATE_FORMAT(created_at,'%Y')=? GROUP BY ctx"
		selector = fmt.Sprintf("%4.4d", ts.Year())
	} else {
		return &ress, nil
	}

	ret := db.Raw(selector1, selector).Scan(&ress)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return &ress, ret.Error
}

type Temperature struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "温度值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Temperature) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Temperature) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

func (this *Temperature) Search(ts time.Time, _type string) (*[]Result, error) {
	return _search(ts, _type, "temperature")
}

type Humidity struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "湿度值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Humidity) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Humidity) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

func (this *Humidity) Search(ts time.Time, _type string) (*[]Result, error) {
	return _search(ts, _type, "humidity")
}

type Pressure struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "气压值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Pressure) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Pressure) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

func (this *Pressure) Search(ts time.Time, _type string) (*[]Result, error) {
	return _search(ts, _type, "pressure")
}
