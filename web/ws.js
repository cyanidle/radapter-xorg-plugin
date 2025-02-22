export function createWebSocket(url) {
    let socket = null;
    let reconnectAttempts = 0;
    let isManualClose = false;
    let callback;
    const maxReconnectDelay = 30000; // 30 seconds max delay

    function connect() {
        if (isManualClose) return;

        socket = new WebSocket(url);

        socket.addEventListener('open', () => {
            reconnectAttempts = 0;
            console.log(`Connected to ${url}`);
        });

        socket.addEventListener('message', (event) => {
            if (!callback) {
                console.warn("Callback not ready!")
                return;
            }
            try {
                const raw = event.data;
                console.debug(raw)
                const data = JSON.parse(raw);
                callback(null, data);
            } catch (error) {
                callback(new Error('Failed to parse JSON message'), null);
            }
        });

        socket.addEventListener('close', (event) => {
            if (isManualClose) return;
            
            const reconnectDelay = Math.min(
                1000 * Math.pow(2, reconnectAttempts), // Exponential backoff
                maxReconnectDelay
            );
            
            console.log(`Connection closed. Reconnecting in ${reconnectDelay/1000} seconds...`);
            setTimeout(connect, reconnectDelay);
            reconnectAttempts++;
        });

        socket.addEventListener('error', (error) => {
            if (!callback) {
                console.warn("Callback not ready!")
                return;
            }
            callback(new Error(`WebSocket error: ${error.message}`), null);
        });
    }

    // Start initial connection 
    connect();

    // Return cleanup function
    return {
        close: () => {
            isManualClose = true;
            if (socket) {
                socket.close();
            }
        },
        send: (msg) => {
            if (socket && socket?.readyState === WebSocket.OPEN) {
                socket.send(JSON.stringify(msg));
            } else {
                throw new Error("Socket not connected");
            }
        },
        onmsg: (handler) => {
            callback = handler
        },
        reconnect: () => {
            if (!isManualClose && socket?.readyState !== WebSocket.OPEN) {
                reconnectAttempts = 0;
                connect();
            }
        }
    };
}


