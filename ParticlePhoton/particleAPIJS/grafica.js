const DATASIZE = 50;

let temp=0;
let hum=0;
let max_temp=0;
let max_hum=0;

let tempData = [];
let humData = [];

const labels = Array(DATASIZE).fill(1).map((n, i) => n + i)
const data = {
  labels: labels,
  datasets: [
    {
      label: 'Temperatura',
      data: tempData,
    },
    {
      label: 'Humedad',
      data: humData,
    }
  ]
};
const config = {
  type: 'line',
  data: data,
  options: {
    responsive: true,
    plugins: {
      legend: {
        position: 'top',
      },
      title: {
        display: true,
        text: 'Datos de DTH11 Enviados por Photon'
      }
    },
    scales: {
      y: {
        type: 'linear',
        display: true,
        position: 'left',
        suggestedMin: 0,
        suggestedMax: 100,
      },
      y1: {
        type: 'linear',
        display: true,
        position: 'right',
        grid: {
          drawOnChartArea: false
        },
        min: 0,
        max: 100,
      },
    }
  },
};

const $Body = $('body')[0];
const $Ctx = $('#Grafica');

const chart = new Chart($Ctx, config);

function addData (temp, hum) {
  tempData.push(temp);
  humData.push(hum);

  if (humData.length > DATASIZE) humData.shift()
  if (tempData.length > DATASIZE) tempData.shift()
}

function toggleBulb (hum, temp, max_hum, max_temp) {
  if (hum > max_hum || temp > max_temp) {
    $Body.classList.add('on');
  } else {
    $Body.classList.remove('on');
  }
}

async function fetchData(name, deviceId, token) {
  try {
    const response = await particle.getVariable({
      deviceId: deviceId,
      name: name,
      auth: token
    });
    return response.body.result;
  } catch (err) {
    console.error(`Error fetching variable "${name}"`, err);
    return null;
  }
}

let intervalFunc;

function observePhoton (interval, user, pass, deviceId) {
  if (intervalFunc) clearInterval(intervalFunc);

  intervalFunc = setInterval(async function() {
    try {
      let loginResponse = await particle.login({ username: user, password: pass });
      token = loginResponse.body.access_token;

      const [newTemp, newHum, newMaxTemp, newMaxHum] = await Promise.all([
        fetchData('TEMP', deviceId, token),
        fetchData('HUM', deviceId, token),
        fetchData('MAX_TEMP', deviceId, token),
        fetchData('MAX_HUM', deviceId, token)
      ]);

      if (newTemp !== null) temp = newTemp;
      if (newHum !== null) hum = newHum;
      if (newMaxTemp !== null) max_temp = newMaxTemp;
      if (newMaxHum !== null) max_hum = newMaxHum;

      addData(temp, hum);
      toggleBulb(hum, temp, max_hum, max_temp);
      chart.update();

    } catch (err) {
      console.error('Error observing Photon', err);
    }
  },interval * 1000);
}
