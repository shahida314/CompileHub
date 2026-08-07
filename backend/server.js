const express = require('express');
const cors = require('cors');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

// Language Runners Import
const { runCCode } = require('./c_hub/c_runner');
const { runCppCode } = require('./cpp_hub/cpp_runner'); 
const { runJavaCompiler } = require('./java_hub/java_runner');
const { runPythonInteractive } = require('./python_hub/python_runner');

const app = express();
app.use(cors());
app.use(express.json());

// Serve frontend static files
app.use(express.static(path.join(__dirname, '../frontend')));

// Create HTTP and WebSocket server for Real-time Python
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Handle WebSocket connection for real-time Python input/output
wss.on('connection', (ws) => {
    ws.on('message', (message) => {
        try {
            const data = JSON.parse(message);
            if (data.type === 'start') {
                runPythonInteractive(ws, data.code);
            }
        } catch (err) {
            ws.send(JSON.stringify({ type: 'error', data: 'Invalid JSON payload' }));
        }
    });
});

// C Compiler API Endpoint
app.post('/api/compile/c', (req, res) => {
    const { code, input } = req.body;
    runCCode(code, input || '', (result) => {
        if (!res.headersSent) res.json(result);
    });
});

// Generic /run Endpoint & C++ Endpoint (যাতে /run এবং /api/compile/cpp দুটোতেই কাজ করে)
const handleCppExecution = (req, res) => {
    const { code, input } = req.body;

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

// Start Server
const PORT = process.env.PORT || 5000;
server.listen(PORT, () => {
    console.log(`CompileHub Backend Server running on port ${PORT}`);
});