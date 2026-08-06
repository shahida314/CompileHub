const express = require('express');
const cors = require('cors');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

const { runCCode } = require('./c_hub/c_runner');
const { runCppCode } = require('./cpp_hub/cpp_runner');
const { runJavaCompiler } = require('./java_hub/java_runner');
const { runPythonInteractive } = require('./python_hub/python_runner');

const app = express();
app.use(cors());
app.use(express.json());

// Serve frontend static files
app.use(express.static(path.join(__dirname, '../frontened')));

// Create HTTP and WebSocket server for Real-time Python
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Handle WebSocket connection for real-time Python input/output
wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        const data = JSON.parse(message);
        if (data.type === 'start') {
            runPythonInteractive(ws, data.code);
        }
    });
});

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code } = req.body;
    runCCode(code, (result) => {
        res.json(result);
    });
});

// C++ Code Execution Endpoint 
app.post('/api/compile/cpp', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runCppCode(code, (result) => {
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

const PORT = process.env.PORT || 5000;
server.listen(PORT, () => console.log(`Backend Server listening on port ${PORT}`));