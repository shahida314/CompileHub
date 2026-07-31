const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

function runPythonCode(code, callback) {
    const tempDir = path.join(__dirname, 'tests');
    if (!fs.existsSync(tempDir)) {
        fs.mkdirSync(tempDir, { recursive: true });
    }

    const tempPath = path.join(tempDir, 'temp_code.py');
    fs.writeFileSync(tempPath, code);

    // exec এর পরিবর্তে spawn ব্যবহার করা হলো যাতে লাইভ ইনপুট/আউটপুট কাজ করে
    const pythonProcess = spawn('python', [tempPath]);

    let stdoutData = '';
    let stderrData = '';

    pythonProcess.stdout.on('data', (data) => {
        stdoutData += data.toString();
        // টার্মিনালে লাইভ আউটপুট দেখতে চাইলে
        process.stdout.write(data);
    });

    pythonProcess.stderr.on('data', (data) => {
        stderrData += data.toString();
        process.stderr.write(data);
    });

    pythonProcess.on('close', (code) => {
        if (fs.existsSync(tempPath)) fs.unlinkSync(tempPath);

        if (code !== 0 || stderrData) {
            callback({ error: stderrData || `Process exited with code ${code}` });
        } else {
            callback({ output: stdoutData });
        }
    });
}

module.exports = { runPythonCode };