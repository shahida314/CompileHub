const { spawn, execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// Run C code interactively via WebSocket
function runCInteractive(ws, code) {
    // Generate unique names to prevent file locking issues
    const uniqueId = Date.now() + Math.random();
    const filePath = path.join(__dirname, `temp_main_${uniqueId}.c`);
    const outPath = path.join(__dirname, `temp_program_${uniqueId}.exe`);

    try {
        // Automatically inject unbuffered stdout to fix printf/scanf buffering
        let modifiedCode = code;
        if (modifiedCode.includes('int main(')) {
            modifiedCode = modifiedCode.replace(
                'int main(',
                '#include <stdio.h>\n__attribute__((constructor)) void auto_flush() { setvbuf(stdout, NULL, _IONBF, 0); setvbuf(stderr, NULL, _IONBF, 0); }\nint main('
            );
        }

        // Save code to temp file and compile with gcc
        fs.writeFileSync(filePath, modifiedCode);
        execSync(`gcc "${filePath}" -o "${outPath}"`, { stdio: 'pipe' });
    } catch (err) {
        if (ws.readyState === ws.OPEN) {
            const errorMsg = err.stderr ? err.stderr.toString() : (err.message || 'Compilation Error');
            ws.send(JSON.stringify({ type: 'error', data: errorMsg.replace(/\n/g, '<br>') }));
            ws.send(JSON.stringify({ type: 'close', exitCode: 1 }));
        }
        cleanup(filePath, outPath);
        return;
    }

    // Spawn compiled executable process
    const child = spawn(outPath, [], { stdio: ['pipe', 'pipe', 'pipe'] });

    // Send program output to frontend
    child.stdout.on('data', (data) => {
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'output', data: data.toString() }));
        }
    });

    // Send errors to frontend
    child.stderr.on('data', (data) => {
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'error', data: data.toString() }));
        }
    });

    // Handle user inputs (scanf) or stop commands from frontend
    ws.on('message', (message) => {
        try {
            const msg = JSON.parse(message);
            if (msg.type === 'input') {
                child.stdin.write(msg.data + '\n');
            } else if (msg.type === 'stop') {
                child.kill();
            }
        } catch (e) { }
    });

    // Clean up files when process closes
    child.on('close', (exitCode) => {
        cleanup(filePath, outPath);
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'close', exitCode }));
        }
    });
}

// Delete temporary source and executable files safely
function cleanup(filePath, outPath) {
    setTimeout(() => {
        try {
            if (fs.existsSync(filePath)) fs.unlinkSync(filePath);
            if (fs.existsSync(outPath)) fs.unlinkSync(outPath);
        } catch (e) { }
    }, 500);
}

module.exports = { runCInteractive };