const { spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

function runJavaCompilerInteractive(code, { onOutput, onError, onExit }) {
    const tempFilePath = path.join(
        __dirname,
        `temp_${Date.now()}_${Math.floor(Math.random() * 10000)}.java`
    );

    fs.writeFileSync(tempFilePath, code);

    const executablePath = path.join(__dirname, 'java_compiler.exe');
    const child = spawn(executablePath, [tempFilePath]);

    child.stdout.on('data', (data) => {
        onOutput(data.toString());
    });

    child.stderr.on('data', (data) => {
        onError(data.toString());
    });

    child.on('close', (exitCode) => {
        if (fs.existsSync(tempFilePath)) {
            fs.unlinkSync(tempFilePath);
        }
        onExit(exitCode);
    });

    child.on('error', (err) => {
        onError(err.message);
        if (fs.existsSync(tempFilePath)) {
            fs.unlinkSync(tempFilePath);
        }
    });

    return child;
}

module.exports = { runJavaCompilerInteractive };