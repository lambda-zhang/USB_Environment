package configs

import (
	"log"
	"os"
	"strconv"
	"strings"

	"github.com/Unknwon/goconfig"
)

type MySqlcfg struct {
	Host     string
	Port     int64
	User     string
	Database string
	Password string
}

var (
	MySql MySqlcfg
)

func getConfigStringValue(cfg *goconfig.ConfigFile, section string, key string, envName string) (val string, err error) {
	env := os.Getenv(envName)
	if strings.Count(env, "") > 1 {
		log.Printf("ENV: %s=%s\n", key, env)
		return env, nil
	}
	value, err := cfg.GetValue(section, key)
	if err != nil {
		log.Printf("section or value not found %s %s\n", section, key)
		return "", err
	} else {
		log.Printf("CFG: %s=%s\n", key, value)
		return value, nil
	}
}

func getConfigIntValue(cfg *goconfig.ConfigFile, section string, key string, envName string) (val int64, err error) {
	env := os.Getenv(envName)
	if strings.Count(env, "") > 1 {
		envint64, err := strconv.ParseInt(env, 10, 32)
		if err == nil {
			log.Printf("ENV: %s=%d\n", key, envint64)
			return envint64, nil
		}
	}
	value, err2 := cfg.GetValue(section, key)
	if err2 != nil {
		log.Printf("section or value not found %s %s\n", section, key)
		return 0, err2
	}
	valint64, err3 := strconv.ParseInt(value, 10, 32)
	if err3 == nil {
		log.Printf("CFG: %s=%d\n", key, valint64)
		return valint64, nil
	}
	return valint64, nil
}

func init() {
	var configfile string = "configs/conf-dev.ini"
	ginmode := os.Getenv("GIN_MODE")
	if ginmode == "release" {
		configfile = "configs/conf.ini"
	}
	cfg, err := goconfig.LoadConfigFile(configfile)
	if err != nil {
		log.Printf("load config failed")
	}

	MySql.Host, _ = getConfigStringValue(cfg, "Mysql", "host", "MYSQL_HOST")
	MySql.Port, _ = getConfigIntValue(cfg, "Mysql", "port", "MYSQL_PORT")
	MySql.Database, _ = getConfigStringValue(cfg, "Mysql", "database", "MYSQL_DB")
	MySql.User, _ = getConfigStringValue(cfg, "Mysql", "user", "MYSQL_USR")
	MySql.Password, _ = getConfigStringValue(cfg, "Mysql", "password", "MYSQL_PASSWD")

	log.Printf("config inited")
}
