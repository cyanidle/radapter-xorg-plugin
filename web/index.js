const mainZone = document.getElementById('mainZone');
const hiddenInput = document.getElementById('hiddenInput');
const inputActivator = document.getElementById('inputActivator');
const isMobile = /iPhone|iPad|iPod|Android/i.test(navigator.userAgent);

// Mouse state with debouncing
let mouseState = { x: 0, y: 0, scrollX: 0, scrollY: 0, dirty: false };
let lastTouchY = 0;

// Update interval for mouse handler
setInterval(() => {
  if (mouseState.dirty) {
    handleMouse(
      mouseState.x,
      mouseState.y,
      { x: mouseState.scrollX, y: mouseState.scrollY }
    );
    mouseState.scrollX = 0;
    mouseState.scrollY = 0;
    mouseState.dirty = false;
  }
}, 40);

// Mobile-specific setup
if (isMobile) {
  inputActivator.style.display = 'flex';
  inputActivator.addEventListener('click', () => {
    hiddenInput.focus();
    inputActivator.style.transform = 'translateX(-50%) scale(0.9)';
    setTimeout(() => {
      inputActivator.style.transform = 'translateX(-50%)';
    }, 100);
  });
} else {
  hiddenInput.focus();
  inputActivator.style.display = 'none';
}

// Pointer event handlers
function handlePointerStart(event) {
  event.preventDefault();
  if (!isMobile) hiddenInput.focus();

  if (event.type === 'touchstart') {
    lastTouchY = event.touches[0].clientY;
    updateMousePosition(event.touches[0]);
  } else {
    updateMousePosition(event);
  }
}

function handlePointerMove(event) {
  event.preventDefault();

  if (event.type === 'touchmove') {
    const touch = event.touches[0];
    updateMousePosition(touch);
    const deltaY = lastTouchY - touch.clientY;
    mouseState.scrollY += deltaY;
    lastTouchY = touch.clientY;
    mouseState.dirty = true;
  } else {
    updateMousePosition(event);
  }

  mouseState.dirty = true;
}

function handlePointerEnd(event) {
  event.preventDefault();
}

// Update mouse position coordinates
function updateMousePosition(source) {
  mouseState.x = source.clientX;
  mouseState.y = source.clientY;
}

// Wheel handler
mainZone.addEventListener('wheel', (event) => {
  event.preventDefault();
  mouseState.scrollX += event.deltaX;
  mouseState.scrollY += event.deltaY;
  mouseState.dirty = true;
});

// Keyboard handler
hiddenInput.addEventListener('keydown', (event) => {
  handleKeyboard(event.key, true);
  hiddenInput.value = '';
});

// Event listeners
mainZone.addEventListener('mousedown', handlePointerStart);
mainZone.addEventListener('mousemove', handlePointerMove);
mainZone.addEventListener('mouseup', handlePointerEnd);

mainZone.addEventListener('touchstart', handlePointerStart);
mainZone.addEventListener('touchmove', handlePointerMove);
mainZone.addEventListener('touchend', handlePointerEnd);

////

import { createWebSocket } from "./ws.js";

const ws = createWebSocket(`ws://${window.location.host}:11337`)

// User-provided handlers
function handleMouse(x, y, scroll) {
    ws.send({mouse: {x, y, scroll}})
}

function handleKeyboard(key) {
    ws.send({keyboard: {key}})
}