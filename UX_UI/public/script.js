// public/script.js
const socket = io(); // เชื่อม WebSocket

socket.on('serial-data', (data) => {
  const content = document.getElementById('content');
  const p = document.createElement('p');
  p.textContent = data;
  content.prepend(p); // แสดงค่าใหม่ด้านบน
});
