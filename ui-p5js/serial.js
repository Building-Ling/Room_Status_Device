// serial.js

let port = null;
let writer = null;
let reader = null;
let serialConnected = false;

async function connectSerial() {
  if (!("serial" in navigator)) {
    alert("WebSerial not supported. Use Chrome or Edge.");
    return;
  }

  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });

    const textEncoder = new TextEncoderStream();
    textEncoder.readable.pipeTo(port.writable);
    writer = textEncoder.writable.getWriter();

    const textDecoder = new TextDecoderStream();
    port.readable.pipeTo(textDecoder.writable);
    reader = textDecoder.readable.getReader();

    serialConnected = true;
    updateStatusText("Connected");
    logLine("Serial connected.");

    // Start read loop
    readLoop();
  } catch (err) {
    console.error(err);
    alert("Failed to open serial port: " + err);
  }
}

async function readLoop() {
  try {
    let buffer = "";
    while (serialConnected && reader) {
      const { value, done } = await reader.read();
      if (done) break;
      if (value) {
        buffer += value;
        // Process complete lines
        const lines = buffer.split('\n');
        buffer = lines.pop() || ""; // Keep incomplete line in buffer
        
        for (const line of lines) {
          const trimmed = line.trim();
          if (trimmed) {
            // Append Arduino text to log
            logLine(trimmed);
            
            // Check for status events and map firmware output to UI status
            if (typeof window.onStatusEvent === 'function') {
              if (trimmed === "EVENT STATUS AVAILABLE") {
                window.onStatusEvent("AVAILABLE");
              } else if (trimmed === "EVENT STATUS BUSY") {
                window.onStatusEvent("BUSY");
              } else if (trimmed === "AVAILABLE") {
                window.onStatusEvent("AVAILABLE");
              } else if (trimmed === "BUSY") {
                window.onStatusEvent("BUSY");
              } else if (trimmed === "Received: AVAILABLE") {
                window.onStatusEvent("AVAILABLE");
              } else if (trimmed === "Received: BUSY") {
                window.onStatusEvent("BUSY");
              }
            }
          }
        }
      }
    }
  } catch (err) {
    console.error(err);
    logLine("Error reading: " + err);
  } finally {
    serialConnected = false;
    updateStatusText("Disconnected");
    logLine("Serial disconnected.");
  }
}

async function sendCommand(cmd) {
  if (!serialConnected || !writer) {
    alert("Serial not connected.");
    return;
  }

  try {
    const line = cmd.trim() + "\n";
    await writer.write(line);
    logLine("> " + cmd); // echo sent command
  } catch (err) {
    console.error(err);
    logLine("Error sending: " + err);
  }
}

// Helper to update connection status text in UI
function updateStatusText(msg) {
  const el = document.getElementById("statusText");
  if (el) el.textContent = msg;
}

// Helper to append log lines
function logLine(text) {
  const log = document.getElementById("log");
  if (!log) return;

  log.value += text;
  if (!text.endsWith("\n")) log.value += "\n";
  log.scrollTop = log.scrollHeight;
}
