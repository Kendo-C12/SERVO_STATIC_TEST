const express = require('express');
const path = require('path');
const http = require('http');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const { Server } = require('socket.io');
const sqlite3 = require('sqlite3').verbose();

const app = express();
const server = http.createServer(app);
const io = new Server(server);

const PORT = 3000;

app.use(express.static(path.join(__dirname, 'public')));

const serial = new SerialPort({ path: 'COM13', baudRate: 9600 });
const parser = serial.pipe(new ReadlineParser({ delimiter: '\n' }));

const db = new sqlite3.Database('sensor_data.db');

// ✅ สร้างตารางใหม่ให้ตรงกับข้อมูล 6 ค่า
db.run(`
  CREATE TABLE IF NOT EXISTS sensor (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    time REAL,
    height REAL,
    velocity REAL,
    acceleration REAL,
    prediction_height REAL,
    deploy_yet INTEGER,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
  )
`);

parser.on('data', (line) => {
  const trimmed = line.trim();
  console.log('📡 Serial CSV:', trimmed);

  const parts = trimmed.split(',');

  if (parts.length === 6) {
    const [
      time,
      height,
      velocity,
      acceleration,
      prediction_height,
      deploy_yet
    ] = parts;

    // ✅ บันทึกลงฐานข้อมูล
    db.run(
      `INSERT INTO sensor (time, height, velocity, acceleration, prediction_height, deploy_yet)
       VALUES (?, ?, ?, ?, ?, ?)`,
      [
        parseFloat(time),
        parseFloat(height),
        parseFloat(velocity),
        parseFloat(acceleration),
        parseFloat(prediction_height),
        parseInt(deploy_yet, 10)
      ],
      (err) => {
        if (err) {
          console.error('❌ DB Error:', err.message);
        } else {
          console.log('✅ Inserted:', parts);
        }
      }
    );

    // ✅ ส่งให้หน้าเว็บ
    io.emit('serial-data', {
      time: parseFloat(time),
      height: parseFloat(height),
      velocity: parseFloat(velocity),
      acceleration: parseFloat(acceleration),
      prediction_height: parseFloat(prediction_height),
      deploy_yet: parseInt(deploy_yet, 10)
    });
  } else {
    console.warn('⚠️ ข้อมูลไม่ครบ 6 ค่า:', parts);
  }
});

// io.on('connection', (socket) => {
//   console.log('🌐 Web client connected');
// });

io.on('connection', (socket) => {
  console.log('🌐 Web client connected');

  socket.on('rotate-command', (msg) => {
    console.log('🔄 Received command from client:', msg);

    // คุณสามารถส่งคำสั่งนี้ออก serial port ได้ เช่น:
    if (serial && serial.writable) {
      serial.write('ROTATE\n'); // หรือ rotate\r\n ตามอุปกรณ์
    }
  });
});


server.listen(PORT, () => {
  console.log(`🚀 Server running at http://localhost:${PORT}`);
});
