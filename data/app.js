const effectNameEl = document.getElementById('effect-name');
const fpsEl = document.getElementById('fps');
const powerStateEl = document.getElementById('power-state');
const networkInfoEl = document.getElementById('network-info');
const btnPower = document.getElementById('btn-power');
const btnAutomode = document.getElementById('btn-automode');
const btnPrev = document.getElementById('btn-prev');
const btnNext = document.getElementById('btn-next');
const btnReset = document.getElementById('btn-reset');
const effectSelect = document.getElementById('effect-select');
const brightnessSlider = document.getElementById('brightness-slider');
const brightnessValue = document.getElementById('brightness-value');

let status = null;
let effectsLoaded = false;
let brightnessTimer = null;
let modeBusy = false;
let statusTimer = null;
let pollIntervalMs = 3000;

async function postMode(path) {
  if (modeBusy) {
    return;
  }
  modeBusy = true;
  btnPrev.disabled = true;
  btnNext.disabled = true;
  try {
    await api(path, { method: 'POST' });
    await refreshStatus();
  } finally {
    modeBusy = false;
    btnPrev.disabled = false;
    btnNext.disabled = false;
  }
}

async function api(path, options = {}) {
  const headers = { ...options.headers };
  if (options.body !== undefined) {
    headers['Content-Type'] = 'application/json';
  }
  const response = await fetch(path, {
    ...options,
    headers,
  });
  if (!response.ok) {
    throw new Error(`${path} failed: ${response.status}`);
  }
  if (response.status === 204) {
    return null;
  }
  const text = await response.text();
  return text ? JSON.parse(text) : null;
}

async function loadEffects() {
  const effects = await api('/api/effects');
  effectSelect.innerHTML = '';
  effects.forEach((effect) => {
    const option = document.createElement('option');
    option.value = String(effect.index);
    option.textContent = `${effect.index}: ${effect.name}`;
    effectSelect.appendChild(option);
  });
  effectsLoaded = true;
}

function renderStatus(data) {
  status = data;
  effectNameEl.textContent = `${data.effectIndex}: ${data.effectName}`;
  fpsEl.textContent = data.fps.toFixed(1);
  powerStateEl.textContent = data.power ? 'Вкл' : 'Выкл';
  networkInfoEl.textContent = `${data.ssid} · ${data.ip}`;

  btnPower.textContent = data.power ? 'Выключить' : 'Включить';
  btnPower.classList.toggle('active', data.power);
  btnAutomode.textContent = data.autoMode ? 'Автомод: вкл' : 'Автомод: выкл';
  btnAutomode.classList.toggle('active', data.autoMode);

  brightnessSlider.value = String(data.brightness);
  brightnessValue.textContent = String(data.brightness);

  if (effectsLoaded) {
    effectSelect.value = String(data.effectIndex);
  }
}

async function refreshStatus() {
  try {
    const data = await api('/api/status');
    renderStatus(data);
    networkInfoEl.textContent = `${data.ssid} · ${data.ip}`;
    return true;
  } catch (error) {
    networkInfoEl.textContent = 'Нет связи, повтор...';
    console.error(error);
    return false;
  }
}

function startStatusPolling() {
  if (statusTimer) {
    clearInterval(statusTimer);
  }
  statusTimer = setInterval(refreshStatus, pollIntervalMs);
}

function stopStatusPolling() {
  if (statusTimer) {
    clearInterval(statusTimer);
    statusTimer = null;
  }
}

document.addEventListener('visibilitychange', () => {
  if (document.hidden) {
    stopStatusPolling();
    return;
  }
  refreshStatus();
  startStatusPolling();
});

btnPower.addEventListener('click', async () => {
  await api('/api/power', {
    method: 'POST',
    body: JSON.stringify({ on: !(status && status.power) }),
  });
  await refreshStatus();
});

btnAutomode.addEventListener('click', async () => {
  await api('/api/automode', {
    method: 'POST',
    body: JSON.stringify({ enabled: !(status && status.autoMode) }),
  });
  await refreshStatus();
});

btnPrev.addEventListener('click', () => {
  postMode('/api/mode/prev');
});

btnNext.addEventListener('click', () => {
  postMode('/api/mode/next');
});

effectSelect.addEventListener('change', async () => {
  await api('/api/mode', {
    method: 'POST',
    body: JSON.stringify({ index: Number(effectSelect.value) }),
  });
  await refreshStatus();
});

brightnessSlider.addEventListener('input', () => {
  brightnessValue.textContent = brightnessSlider.value;
  clearTimeout(brightnessTimer);
  brightnessTimer = setTimeout(async () => {
    await api('/api/brightness', {
      method: 'POST',
      body: JSON.stringify({ value: Number(brightnessSlider.value) }),
    });
    await refreshStatus();
  }, 200);
});

btnReset.addEventListener('click', async () => {
  if (!confirm('Сбросить список эффектов на заводской?')) {
    return;
  }
  await api('/api/reset', { method: 'POST' });
  await loadEffects();
  await refreshStatus();
});

async function init() {
  try {
    await loadEffects();
    await refreshStatus();
    startStatusPolling();
  } catch (error) {
    networkInfoEl.textContent = 'Ошибка связи с платой';
    console.error(error);
    setTimeout(init, 3000);
  }
}

init();
