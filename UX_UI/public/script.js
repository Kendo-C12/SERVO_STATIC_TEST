// public/script.js
const socket = io(); // เชื่อม WebSocket
const ctx = document.getElementById('dataChart').getContext('2d');

socket.on('serial-data', (data) => {
  const content = document.getElementById('content');
  const p = document.createElement('p');
  p.textContent = data;
  content.prepend(p); // แสดงค่าใหม่ด้านบน
});

// สร้างกราฟเส้นเปล่า
const chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [], // เวลา
        datasets: [{
            label: 'ค่าที่รับจาก Serial',
            data: [],
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    },
    options: {
        animation: false,
        scales: {
            x: {
                title: {
                    display: true,
                    text: 'เวลา (s)'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'ค่า'
                }
            }
        }
    }
});

// ฟังก์ชันเพิ่มข้อมูลลงกราฟ (คุณเรียกจาก parser.on('data') ได้)
function updateGraph(value) {
    const now = new Date().toLocaleTimeString();
    chart.data.labels.push(now);
    chart.data.datasets[0].data.push(parseFloat(value));

    if (chart.data.labels.length > 20) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
    }

    chart.update();
}