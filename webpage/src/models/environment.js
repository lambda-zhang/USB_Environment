export var getEnvironmentHour = _getEnvironmentHour
export var getEnvironmentDay = _getEnvironmentDay
export var getEnvironmentMonth = _getEnvironmentMonth

function _getdata (data) {
  var x = data.ts
  var chartopt = {
    title: {text: '环境'},
    tooltip: {trigger: 'axis'},
    legend: {x: 'right', data: ['湿度', '温度', '气压'], textStyle: {color: '#2c3e50', fontsize: 5}},
    grid: {left: '3%', right: '4%', bottom: '3%', containLabel: true},
    xAxis: {type: 'category', data: x},
    yAxis: [
      {
        boundaryGap: [0, '50%'],
        axisLine: {lineStyle: {color: '#0B438B'}},
        type: 'value',
        name: '湿度%',
        position: 'left',
        offset: 40,
        max: parseInt(data.maxh) + (10 - (parseInt(data.maxh) % 10)),
        min: parseInt(data.minh) - (parseInt(data.minh) % 10),
        splitNumber: 10,
        axisLabel: {formatter: '   {value}', textStyle: {color: '#0B438B'}},
        splitLine: {show: false}
      },
      {
        boundaryGap: [0, '50%'],
        axisLine: {lineStyle: {color: '#0B438B'}},
        splitLine: {show: false},
        type: 'value',
        name: '温度℃',
        position: 'left',
        max: parseInt(data.maxt) + (10 - (parseInt(data.maxt) % 10)),
        min: parseInt(data.mint) - (parseInt(data.mint) % 10),
        axisLabel: {formatter: '{value}'}
      },
      {
        boundaryGap: [0, '50%'],
        axisLine: {lineStyle: {color: '#0B438B'}},
        splitLine: {show: false},
        type: 'value',
        name: '气压hPa',
        max: parseInt(data.maxp) + (10 - (parseInt(data.maxp) % 10)),
        min: parseInt(data.minp) - (parseInt(data.minp) % 10),
        position: 'right',
        axisTick: {inside: 'false', length: 10}
      }
    ],
    series: [
      {name: '湿度', symbol: 'none', type: 'line', smooth: 0.3, color: '#00EC00', data: data.humidity, yAxisIndex: 0},
      {name: '温度', symbol: 'none', type: 'line', smooth: 0.3, color: '#FF2D2D', data: data.temperature, yAxisIndex: 1},
      {name: '气压', symbol: 'none', type: 'line', smooth: 0.3, color: '#FFD306', data: data.pressure, yAxisIndex: 2}
    ]
  }
  return chartopt
}

function _getEnvironmentHour (_this, cb) {
  _this.$http.get('/env2?type=hour').then((res) => {
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = Object.assign(res.data)
    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}

function _getEnvironmentDay (_this, cb) {
  _this.$http.get('/env2?type=day').then((res) => {
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = Object.assign(res.data)
    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}

function _getEnvironmentMonth (_this, cb) {
  _this.$http.get('/env2?type=month').then((res) => {
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = Object.assign(res.data)
    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}
