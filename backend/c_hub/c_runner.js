const { spawn, execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

// Run C code interactively via WebSocket with automatic unbuffered stdout support
function runCInteractive(ws, code) {
    // Generate unique suffix to avoid file locking / permission denied issues
    const uniqueId = Date.now() + Math.random();
    const filePath = path.join(__dirname, `temp_main_${uniqueId}.c`);
    const outPath = path.join(__dirname, `temp_program_${uniqueId}.exe`);

    try {
        // Automatically inject unbuffered stdout configuration to prevent printf output buffering issues
        // This removes the need for users to manually type fflush(stdout) in their C code.
        let modifiedCode = code;
        if (modifiedCode.includes('int main(')) {
            modifiedCode = modifiedCode.replace(
                'int main(',
                '#include <stdio.h>\n__attribute__((constructor)) void auto_flush_init() { setvbuf(stdout, NULL, _IONBF, 0); setvbuf(stderr, NULL, _IONBF, 0); }\nint main('
            );
        }

        // Save modified code to unique .c file
        fs.writeFileSync(filePath, modifiedCode);

        // Compile unique C file into executable using gcc
        execSync(`gcc "${filePath}" -o "${outPath}"`);
    } catch (err) {
        if (ws.readyState === ws.OPEN) {
            const errorMsg = err.stderr ? err.stderr.toString() : err.message;
            ws.send(JSON.stringify({ type: 'error', data: errorMsg.replace(/\n/g, '<br>') }));
            ws.send(JSON.stringify({ type: 'close', exitCode: 1 }));
        }
        cleanup(filePath, outPath);
        return;
    }

    // Spawn the compiled executable for interactive execution
    const child = spawn(outPath, [], { stdio: ['pipe', 'pipe', 'pipe'] });

    // Send stdout to frontend
    child.stdout.on('data', (data) => {
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'output', data: data.toString() }));
        }
    });

    // Send stderr to frontend
    child.stderr.on('data', (data) => {
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'error', data: data.toString() }));
        }
    });

    // Handle input/stop messages from frontend (for scanf)
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

    // Clean up temporary files and notify close
    child.on('close', (exitCode) => {
        cleanup(filePath, outPath);
        if (ws.readyState === ws.OPEN) {
            ws.send(JSON.stringify({ type: 'close', exitCode }));
        }
    });
}

function cleanup(filePath, outPath) {
    // Delay slightly to ensure file handles are fully released by the OS before deletion
    setTimeout(() => {
        try {
            if (fs.existsSync(filePath)) fs.unlinkSync(filePath);
            if (fs.existsSync(outPath)) fs.unlinkSync(outPath);
        } catch (e) { }
    }, 500);
}

module.exports = { runCInteractive };