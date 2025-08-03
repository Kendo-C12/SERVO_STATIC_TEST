// server.js
const express = require('express');
const path = require('path');
const http = require('http');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const { Server } = require('socket.io');

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = 3000;

// ===== Serve Static Files =====
app.use(express.static(path.join(__dirname, 'public')));

// ===== Serial Port Setup =====
// ✅ เปลี่ยน COM3 เป็นพอร์ตจริงของ Arduino
const serial = new SerialPort({ path: 'COM13', baudRate: 115200 });
const parser = serial.pipe(new ReadlineParser({ delimiter: '\n' }));

const sqlite3 = require('sqlite3').verbose();

// === สร้าง / เชื่อมต่อฐานข้อมูล
const db = new sqlite3.Database('sensor_data.db');

// === สร้างตารางถ้ายังไม่มี
db.run(`
  CREATE TABLE IF NOT EXISTS sensor (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    temperature REAL,
    humidity INTEGER,
    light INTEGER,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
  )
`);

parser.on('data', (line) => {
  const trimmed = line.trim();
  console.log('📡 Serial CSV:', trimmed);

  // แยกข้อมูลจาก CSV เช่น "25.7,58,814"
  const parts = trimmed.split(',');

  // ตรวจว่ามีครบ 3 ค่า
  if (parts.length === 3) {
    const [time, height, deploy_yet] = parts;

    // บันทึกลงฐานข้อมูล
    db.run(
      `INSERT INTO sensor (time, height, deploy_yet) VALUES (?, ?, ?)`,
      [time, parseInt(height), deploy_yet === 'true'],
      (err) => {
        if (err) {
          console.error('❌ DB Error:', err.message);
        } else {
          console.log('✅ Data inserted:', temperature, humidity, light);
        }
      }
    );
  }

  // ส่งข้อมูลให้หน้าเว็บด้วย
  io.emit('serial-data', trimmed);

  document.getElementById('output').textContent = line;

  const value = parts[0]; // ดึงค่าแรก เช่น "25.3"
  updateGraph(value);
});


// ===== Socket.io Connection =====
io.on('connection', (socket) => {
  console.log('🌐 Web client connected');
});

// ===== Start Server =====
server.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
