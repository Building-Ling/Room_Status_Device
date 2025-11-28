// ui.js

function setup() {
  // We don't need a canvas; p5's setup just ensures DOM is ready.
  noCanvas();

  const btnConnect     = document.getElementById("btnConnect");
  const btnAvailable   = document.getElementById("btnAvailable");
  const btnBusy        = document.getElementById("btnBusy");
  const btnMeeting     = document.getElementById("btnMeeting");

  const statusInput    = document.getElementById("statusInput");
  const btnSendStatus  = document.getElementById("btnSendStatus");

  const timerModeSel   = document.getElementById("timerMode");
  const timerMinutes   = document.getElementById("timerMinutes");
  const btnStartTimer  = document.getElementById("btnStartTimer");
  const btnStopTimer   = document.getElementById("btnStopTimer");

  const emojiSelect    = document.getElementById("emojiSelect");
  const btnEmoji       = document.getElementById("btnEmoji");

  const btnClear       = document.getElementById("btnClear");

  // Connect button
  btnConnect.addEventListener("click", () => {
    connectSerial();
  });

  // ---------------- STATUS (no timers attached) ----------------

  btnAvailable.addEventListener("click", () => {
    // purely status; timer is separate
    sendCommand("AVAILABLE");
  });

  btnBusy.addEventListener("click", () => {
    sendCommand("BUSY");
  });

  btnMeeting.addEventListener("click", () => {
    sendCommand("MEETING");
  });

  // Custom STATUS <text>
  btnSendStatus.addEventListener("click", () => {
    const txt = statusInput.value.trim();
    if (!txt.length) return;
    sendCommand("STATUS " + txt);
  });

  statusInput.addEventListener("keydown", (e) => {
    if (e.key === "Enter") {
      btnSendStatus.click();
    }
  });

  // ---------------- TIMER (fully decoupled) ----------------

  btnStartTimer.addEventListener("click", () => {
    const mode = timerModeSel.value; // "UP" or "DOWN"
    let mins = parseInt(timerMinutes.value, 10);
    if (isNaN(mins) || mins <= 0) {
      mins = (mode === "UP") ? 25 : 30; // sane defaults
      timerMinutes.value = mins;
    }

    if (mode === "UP") {
      // TIMER UP <minutes>
      sendCommand("TIMER UP " + mins);
    } else {
      // TIMER DOWN <minutes>
      sendCommand("TIMER DOWN " + mins);
    }
  });

  btnStopTimer.addEventListener("click", () => {
    // TIMER STOP stops any active timer
    sendCommand("TIMER STOP");
  });

  // ---------------- EMOJI ----------------

  btnEmoji.addEventListener("click", () => {
    const value = emojiSelect.value;
    sendCommand("EMOJI " + value);
  });

  // ---------------- CLEAR ----------------

  btnClear.addEventListener("click", () => {
    sendCommand("CLEAR");
  });
}

function draw() {
  // No canvas drawing needed at the moment.
}
