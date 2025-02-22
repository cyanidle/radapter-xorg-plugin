import { createWebSocket } from "./ws.js";

const mainZone = document.getElementById('mainZone');
const hiddenInput = document.getElementById('hiddenInput');
const inputActivator = document.getElementById('keyboardButton');
const isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);

// Mouse state with debouncing
let mouseState = { x: 0, y: 0, scroll: 0, dirty: false };
let startTouchY = 0;
let lastTouchY = 0;
let startTouchX = 0;
let startTouchTS = 0;
let maxTouchCount = 0;
let lastMouseX = -1;
let lastMouseY = -1;

// Update interval for mouse handler
function checkMouseState() {
  if (mouseState.dirty) {
    if (lastMouseX < 0) {lastMouseX = mouseState.x;}
    if (lastMouseY < 0) {lastMouseY = mouseState.y;}
    handleMouse(
      mouseState.x - lastMouseX,
      mouseState.y - lastMouseY,
      mouseState.scroll,
    );
    lastMouseX = mouseState.x;
    lastMouseY = mouseState.y;
    mouseState.scroll = 0;
    mouseState.dirty = false;
  }
}
let lastUpdate = new Date()
let updatePending = false;
var updateRate = 20
function tryUpdateMouse() {
  const now = new Date();
  const diff = now - lastUpdate;
  if (diff < updateRate) {
    if (!updatePending) {
      updatePending = true;
      setTimeout(tryUpdateMouse, updateRate - diff);
    }
    return;
  }
  updatePending = false;
  lastUpdate = now;
  checkMouseState();
}

if (isMobile) {
  inputActivator.style.display = 'flex';
  inputActivator.addEventListener('click', () => {
    hiddenInput.focus();
  });
  document.getElementById('leftClickButton').addEventListener('touchend', (e) => {
    handleKeyboard("lmb");
  });
  document.getElementById('rightClickButton').addEventListener('touchend', (e) => {
    handleKeyboard("rmb");
  });
} else {
  inputActivator.style.display = 'none';
  hiddenInput.focus();
}

function updateTouchCount(event) {
  maxTouchCount = maxTouchCount > event.touches.length ? maxTouchCount : event.touches.length;
}

// Pointer event handlers
function handlePointerStart(event) {
  event.preventDefault();

  lastMouseX = -1;
  lastMouseY = -1;

  if (!isMobile) hiddenInput.focus();

  if (event.type === 'touchstart') {
    if (event.touches.length === 1) {
      const touch = event.touches[0];
      startTouchY = touch.clientY;
      startTouchX = touch.clientX;
      updateMousePosition(touch);
    } else if (event.touches.length === 2) {
      const finalY = (event.touches[0].clientY + event.touches[1].clientY) / 2;
      lastTouchY = finalY;
    }
    startTouchTS = new Date();
  } else {
    updateMousePosition(event);
  }
}

function handlePointerMove(event) {
  event.preventDefault();

  if (event.type === 'touchmove') {
    updateTouchCount(event);
    if (event.touches.length === 2) {
      const finalY = (event.touches[0].clientY + event.touches[1].clientY) / 2;
      mouseState.scroll += lastTouchY - finalY;
      lastTouchY = finalY;
      mouseState.dirty = true;
      tryUpdateMouse();
    } else {
      updateMousePosition(event.touches[0]);
    }
  } else {
    updateMousePosition(event);
  }

}

function handlePointerEnd(event) {
  event.preventDefault();
  if (event.type === 'touchend') {
    if (maxTouchCount == 1) {
      const timeDiff = new Date() - startTouchTS;
      const diffX = mouseState.x - startTouchX
      const diffY = mouseState.y - startTouchY
      const smaller = window.innerWidth < window.innerHeight ? window.innerWidth : window.innerHeight;
      if (timeDiff < 200 && Math.sqrt(diffX * diffX + diffY * diffY) < (smaller * 0.07)) {
        handleKeyboard("lmb");
      }
    }
    maxTouchCount = 0;
  }
}

// Update mouse position coordinates
function updateMousePosition(source) {
  mouseState.x = source.clientX;
  mouseState.y = source.clientY;
  mouseState.dirty = true;
  tryUpdateMouse();
}

// Wheel handler
mainZone.addEventListener('wheel', (event) => {
  event.preventDefault();
  mouseState.scroll += event.deltaY;
  mouseState.dirty = true;
  tryUpdateMouse();
});

const ws = createWebSocket(`ws://${window.location.hostname}:11337`)

// Keyboard handler
let was = "";
hiddenInput.onkeyup = (ev) => {
  if (ev.keyCode == 8) {
    handleKeyboard("backspace");
  }
  if (ev.keyCode == 13) {
    handleKeyboard("enter");
  }
}
hiddenInput.oninput = (ev) => {
  const now = ev.target.value;
  let deleted = was.length - now.length;
  if (deleted > 0) {
    was = was.substring(0, was.length - deleted);
    while (deleted-- != 0) {
      handleKeyboard("backspace");
    }
  }
  for (let i = was.length; i < now.length; ++i) {
    handleKeyboard(now[i])
  }
  was = now;
}

// User-provided handlers
function handleMouse(dx, dy, scroll) {
  ws.send({mouse: {dx, dy, scroll}})
}

function handleKeyboard(key) {
    ws.send({keyboard: {key}})
}

// Event listeners
mainZone.addEventListener('mousedown', handlePointerStart);
mainZone.addEventListener('mousemove', handlePointerMove);
mainZone.addEventListener('mouseup', handlePointerEnd);

mainZone.addEventListener('touchstart', handlePointerStart);
mainZone.addEventListener('touchmove', handlePointerMove);
mainZone.addEventListener('touchend', handlePointerEnd);
////