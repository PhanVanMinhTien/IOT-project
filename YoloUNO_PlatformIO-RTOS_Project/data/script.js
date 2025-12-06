// ==================== WEBSOCKET ====================
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;


window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
    initGauges();
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function Send_Data(data) {
    if (websocket && websocket.readyState === WebSocket.OPEN) {
        websocket.send(data);
        console.log("📤 Gửi:", data);
    } else {
        console.warn("⚠️ WebSocket chưa sẵn sàng!");
        alert("⚠️ WebSocket chưa kết nối!");
    }
}
// function onMessage(event) {
//     console.log("📩 Nhận:", event.data);
//     try {
//         const data = JSON.parse(event.data);

//         // Dữ liệu sensor từ ESP32: { "page":"home", "value": { "temp":..., "humi":... } }
//         if (data.page === "home" && data.value) {
//             const temp = Number(data.value.temp);
//             const humi = Number(data.value.humi);

//             console.log("➡ Temp:", temp, "Humi:", humi);

//             if (!isNaN(temp) && gaugeTemp) {
//                 gaugeTemp.refresh(temp);
//             }
//             if (!isNaN(humi) && gaugeHumi) {
//                 gaugeHumi.refresh(humi);
//             }
//         }

//         // Sau này có thể xử lý thêm page === "device", "setting", ...
//     } catch (e) {
//         console.warn("Không phải JSON hợp lệ:", event.data);
//     }
// }
function onMessage(event) {
    console.log("📩 Nhận:", event.data);
    try {
        const data = JSON.parse(event.data);

        // Dữ liệu sensor từ ESP32: { "page":"home", "value": { "temp":..., "humi":... } }
        if (data.page === "home" && data.value) {
            const temp = Number(data.value.temp);
            const humi = Number(data.value.humi);

            console.log("➡ Temp:", temp, "Humi:", humi);

            // ---- DOM elements ----
            const tempValueEl = document.getElementById("temp_value");
            const tempStatusEl = document.getElementById("temp_status");
            const tempCardEl = document.getElementById("card_temp");

            const humiValueEl = document.getElementById("humi_value");
            const humiStatusEl = document.getElementById("humi_status");
            const humiCardEl = document.getElementById("card_humi");

            // ---- Cập nhật nhiệt độ ----
            if (!isNaN(temp) && tempValueEl && tempStatusEl && tempCardEl) {
                tempValueEl.textContent = temp.toFixed(1);

                // Reset class trạng thái
                tempCardEl.classList.remove("normal", "warning", "critical");

                if (temp >= 35) {
                    tempCardEl.classList.add("critical");
                    tempStatusEl.textContent = "Nhiệt độ cao! (≥ 35°C)";
                } else if (temp >= 30) {
                    tempCardEl.classList.add("warning");
                    tempStatusEl.textContent = "Hơi nóng (30–35°C)";
                } else if (temp <= 20) {
                    tempCardEl.classList.add("warning");
                    tempStatusEl.textContent = "Hơi lạnh (≤ 20°C)";
                } else {
                    tempCardEl.classList.add("normal");
                    tempStatusEl.textContent = "Nhiệt độ bình thường";
                }
            }

            // ---- Cập nhật độ ẩm ----
            if (!isNaN(humi) && humiValueEl && humiStatusEl && humiCardEl) {
                humiValueEl.textContent = humi.toFixed(1);

                humiCardEl.classList.remove("normal", "warning", "critical");

                if (humi >= 80) {
                    humiCardEl.classList.add("critical");
                    humiStatusEl.textContent = "Độ ẩm rất cao! (≥ 80%)";
                } else if (humi >= 60) {
                    humiCardEl.classList.add("warning");
                    humiStatusEl.textContent = "Độ ẩm hơi cao (60–80%)";
                } else if (humi <= 30) {
                    humiCardEl.classList.add("warning");
                    humiStatusEl.textContent = "Độ ẩm thấp (≤ 30%)";
                } else {
                    humiCardEl.classList.add("normal");
                    humiStatusEl.textContent = "Độ ẩm bình thường";
                }
            }
        }

        // Sau này có thể xử lý thêm page === "device", "setting", ...
    } catch (e) {
        console.warn("Không phải JSON hợp lệ:", event.data);
    }
}


// ==================== UI NAVIGATION ====================
let relayList = [];
let deleteTarget = null;

function showSection(id, event) {
    document.querySelectorAll('.section').forEach(sec => sec.style.display = 'none');
    document.getElementById(id).style.display = id === 'settings' ? 'flex' : 'block';
    document.querySelectorAll('.nav-item').forEach(i => i.classList.remove('active'));
    event.currentTarget.classList.add('active');
}


// ==================== HOME GAUGES ====================
// let gaugeTemp;
// let gaugeHumi;
// function initGauges() {
//     gaugeTemp = new JustGage({
//         id: "gauge_temp",
//         value: 0,
//         min: -10,
//         max: 50,
//         donut: true,
//         pointer: false,
//         gaugeWidthScale: 0.25,
//         gaugeColor: "transparent",
//         levelColorsGradient: true,
//         levelColors: ["#00BCD4", "#4CAF50", "#FFC107", "#F44336"]
//     });

//     gaugeHumi = new JustGage({
//         id: "gauge_humi",
//         value: 0,
//         min: 0,
//         max: 100,
//         donut: true,
//         pointer: false,
//         gaugeWidthScale: 0.25,
//         gaugeColor: "transparent",
//         levelColorsGradient: true,
//         levelColors: ["#42A5F5", "#00BCD4", "#0288D1"]
//     });
// }



// ==================== DEVICE FUNCTIONS ====================
function openAddRelayDialog() {
    document.getElementById('addRelayDialog').style.display = 'flex';
}
function closeAddRelayDialog() {
    document.getElementById('addRelayDialog').style.display = 'none';
}
function saveRelay() {
    const name = document.getElementById('relayName').value.trim();
    const gpio = document.getElementById('relayGPIO').value.trim();
    if (!name || !gpio) return alert("⚠️ Please fill all fields!");
    relayList.push({ id: Date.now(), name, gpio, state: false });
    renderRelays();
    closeAddRelayDialog();
}
function renderRelays() {
    const container = document.getElementById('relayContainer');
    container.innerHTML = "";
    relayList.forEach(r => {
        const card = document.createElement('div');
        card.className = 'device-card';
        card.innerHTML = `
      <i class="fa-solid fa-bolt device-icon"></i>
      <h3>${r.name}</h3>
      <p>GPIO: ${r.gpio}</p>
      <button class="toggle-btn ${r.state ? 'on' : ''}" onclick="toggleRelay(${r.id})">
        ${r.state ? 'ON' : 'OFF'}
      </button>
      <i class="fa-solid fa-trash delete-icon" onclick="showDeleteDialog(${r.id})"></i>
    `;
        container.appendChild(card);
    });
}
function toggleRelay(id) {
    const relay = relayList.find(r => r.id === id);
    if (relay) {
        relay.state = !relay.state;
        const relayJSON = JSON.stringify({
            page: "device",
            value: {
                name: relay.name,
                status: relay.state ? "ON" : "OFF",
                gpio: relay.gpio
            }
        });
        Send_Data(relayJSON);
        renderRelays();
    }
}
function showDeleteDialog(id) {
    deleteTarget = id;
    document.getElementById('confirmDeleteDialog').style.display = 'flex';
}
function closeConfirmDelete() {
    document.getElementById('confirmDeleteDialog').style.display = 'none';
}
function confirmDelete() {
    relayList = relayList.filter(r => r.id !== deleteTarget);
    renderRelays();
    closeConfirmDelete();
}


// ==================== SETTINGS FORM (BỔ SUNG) ====================
document.getElementById("settingsForm").addEventListener("submit", function (e) {
    e.preventDefault();

    const ssid = document.getElementById("ssid").value.trim();
    const password = document.getElementById("password").value.trim();
    const token = document.getElementById("token").value.trim();
    const server = document.getElementById("server").value.trim();
    const port = document.getElementById("port").value.trim();

    const settingsJSON = JSON.stringify({
        page: "setting",
        value: {
            ssid: ssid,
            password: password,
            token: token,
            server: server,
            port: port
        }
    });

    Send_Data(settingsJSON);
    alert("✅ Cấu hình đã được gửi đến thiết bị!");
});


// ==================== LED BRIGHTNESS CONTROLS ====================
function onBrightnessChange(target, value) {
    // target: 'builtin' hoặc 'neo'
    value = Number(value);

    // Cập nhật label trên UI
    let labelId =
        target === "builtin"
            ? "builtin_brightness_label"
            : "neo_brightness_label";

    const labelEl = document.getElementById(labelId);
    if (labelEl) {
        labelEl.textContent = value;
    }

    // Tạo JSON gửi xuống ESP32
    const msg = JSON.stringify({
        page: "home",
        value: {
            type: "brightness",
            target: target, // 'builtin' hoặc 'neo'
            level: value    // 0–255
        }
    });

    Send_Data(msg);
}
