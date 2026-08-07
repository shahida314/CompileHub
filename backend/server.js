const express = require('express');
const cors = require('cors');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

// C, C++, Java, Python Runner Imports
const { runCCode, runCInteractive } = require('./c_hub/c_runner');
const { runCppCode } = require('./cpp_hub/cpp_runner');
const { runJavaCompiler, runJavaCompilerInteractive } = require('./java_hub/java_runner');
const { runPythonCode, runPythonInteractive } = require('./python_hub/python_runner');

const app = express();
app.use(cors());
app.use(express.json());

// Serve frontend static files
app.use(express.static(path.join(__dirname, '../frontened')));

// Create HTTP and WebSocket server
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// WebSocket Server for All Interactive Languages (C, Java, Python)
wss.on('connection', (ws) => {
    let currentChild = null;

    ws.on('message', (message) => {
        let msg;
        try {
            msg = JSON.parse(message);
        } catch (e) {
            return;
        }

        // C & Python Start Interactive Sessions
        if (msg.type === 'start') {
            runPythonInteractive(ws, msg.code);
        } else if (msg.type === 'start_c') {
            runCInteractive(ws, msg.code);
        }

        // Java Interactive Execution Logic
        if (msg.type === 'run') {
            if (currentChild) {
                currentChild.kill();
                currentChild = null;
            }

            currentChild = runJavaCompilerInteractive(msg.code, {
                onOutput: (data) => {
                    if (ws.readyState === ws.OPEN) ws.send(JSON.stringify({ type: 'output', data }));
                },
                onError: (data) => {
                    if (ws.readyState === ws.OPEN) ws.send(JSON.stringify({ type: 'error', data }));
                },
                onExit: (code) => {
                    if (ws.readyState === ws.OPEN) ws.send(JSON.stringify({ type: 'exit', code }));
                    currentChild = null;
                }
            });
        }

        // Handle User Input in Terminal
        if (msg.type === 'input') {
            if (currentChild && currentChild.stdin && currentChild.stdin.writable) {
                currentChild.stdin.write(msg.data + '\n');
            }
        }

        // Handle Stop Execution
        if (msg.type === 'stop') {
            if (currentChild) {
                currentChild.kill();
                currentChild = null;
                if (ws.readyState === ws.OPEN) {
                    ws.send(JSON.stringify({ type: 'exit', code: null, stopped: true }));
                }
            }
        }
    });

    ws.on('close', () => {
        if (currentChild) {
            currentChild.kill();
            currentChild = null;
        }
    });
});

// ================= API ENDPOINTS ================= //

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code } = req.body;
    runCCode(code, (result) => {
        res.json(result);
    });
});

// C++ Code Execution Endpoint
app.post('/api/compile/cpp', (req, res) => {
    const { code, input } = req.body;
    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }
    runCppCode(code, input, (result) => {
        res.json(result);
    });
});

// Java Code Execution Endpoint
app.post('/api/compile/java', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runJavaCompiler(code, (output) => {
        res.json({ output });
    });
});

// Python Compiler API Endpoint
app.post('/api/compile/python', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runPythonCode(code, (result) => {
        res.json(result);
    });
});

// Server Listen
const PORT = process.env.PORT || 5000;
server.listen(PORT, () => console.log(`Backend Server (HTTP + WS) listening on port ${PORT}`));