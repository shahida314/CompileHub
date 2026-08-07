const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

function runCppCode(code, input, callback) {
    const tempDir = path.join(__dirname, 'temp');
    if (!fs.existsSync(tempDir)) {
        fs.mkdirSync(tempDir);
    }

    const sourcePath = path.join(tempDir, 'temp_code.cpp');
    const inputPath = path.join(tempDir, 'input.txt');
    const exePath = path.join(tempDir, 'temp_code.exe');

    // ইউজার যে ইনপুট দিয়েছে (যদি খালি থাকে তবে ফাঁকা স্ট্রিম)
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

        // 3 সেকেন্ড টাইমআউট যেন ইনফিনিট লুপে না পড়ে
        exec(runCmd, { timeout: 3000 }, (runErr, runStdout, runStderr) => {
            if (runErr) {
                if (runErr.killed) {
                    callback({ output: "⚠️ Timeout: Program waited for input or hit an infinite loop." });
                } else {
                    callback({ output: runStderr || runErr.message });
                }
                return;
            }
            
            callback({ output: runStdout });
        });
    });
}

module.exports = { runCppCode };