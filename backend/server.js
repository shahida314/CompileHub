const express = require('express');
const cors = require('cors');
const path = require('path');
const http = require('http');
const WebSocket = require('ws');

// C, C++, Java, Python Runner Imports
const { runCCode, runCInteractive } = require('./c_hub/c_runner');
const { runCppCode, runCppInteractive } = require('./cpp_hub/cpp_runner');
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

// WebSocket Server for All Interactive Languages (C, C++, Java, Python)
wss.on('connection', (ws) => {
    let currentChild = null;

    ws.on('message', (message) => {
        let msg;
        try {
            msg = JSON.parse(message);
        } catch (e) {
            return;
        }

        // Python Interactive Session
        if (msg.type === 'start') {
            if (currentChild) {
                try { currentChild.kill(); } catch (e) { }
                currentChild = null;
            }
            runPythonInteractive(ws, msg.code);
        }
        // C Interactive Session
        else if (msg.type === 'start_c') {
            if (currentChild) {
                try { currentChild.kill(); } catch (e) { }
                currentChild = null;
            }
            currentChild = runCInteractive(ws, msg.code);
        }
        // C++ Interactive Session
        else if (msg.type === 'start_cpp') {
            if (currentChild) {
                try { currentChild.kill(); } catch (e) { }
                currentChild = null;
            }
            currentChild = runCppInteractive(ws, msg.code);
        }

        // Java Interactive Execution Logic
        if (msg.type === 'run') {
            if (currentChild) {
                try { currentChild.kill(); } catch (e) { }
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

        // Handle User Input in Terminal (scanf/cin/input)
        if (msg.type === 'input') {
            if (currentChild && currentChild.stdin && currentChild.stdin.writable) {
                currentChild.stdin.write(msg.data + '\n');
            }
        }

        // Handle Stop Execution
        if (msg.type === 'stop') {
            if (currentChild) {
                try { currentChild.kill(); } catch (e) { }
                currentChild = null;
                if (ws.readyState === ws.OPEN) {
                    ws.send(JSON.stringify({ type: 'exit', code: null, stopped: true }));
                }
            }
        }
    });

    ws.on('close', () => {
        if (currentChild) {
            try { currentChild.kill(); } catch (e) { }
            currentChild = null;
        }
    });
});

// ================= API ENDPOINTS ================= //

// C Compiler API Endpoint (Non-interactive fallback)
app.post('/api/compile/c', (req, res) => {
    const { code, input } = req.body;
    runCCode(code, input || '', (result) => {
        if (!res.headersSent) res.json(result);
    });
});

// Generic /run Endpoint & C++ Endpoint
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
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runJavaCompiler(code, (output) => {
        if (!res.headersSent) {
            res.json({ output });
        }
    });
});

// Python Compiler API Endpoint
app.post('/api/compile/python', (req, res) => {
    const { code } = req.body;

    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    runPythonCode(code, (result) => {
        if (!res.headersSent) res.json(result);
    });
});

// Start Server
const PORT = process.env.PORT || 5000;
server.listen(PORT, () => {
    console.log(`CompileHub Backend Server (HTTP + WS) running on port ${PORT}`);
});