export var getEnvironmentHour = _getEnvironmentHour
export var getEnvironmentDay = _getEnvironmentDay
export var getEnvironmentMonth = _getEnvironmentMonth

function _getdata (data) {
  var x = data.index
  var chartopt = {
    title: {text: '环境'},
    tooltip: {trigger: 'axis'},
    legend: {x: 'right', data: ['湿度', '温度', '气压'], textStyle: {color: '#FFFFFF', fontsize: 5}},
    grid: {left: '3%', right: '4%', bottom: '3%', containLabel: true},
    toolbox: {feature: {saveAsImage: {}}},
    xAxis: {type: 'category', data: x},
    yAxis: [
      {
        boundaryGap: [0, '50%'],
        axisLine: {lineStyle: {color: '#0B438B'}},
        type: 'value',
        name: '湿度%',
        position: 'left',
        offset: 40,
        max: 99,
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
        max: 50,
        axisLabel: {formatter: '{value}'}
      },
      {
        boundaryGap: [0, '50%'],
        axisLine: {lineStyle: {color: '#0B438B'}},
        splitLine: {show: false},
        type: 'value',
        name: '气压hPa',
        max: 999,
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
  _this.$http.get('/env?type=hour').then((res) => {
    var x = []
    console.log(res)
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = res.data
    for (var i = 0; i < data.index.length; i++) {
      x.push(data.index[i])
    }

    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}

function _getEnvironmentDay (_this, cb) {
  _this.$http.get('/env?type=day').then((res) => {
    var x = []
    console.log(res)
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = res.data
    for (var i = 0; i < data.index.length; i++) {
      x.push(data.index[i])
    }

    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}

function _getEnvironmentMonth (_this, cb) {
  _this.$http.get('/env?type=month').then((res) => {
    var x = []
    console.log(res)
    if (!res || !res.status || res.status !== 200) {
      return cb && cb.call(_this, null, 'get cpuusage failed')
    }
    var data = res.data
    for (var i = 0; i < data.index.length; i++) {
      x.push(data.index[i])
    }

    var chartopt = _getdata(data)
    return cb && cb.call(_this, chartopt, null)
  })
}
