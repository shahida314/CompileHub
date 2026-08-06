const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

// Interactive Python runner for WebSockets
function runPythonInteractive(ws, code) {
    const tempDir = path.join(__dirname, 'tests');
    if (!fs.existsSync(tempDir)) {
        fs.mkdirSync(tempDir, { recursive: true });
    }

    const tempPath = path.join(tempDir, `temp_${Date.now()}.py`);
    fs.writeFileSync(tempPath, code);

    // Use -u flag for unbuffered output
    const pythonProcess = spawn('python', ['-u', tempPath]);

    // Send stdout data to client live
    pythonProcess.stdout.on('data', (data) => {
        ws.send(JSON.stringify({ type: 'output', text: data.toString() }));
    });

    // Send stderr data to client live
    pythonProcess.stderr.on('data', (data) => {
        ws.send(JSON.stringify({ type: 'error', text: data.toString() }));
    });

    // Clean up temp file and notify client on close
    pythonProcess.on('close', (codeExit) => {
        if (fs.existsSync(tempPath)) fs.unlinkSync(tempPath);
        ws.send(JSON.stringify({ type: 'exit', code: codeExit }));
    });

    // Handle user input from frontend terminal
    ws.on('message', (message) => {
        try {
            const msg = JSON.parse(message);
            if (msg.type === 'input') {
                pythonProcess.stdin.write(msg.text + '\n');
            }
        } catch (err) {
            console.error('Invalid message format', err);
        }
    });
}

module.exports = { runPythonInteractive };