const { spawn, exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runCppInteractive(ws, code) {
    const tempDir = path.join(__dirname, 'temp');
    if (!fs.existsSync(tempDir)) {
        fs.mkdirSync(tempDir, { recursive: true });
    }

    const sourcePath = path.join(tempDir, 'temp_code.cpp');
    const exePath = path.join(tempDir, 'temp_code.exe');

    fs.writeFileSync(sourcePath, code);

    const compile = spawn('g++', [sourcePath, '-o', exePath]);

    let compileError = '';

    compile.stderr.on('data', (data) => {
        compileError += data.toString();
    });

    let cppProcess = null;

    compile.on('close', (codeExit) => {
        if (codeExit !== 0) {
            if (ws.readyState === ws.OPEN) {
                ws.send(JSON.stringify({
                    type: 'output',
                    data: `Compilation Error:\n${compileError}`
                }));
                ws.send(JSON.stringify({ type: 'exit', code: codeExit }));
            }
            return;
        }

        // Spawn compiled executable with unbuffered handling via shell execution tracking
        cppProcess = spawn(exePath, [], {
            shell: true,
            stdio: ['pipe', 'pipe', 'pipe']
        });

        const handleMessage = (message) => {
            try {
                const msg = JSON.parse(message);
                if (msg.type === 'input' && cppProcess && cppProcess.stdin.writable) {
                    cppProcess.stdin.write(msg.data + '\n');
                } else if (msg.type === 'stop') {
                    cppProcess.kill();
                }
            } catch (e) { }
        };

        ws.on('message', handleMessage);

        cppProcess.stdout.on('data', (data) => {
            if (ws.readyState === ws.OPEN) {
                ws.send(JSON.stringify({ type: 'output', data: data.toString() }));
            }
        });

        cppProcess.stderr.on('data', (data) => {
            if (ws.readyState === ws.OPEN) {
                ws.send(JSON.stringify({ type: 'output', data: data.toString() }));
            }
        });

        cppProcess.on('close', (exitCode) => {
            ws.removeListener('message', handleMessage);
            if (ws.readyState === ws.OPEN) {
                ws.send(JSON.stringify({ type: 'exit', code: exitCode }));
            }
        });
    });

    return {
        kill: () => {
            if (cppProcess) cppProcess.kill();
            if (compile) compile.kill();
        },
        get stdin() {
            return cppProcess ? cppProcess.stdin : null;
        }
    };
}

function runCppCode(code, input, callback) {
    const tempDir = path.join(__dirname, 'temp');
    if (!fs.existsSync(tempDir)) {
        fs.mkdirSync(tempDir, { recursive: true });
    }

    const sourcePath = path.join(tempDir, 'temp_code.cpp');
    const inputPath = path.join(tempDir, 'input.txt');
    const exePath = path.join(tempDir, 'temp_code.exe');

    const cleanInput = input ? input.trim() : '';

    fs.writeFileSync(sourcePath, code);
    fs.writeFileSync(inputPath, cleanInput + '\n');

    const compileCmd = `g++ "${sourcePath}" -o "${exePath}"`;

    exec(compileCmd, (compileErr, stdout, stderr) => {
        if (compileErr) {
            callback({ output: stderr || compileErr.message });
            return;
        }

        const runCmd = `"${exePath}" < "${inputPath}"`;

        exec(runCmd, { timeout: 3000 }, (runErr, runStdout, runStderr) => {
            if (runErr) {
                if (runErr.killed) {
                    callback({ output: "Timeout: Program waited for input or hit an infinite loop." });
                } else {
                    callback({ output: runStderr || runErr.message });
                }
                return;
            }

            callback({ output: runStdout });
        });
    });
}

module.exports = { runCppInteractive, runCppCode };