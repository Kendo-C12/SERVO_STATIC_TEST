// public/script.js
const socket = io(); // เชื่อม WebSocket
// const ctx = document.getElementById('dataChart').getContext('2d');

socket.on('serial-data', (data) => {
  document.getElementById('output').textContent =
    `time: ${data.time}s | height: ${data.height}m | velocity: ${data.velocity}m/s | acceleration: ${data.acceleration}m/s² | deploy: ${data.deploy_yet}`;

});

// สร้างกราฟเส้นเปล่า
// function createChart(ctx, label, color, yMax) {
//   return new Chart(ctx, {
//     type: 'line',
//     data: {
//       labels: [],
//       datasets: [{
//         label,
//         data: [],
//         fill: false,
//         borderColor: color,
//         tension: 0.1
//       }]
//     },
//     options: {
//       animation: false,
//       scales: {
//         x: {
//           title: {
//             display: true,
//             text: 'เวลา (s)'
//           }
//         },
//         y: {
//           title: {
//             display: true,
//             text: label
//           },
//           min: 0,
//           max: yMax
//         }
//       }
//     }
//   });
// }

// ฟังก์ชันเพิ่มข้อมูลลงกราฟ (คุณเรียกจาก parser.on('data') ได้)

// ✅ สร้าง chart 3 ตัว
// const heightChart = createChart(ctxHeight, 'ความสูง', 'rgb(75, 192, 192)', 7000);
// const velocityChart = createChart(ctxVelocity, 'ความเร็ว', 'rgb(255, 159, 64)', 500);
// const accelerationChart = createChart(ctxAcceleration, 'ความเร่ง', 'rgb(255, 99, 132)', 100);

// function updateChart(chart, value) {
//   const now = new Date().toLocaleTimeString();
//   chart.data.labels.push(now);
//   chart.data.datasets[0].data.push(parseFloat(value));

//   if (chart.data.labels.length > 20) {
//     chart.data.labels.shift();
//     chart.data.datasets[0].data.shift();
//   }

//   chart.update();
// }

// // ✅ ฟัง WebSocket แล้วอัปเดตทุกกราฟ
// socket.on('serial-data', (data) => {
//   document.getElementById('output').textContent =
//     `time: ${data.time}s | height: ${data.height}m | velocity: ${data.velocity}m/s | acceleration: ${data.acceleration}m/s² | prediction_height: ${data.prediction_height}m | deploy: ${data.deploy_yet}`;
// }
//   updateChart(heightChart, data.height);
//   updateChart(velocityChart, data.velocity);
//   updateChart(accelerationChart, data.acceleration);
// });

document.getElementById('rotateBtn').addEventListener('click', () => {
  socket.emit('rotate-command', 'rotate');
});
