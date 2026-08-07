const express = require('express');
const cors = require('cors');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

<<<<<<< HEAD
// Language Runners Import
const { runCCode } = require('./c_hub/c_runner');
const { runCppCode } = require('./cpp_hub/cpp_runner'); 
const { runJavaCompiler } = require('./java_hub/java_runner');
const { runPythonInteractive } = require('./python_hub/python_runner');
=======
// C, C++, Java, Python Runner Imports
const { runCCode, runCInteractive } = require('./c_hub/c_runner');
const { runCppCode } = require('./cpp_hub/cpp_runner');
const { runJavaCompiler, runJavaCompilerInteractive } = require('./java_hub/java_runner');
const { runPythonCode, runPythonInteractive } = require('./python_hub/python_runner');
>>>>>>> 482762f823d50dcfdbacb90e9e44d91bd57eed31

const app = express();
app.use(cors());
app.use(express.json());

// Serve frontend static files
app.use(express.static(path.join(__dirname, '../frontend')));

// Create HTTP and WebSocket server
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// WebSocket Server for All Interactive Languages (C, Java, Python)
wss.on('connection', (ws) => {
    let currentChild = null;

    ws.on('message', (message) => {
<<<<<<< HEAD
        try {
            const data = JSON.parse(message);
            if (data.type === 'start') {
                runPythonInteractive(ws, data.code);
            }
        } catch (err) {
            ws.send(JSON.stringify({ type: 'error', data: 'Invalid JSON payload' }));
=======
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
>>>>>>> 482762f823d50dcfdbacb90e9e44d91bd57eed31
        }
    });
});

// ================= API ENDPOINTS ================= //

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code, input } = req.body;
    runCCode(code, input || '', (result) => {
        if (!res.headersSent) res.json(result);
    });
});

<<<<<<< HEAD
// Generic /run Endpoint & C++ Endpoint (যাতে /run এবং /api/compile/cpp দুটোতেই কাজ করে)
const handleCppExecution = (req, res) => {
    const { code, input } = req.body;
=======
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
>>>>>>> 482762f823d50dcfdbacb90e9e44d91bd57eed31

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    try {
        runCppCode(code, input || '', (result) => {
            if (res.headersSent) return;

            if (!result) {
                return res.status(500).json({ error: 'Execution failed to return a result.' });
            }

            return res.json(result);
        });
    } catch (error) {
        if (!res.headersSent) {
            return res.status(500).json({ error: 'Internal Server Error: ' + error.message });
        }
    }
};

app.post('/run', handleCppExecution);
app.post('/api/compile/cpp', handleCppExecution);

// Java Code Execution Endpoint
app.post('/api/compile/java', (req, res) => {
    const { code, input } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runJavaCompiler(code, input || '', (output) => {
        if (!res.headersSent) {
            res.json({ output });
        }
    });
});

<<<<<<< HEAD
// Start Server
const PORT = process.env.PORT || 5000;
server.listen(PORT, () => {
    console.log(`CompileHub Backend Server running on port ${PORT}`);
});
=======
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
>>>>>>> 482762f823d50dcfdbacb90e9e44d91bd57eed31
