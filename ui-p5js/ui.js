// ui.js

// Helper functions for sending commands
function sendCustomStatus(text) {
  if (!text || !text.trim().length) return;
  sendCommand("STATUS " + text.trim());
}

function sendEmoji(name) {
  sendCommand("EMOJI " + name);
}

function sendResetAll() {
  sendCommand("RESET_ALL");
}

// Update status chip highlights
function updateStatusChips(status) {
  const chipAvailable = document.getElementById("chip-available");
  const chipBusy = document.getElementById("chip-busy");
  
  if (chipAvailable && chipBusy) {
    // Remove active class from both
    chipAvailable.classList.remove("active");
    chipBusy.classList.remove("active");
    
    // Add active class to the current status
    if (status === "AVAILABLE") {
      chipAvailable.classList.add("active");
    } else if (status === "BUSY") {
      chipBusy.classList.add("active");
    }
  }
}

// Global callback for status events from serial.js
window.onStatusEvent = function(status) {
  updateStatusChips(status);
};

function setup() {
  // We don't need a canvas; p5's setup just ensures DOM is ready.
  noCanvas();

  const btnConnect = document.getElementById("btnConnect");
  const statusInput = document.getElementById("statusInput");
  const btnSendStatus = document.getElementById("btn-send-status");
  const btnEmojiSmile = document.getElementById("btn-emoji-smile");
  const btnEmojiNeutral = document.getElementById("btn-emoji-neutral");
  const btnEmojiBusy = document.getElementById("btn-emoji-busy");
  const btnEmojiSleep = document.getElementById("btn-emoji-sleep");
  const btnEmojiCheck = document.getElementById("btn-emoji-check");
  const btnEmojiWarn = document.getElementById("btn-emoji-warn");
  const btnResetAll = document.getElementById("btn-reset-all");

  // Connect button
  btnConnect.addEventListener("click", () => {
    connectSerial();
  });

  // Custom STATUS <text>
  btnSendStatus.addEventListener("click", () => {
    sendCustomStatus(statusInput.value);
  });

  statusInput.addEventListener("keydown", (e) => {
    if (e.key === "Enter") {
      btnSendStatus.click();
    }
  });

  // ---------------- EMOJI ----------------

  btnEmojiSmile.addEventListener("click", () => {
    sendEmoji("SMILE");
  });

  btnEmojiNeutral.addEventListener("click", () => {
    sendEmoji("NEUTRAL");
  });

  btnEmojiBusy.addEventListener("click", () => {
    sendEmoji("BUSY");
  });

  btnEmojiSleep.addEventListener("click", () => {
    sendEmoji("SLEEP");
  });

  btnEmojiCheck.addEventListener("click", () => {
    sendEmoji("CHECK");
  });

  btnEmojiWarn.addEventListener("click", () => {
    sendEmoji("WARN");
  });

  // ---------------- RESET_ALL ----------------

  btnResetAll.addEventListener("click", () => {
    sendResetAll();
  });
}

function draw() {
  // No canvas drawing needed at the moment.
}
